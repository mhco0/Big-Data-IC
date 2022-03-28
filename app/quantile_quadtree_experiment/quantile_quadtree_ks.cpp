#include <kolmogorov_smirnov/kolmogorov_smirnov.hpp>
#include <quantile_quadtree/quantile_quadtree.hpp>
#include <gk_factory/gk_factory.hpp>
#include <kll_factory/kll_factory.hpp>
#include <dcs_factory/dcs_factory.h>
#include <q_digest_factory/q_digest_factory.h>
#include <qsbd_debug/qsbd_debug.h>
#include <logger/logger.h>
#include <utils/utils.h>
#include <iostream>
#include <fstream>
#include <csv_parser/csv.h>
#define MIN_VALUE_IN_STREAM 0
#define MAX_VALUE_IN_STREAM 1023
using namespace std;
using namespace qsbd;
using namespace stream_maker;

class dummy_sketch : public quantile_sketch<int> {
	vector<int> samples;
public:
    dummy_sketch(){
    }

    ~dummy_sketch(){
    }

    void update(int x, int weight) override {
		for(int i = weight; i > 0; i--){
			samples.emplace_back(x);
		}
		return;
    }

	void update(int x) override {
		samples.emplace_back(x);
		return;
	}

    int query(int x) override {
        sort(samples.begin(), samples.end());
		return rank_from_samples(samples, x);
    }

	double cdf(int x) override {
		ASSERT(samples.size() > 0);
		int rank = this->query(x);

		double probability = rank / (double) samples.size();

		return probability;
	}

    void inner_merge(quantile_sketch<int>& rhs) override {
		dummy_sketch& rhs_cv = dynamic_cast<dummy_sketch&> (rhs);
        for(auto& it : rhs_cv.samples){
			this->samples.emplace_back(it);
		}
        return;
    }

    quantile_sketch<int>* merge(quantile_sketch<int>& rhs) override {
        dummy_sketch& rhs_cv = dynamic_cast<dummy_sketch&> (rhs);
		
		dummy_sketch * merged = new dummy_sketch();
		size_t i = 0, j = 0;

		for(; i < this->samples.size() and j < rhs_cv.samples.size(); i++, j++){
			merged->samples.emplace_back(this->samples[i]);
			merged->samples.emplace_back(rhs_cv.samples[j]);
		}

		if(i < this->samples.size()){
			merged->samples.emplace_back(this->samples[i]);			
		}

		if(j < rhs_cv.samples.size()){
			merged->samples.emplace_back(rhs_cv.samples[j]);
		}
		
		return merged;
    }

	/*vector<int> get_samples() override {
		return this->samples;
	}*/

    uint64_t get_heap_size() override {
        return sizeof(int) * samples.capacity();
    }
};

class dummy_factory : public sketch_factory<int> {
public:
    dummy_factory(){
    }

    quantile_sketch<int>* instance() override {
        return new dummy_sketch();
    }
};

using regions_samples = vector<pair<aabb<int>, aabb<int>>>;

struct ks_compair_qq_t {
	vector<double> left_region_cdf;
	vector<double> right_region_cdf;
	vector<double> lhs_gk_cdf;
	vector<double> lhs_kll_cdf;
	vector<double> lhs_q_digest_cdf;
	vector<double> lhs_dcs_cdf;
	vector<double> rhs_gk_cdf;
	vector<double> rhs_kll_cdf;
	vector<double> rhs_q_digest_cdf;
	vector<double> rhs_dcs_cdf;
	double max_distance_distributions;
	double max_distance_gk;
	double max_distance_kll;
	double max_distance_q_digest;
	double max_distance_dcs;
};

short direction(const aabb<int>& box, const point<int>& pos){
    int center_x = (box.bounds().first.x() + box.bounds().second.x()) / 2;
    int center_y = (box.bounds().first.y() + box.bounds().second.y()) / 2;

    if(pos.x() >= center_x and pos.y() >= center_y) return 0;
    else if(pos.x() < center_x and pos.y() >= center_y) return 1;
    else if(pos.x() < center_x and pos.y() < center_y) return 2;
    else if(pos.x() >= center_x and pos.y() < center_y) return 3;
    else return 0;
}

bool unit_box(const aabb<int>& region){
    int width = region.bounds().second.x() - region.bounds().first.x();
    int height = region.bounds().second.y() - region.bounds().first.y();
    return not (width > 1 || height > 1);
}

void change_box(aabb<int>& cur_box, short direction){
    int center_x = (cur_box.bounds().second.x() + cur_box.bounds().first.x()) / 2;
    int center_y = (cur_box.bounds().second.y() + cur_box.bounds().first.y()) / 2;

    switch (direction){
        case 0 : {
            // ne
            cur_box.bounds(center_x, center_y, cur_box.bounds().second.x(), cur_box.bounds().second.y());
        }
        break;
        case 1 : {
            // nw
            cur_box.bounds(cur_box.bounds().first.x(), center_y, center_x, cur_box.bounds().second.y());
        }
        break;
        case 2 : {
            // sw
            cur_box.bounds(cur_box.bounds().first.x(), cur_box.bounds().first.y(), center_x, center_y);
        }
        break;
        case 3 : {
            // se
            cur_box.bounds(center_x, cur_box.bounds().first.y(), cur_box.bounds().second.x(), center_y);
        }
        break;
        default:{
            DEBUG_ERR("Invalid direction on quantile_quadtree::change_box");
        }
        break;
    }
}

void write_bounds(aabb<int>& root_box, int depth, const int& max_depth, vector<aabb<int>>& out){

    if(depth == max_depth or unit_box(root_box)){
        out.emplace_back(root_box);
		return;
    }

    for(int i = 0; i < 4; i++){
        aabb<int> child_box(root_box);
        change_box(child_box, i);
        write_bounds(child_box, depth + 1, max_depth, out);    
    }

    return;
}


regions_samples all_pairs_in_resolution(const int& depth){
	aabb<int> max_bound(0, 0, 1 << depth, 1 << depth);
	vector<aabb<int>> bounds;

	regions_samples all_pairs;

	for(int i = 0; i <= depth; i++){
		write_bounds(max_bound, 0, i, bounds);
	}

	for(size_t i = 0; i < bounds.size(); i++){
		for(size_t j = i + 1; j < bounds.size(); j++){
			all_pairs.emplace_back(bounds[i], bounds[j]);
		}
	}

	return all_pairs;
}

void rprint(const string& text, const aabb<int>& region, const vector<int>& samples){
	cout << text << endl;
	cout << region << ": ";
	
	vector<int> copy(samples);

	sort(copy.begin(), copy.end());

	for(auto& it : copy){
		cout << it << " ";
	}
	
	cout << endl;
}

pair<vector<double>, vector<double>> real_cdfs_from_region_pair(const vector<pair<int, pair<double, double>>>& stream, const pair<aabb<int>, aabb<int>>& regions, const int& depth, const double * bounds, const int& start_point, const int& end_point, const int& step, bool& both_have_data){

	vector<int> lhs_samples;
	vector<int> rhs_samples;

	for(auto& it : stream){
		point<int> coord(map_coord(it.second.first, bounds[0], bounds[2], depth), map_coord(it.second.second, bounds[1], bounds[3], depth));
		
		if (regions.first.contains(coord)){
			lhs_samples.push_back(it.first);
		}

		if (regions.second.contains(coord)){
			rhs_samples.push_back(it.first);
		}
	}


	if(rhs_samples.size() != 0 and lhs_samples.size() != 0){
		both_have_data = true;

		//rprint("left region", regions.first, lhs_samples);
		//rprint("right region", regions.second, rhs_samples);

		vector<double> lhs_cdf = cdf_from_samples(lhs_samples, start_point, end_point, step);
		vector<double> rhs_cdf = cdf_from_samples(rhs_samples, start_point, end_point, step);

		return {lhs_cdf, rhs_cdf};
	}

	return {{}, {}};
}

vector<pair<int, pair<double, double>>> stream_by_csv(const string& filename){
	vector<pair<int, pair<double, double>>> stream;
	io::CSVReader<3, io::trim_chars<' ', '\t'>, io::no_quote_escape<','>, io::throw_on_overflow, io::empty_line_comment> in(filename);
	in.read_header(io::ignore_extra_column, "passenger_count", "pickup_longitude", "pickup_latitude");
	double lon, lat;
	int pass_cout;
	
	while(in.read_row(pass_cout, lon, lat)){
		//cout << pass_cout << " " << lon << " " << lat << endl;
		stream.push_back({pass_cout, {lon, lat}});
	}

	return stream;
}

vector<ks_compair_qq_t> test_quantile_quadtree_ks(const int& samples, const int& start_point, const int& end_point, const int& step, const int& citys, const double& max_radius, const double& err, const int& universe, const int& depth, const bool& only_leafs, const double * bounds, const string& filename = ""){
	int discrete_bounds[4] = {0, 0, 0, 0};

	cout << "Discrete bounds : " << endl;
	for(int i = 0; i < 4; i++){
		// 0 -> 0 2
		// 1 -> 1 3
		// 2 -> 0 2 
		// 3 -> 1 3
		if(i & 1){
			discrete_bounds[i] = map_coord(bounds[i], bounds[1], bounds[3], depth);
		}else{
			discrete_bounds[i] = map_coord(bounds[i], bounds[0], bounds[2], depth);
		}

		cout << discrete_bounds[i] << " ";
	}
	cout << endl;

	cout << "Only leafs : " << only_leafs << endl;

 	aabb<int> bound_box(discrete_bounds[0], discrete_bounds[1], discrete_bounds[2], discrete_bounds[3]);
	regions_samples regions_to_search = all_pairs_in_resolution(depth);
	vector<int> values_to_search;
	vector<pair<int, pair<double, double>>> stream;
	
	if (filename == ""){
		stream = random_stream_city(samples, bounds[0], bounds[1], bounds[2], bounds[3], MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM, citys, max_radius);
	}else{
		stream = stream_by_csv(filename);
	}
	//cout << stream << endl;
	vector<ks_compair_qq_t> tests;

	q_digest_factory qd_factory(err, universe);
	dcs_factory dcs_factory(err, universe);
	kll_factory<int> kll_factory(err);
	gk_factory<int> gk_factory(err);
	//dummy_factory dm_factory;

    quantile_quadtree<int> qq_qd_test(bound_box, depth, &qd_factory, only_leafs);
	quantile_quadtree<int> qq_dcs_test(bound_box, depth, &dcs_factory, only_leafs);
	quantile_quadtree<int> qq_kll_test(bound_box, depth, &kll_factory, only_leafs);
	quantile_quadtree<int> qq_gk_test(bound_box, depth, &gk_factory, only_leafs); 
	//quantile_quadtree<int> qq_dm_test(bound_box, depth, &dm_factory, only_leafs);


    for(size_t i = 0; i < stream.size(); i++){
        point<int> coord(map_coord(stream[i].second.first, bounds[0], bounds[2], depth), map_coord(stream[i].second.second, bounds[1], bounds[3], depth));

        qq_qd_test.update(coord, stream[i].first, 1);
		qq_dcs_test.update(coord, stream[i].first, 1);
		qq_kll_test.update(coord, stream[i].first);
		qq_gk_test.update(coord, stream[i].first);
		//qq_dm_test.update(coord, stream[i].first);
    }	

	for(int i = start_point; i < end_point; i++){
		values_to_search.emplace_back(i);
	}

	for(auto& region : regions_to_search){
		bool both_have_data = false;
		pair<vector<double>, vector<double>> real_cdfs = real_cdfs_from_region_pair(stream, region, depth, bounds, start_point, end_point, step, both_have_data);
		vector<double> lhs_cdf = real_cdfs.first;
		vector<double> rhs_cdf = real_cdfs.second;
		double max_distance_distributions = numeric_limits<double>::min();
		double max_distance_q_digest = numeric_limits<double>::min();
		double max_distance_dcs = numeric_limits<double>::min();
		double max_distance_kll = numeric_limits<double>::min();
		double max_distance_gk = numeric_limits<double>::min();
		//double max_distance_dm = numeric_limits<double>::min();

		/*if (both_have_data){
			rprint("left region", region.first, qq_dm_test.get_samples(region.first));
			rprint("right region", region.second, qq_dm_test.get_samples(region.second));
		}*/

		if(both_have_data){
			vector<double> lhs_q_digest_cdf = qq_qd_test.cdfs(region.first, values_to_search);
			vector<double> lhs_dcs_cdf = qq_dcs_test.cdfs(region.first, values_to_search);
			vector<double> lhs_kll_cdf = qq_kll_test.cdfs(region.first, values_to_search);
			vector<double> lhs_gk_cdf = qq_gk_test.cdfs(region.first, values_to_search);
			//vector<double> lhs_dm_cdf = qq_dm_test.cdfs(region.first, values_to_search);

			vector<double> rhs_q_digest_cdf = qq_qd_test.cdfs(region.second, values_to_search);
			vector<double> rhs_dcs_cdf = qq_dcs_test.cdfs(region.second, values_to_search);
			vector<double> rhs_kll_cdf = qq_kll_test.cdfs(region.second, values_to_search);
			vector<double> rhs_gk_cdf = qq_gk_test.cdfs(region.second, values_to_search);
			//vector<double> rhs_dm_cdf = qq_dm_test.cdfs(region.second, values_to_search);

			//ASSERT(lhs_dm_cdf.size() == rhs_dm_cdf.size() and lhs_cdf.size() == lhs_dm_cdf.size() and rhs_cdf.size() == rhs_cdf.size());

			//cout << "lhs : " << lhs_cdf << endl;
			//cout << "lhs dm : " << lhs_dm_cdf << endl;
			//cout << "rhs : " << rhs_cdf << endl;
			//cout << "rhs dm : " << rhs_dm_cdf << endl;

			/*for(size_t i = 0; i < lhs_cdf.size(); i++){
				ASSERT(fabs(lhs_cdf[i] - lhs_dm_cdf[i]) <= 1e-6);
			}

			for(size_t i = 0; i < rhs_cdf.size(); i++){
				ASSERT(fabs(rhs_cdf[i] - rhs_dm_cdf[i]) <= 1e-6);
			}*/

			ASSERT(lhs_cdf.size() == rhs_cdf.size());
			ASSERT((lhs_cdf.size() == lhs_q_digest_cdf.size()) and (lhs_q_digest_cdf.size() == rhs_q_digest_cdf.size()));
			ASSERT((lhs_cdf.size() == lhs_dcs_cdf.size()) and (lhs_dcs_cdf.size() == rhs_dcs_cdf.size()));
			ASSERT((lhs_cdf.size() == lhs_kll_cdf.size()) and (lhs_kll_cdf.size() == rhs_kll_cdf.size()));
			ASSERT((lhs_cdf.size() == lhs_gk_cdf.size()) and (lhs_gk_cdf.size() == rhs_gk_cdf.size()));
			//ASSERT((lhs_cdf.size() == lhs_dm_cdf.size()) and (lhs_dm_cdf.size() == rhs_dm_cdf.size()));

			for(size_t i = 0; i < lhs_cdf.size(); i++){
				double distribution_distance = fabs(lhs_cdf[i] - rhs_cdf[i]);
				double q_digest_distance = fabs(lhs_q_digest_cdf[i] - rhs_q_digest_cdf[i]);
				double dcs_distance = fabs(lhs_dcs_cdf[i] - rhs_dcs_cdf[i]);
				double kll_distance = fabs(lhs_kll_cdf[i] - rhs_kll_cdf[i]);
				double gk_distance = fabs(lhs_gk_cdf[i] - rhs_gk_cdf[i]);
				//double dm_distance = fabs(lhs_dm_cdf[i] - rhs_dm_cdf[i]);

				//ASSERT(fabs(distribution_distance - dm_distance) <= 1e-6);
			
				max_distance_distributions = max(max_distance_distributions, distribution_distance);
				max_distance_q_digest = max(max_distance_q_digest, q_digest_distance);
				max_distance_dcs = max(max_distance_dcs, dcs_distance);
				max_distance_kll = max(max_distance_kll, kll_distance);
				max_distance_gk = max(max_distance_gk, gk_distance);
				//max_distance_dm = max(max_distance_dm, dm_distance);
			}

			//ASSERT(fabs(max_distance_distributions - max_distance_dm) <= 1e-6);
		
			ks_compair_qq_t test = {
				.left_region_cdf = lhs_cdf,
				.right_region_cdf = rhs_cdf, 
				.lhs_gk_cdf = lhs_gk_cdf,
				.lhs_kll_cdf = lhs_kll_cdf,
				.lhs_q_digest_cdf = lhs_q_digest_cdf,
				.lhs_dcs_cdf = lhs_dcs_cdf,
				.rhs_gk_cdf = rhs_gk_cdf,
				.rhs_kll_cdf = rhs_kll_cdf,
				.rhs_q_digest_cdf = rhs_q_digest_cdf,
				.rhs_dcs_cdf = rhs_dcs_cdf,
				.max_distance_distributions = max_distance_distributions,
				.max_distance_gk = max_distance_gk,
				.max_distance_kll = max_distance_kll,
				.max_distance_q_digest = max_distance_q_digest,
				.max_distance_dcs = max_distance_dcs,
			};

			tests.emplace_back(test);
		}
	}

	return tests;
}

void help(){
    cout << "This program shows some results of the test for the kolmogorov-smirnov algorithm in the quantile quadtree." << endl;
	cout << "Usage: " << endl;
	cout << "./quantile_quadtree_ks samples start_point end_point step citys max_radius error universe depth only_leafs min_x min_y max_x max_y" << endl;
	cout << "samples: The number of samples collected by each distribution" << endl;
	cout << "start_point: The start point to collect in the cdf" << endl;
	cout << "end_point: The end point to collect in the cdf" << endl;
	cout << "step: The step between the end and the start point" << endl;
	cout << "citys: The number of citys in the distribution" << endl;
	cout << "max_radius: The max radius for each city in the distribution" << endl;
	cout << "error: The epsilon error for each sketch" << endl;
	cout << "universe: The universe of values range for the sketches that need it" << endl;
    cout << "depth: The max depth for the quantile quadtree" << endl;
    cout << "only_leafs: A boolean (0 or 1) to say if the quantile quadtree should be using sketches only in the leafs of the tree" << endl;
    cout << "min_x: The minimum x bound for the points" << endl; 
    cout << "min_y: The minimum y bound for the points" << endl; 
    cout << "max_x: The maximum x bound for the points" << endl; 
    cout << "max_y: The maximum y bound for the points" << endl;
}

void save_csv_compair(const string& prefix, const vector<ks_compair_qq_t>& tests){
	string ks_filename = "ks_distances.csv";
	fstream out;

	out.open(prefix + ks_filename, fstream::out);

	out << "max_distance_distributions,max_distance_gk,max_distance_kll,max_distance_q_digest,max_distance_dcs\n";

	for(const ks_compair_qq_t& test : tests){
		out << test.max_distance_distributions << "," << test.max_distance_gk << "," << test.max_distance_kll << "," << test.max_distance_q_digest << "," << test.max_distance_dcs << "\n";
	}

	out.close();
}

int main(int argc, char* argv[]){
    deque<string> args = process_args(argc, argv); 

    if (not (args.size() == 14 or args.size() == 15)){
        help();
        return -1;
    }

    int samples = stoi(args[0]);
	int start_point = stoi(args[1]);
	int end_point = stoi(args[2]);
	int step = stoi(args[3]);
	int citys = stoi(args[4]);
	double max_radius = stod(args[5]);
	double error = stod(args[6]);
	int universe = stoi(args[7]);
	int depth = stoi(args[8]);
    bool only_leafs = (bool) stoi(args[9]);
	double bounds[4] = {stod(args[10]), stod(args[11]), stod(args[12]), stod(args[13])};
	string filename = "";

	if(args.size() == 15){
		filename = args[14];
	}

	vector<ks_compair_qq_t> tests = test_quantile_quadtree_ks(samples, start_point, end_point, step, citys, max_radius, error, universe, depth, only_leafs, bounds, filename);

	string use_leafs = only_leafs ? "leaf_" : "";

	save_csv_compair("file_ks_quantile_test_" + to_string(depth) + "_" + use_leafs + to_string(error) + "_", tests);
 
    return 0;
}