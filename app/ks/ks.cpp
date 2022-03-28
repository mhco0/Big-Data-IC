#include <kolmogorov_smirnov/kolmogorov_smirnov.hpp>
#include <kll/kll.hpp>
#include <gk/gk.hpp>
#include <q_digest/q_digest.h>
#include <dcs/dcs.h>
#include <iostream>
#include <fstream>
#define MIN_VALUE_IN_STREAM 0
#define MAX_VALUE_IN_STREAM 1023
using namespace std;
using namespace qsbd;
using namespace stream_maker;

struct ks_compair_test_result_t{
	vector<int> distribution_lhs;
	vector<int> distribution_rhs;
	vector<double> cdf_distribution_lhs;
	vector<double> cdf_distribution_rhs;
	vector<double> sketch_cdf_distribution_lhs;
	vector<double> sketch_cdf_distribution_rhs;
	vector<double> distances_distributions;
	vector<double> distances_sketches;
	double max_distance_distributions;
	double max_distance_sketches;
};

struct ks_test_result_t{
	vector<int> distribution;
	vector<double> cdf_distribution;
	vector<double> sketch_cdf_distribution;
	vector<double> distances;
	double max_distance;
};

using samples_vector = vector<pair<vector<int>, vector<int>>>;

samples_vector all_pair_distributions_from(const int& samples, const double& min_mean, const double& max_mean, const double& step_mean, const double& min_std, const double& max_std, const double& step_std){
	vector<vector<int>> vsamples;
	samples_vector all_pairs;

	for(double mean = min_mean; mean <= max_mean; mean += step_mean){
		for(double std = min_std; std <= max_std; std += step_std){
			vsamples.emplace_back(normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM));
		}
	}

	for(int i = 0; i < vsamples.size(); i++){
		for(int j = i + 1; j < vsamples.size(); j++){
			all_pairs.emplace_back(vsamples[i], vsamples[j]);
		}
	}

	return all_pairs;
}

ks_compair_test_result_t test_compair_normal_distribution_with_kll(const vector<int>& lhs_samples, const vector<int>& rhs_samples, const int& start, const int& end, const int& step, const double& error){
	vector<double> distances_distributions;
	vector<double> distances_sketches;
	vector<double> sketch_cdf_distribution_lhs;
	vector<double> sketch_cdf_distribution_rhs;
	double max_dist_distributions = numeric_limits<double>::min();
	double max_dist_sketches = numeric_limits<double>::min();
	kll<int> lhs_sketch(error);
	kll<int> rhs_sketch(error);

   	vector<double> cdf_lhs = cdf_from_samples(lhs_samples, start, end, step);
	vector<double> cdf_rhs = cdf_from_samples(rhs_samples, start, end, step);

	for(size_t i = 0; i < cdf_lhs.size(); i++){
		double distance = fabs(cdf_lhs[i] - cdf_rhs[i]);
		distances_distributions.push_back(distance);
		max_dist_distributions = max(max_dist_distributions, distance);
	}

	for(size_t i = 0; i < lhs_samples.size(); i++){
		ASSERT(lhs_samples[i] >= 0);
		lhs_sketch.update(lhs_samples[i]);
	}

	for(size_t i = 0; i < rhs_samples.size(); i++){
		ASSERT(rhs_samples[i] >= 0);
		rhs_sketch.update(rhs_samples[i]);
	}

	for(int i = start; i < end; i += step){
		double sketch_lhs_cdf = lhs_sketch.cdf(i);
		double sketch_rhs_cdf = rhs_sketch.cdf(i);
		double distance = fabs(sketch_lhs_cdf - sketch_rhs_cdf);
		
		sketch_cdf_distribution_lhs.push_back(sketch_lhs_cdf);
		sketch_cdf_distribution_rhs.push_back(sketch_rhs_cdf);
		distances_sketches.push_back(distance);
		
		max_dist_sketches = max(max_dist_sketches, distance);
	}

	return {.distribution_lhs = lhs_samples, .distribution_rhs = rhs_samples, .cdf_distribution_lhs = cdf_lhs, .cdf_distribution_rhs = cdf_rhs, .sketch_cdf_distribution_lhs = sketch_cdf_distribution_lhs, .sketch_cdf_distribution_rhs = sketch_cdf_distribution_rhs, .distances_distributions = distances_distributions, .distances_sketches = distances_sketches, .max_distance_distributions = max_dist_distributions, .max_distance_sketches = max_dist_sketches};
}

ks_compair_test_result_t test_compair_normal_distribution_with_dcs(const vector<int>& lhs_samples, const vector<int>& rhs_samples, const int& start, const int& end, const int& step, const double& error,  const int& universe){
	vector<double> distances_distributions;
	vector<double> distances_sketches;
	vector<double> sketch_cdf_distribution_lhs;
	vector<double> sketch_cdf_distribution_rhs;
	double max_dist_distributions = numeric_limits<double>::min();
	double max_dist_sketches = numeric_limits<double>::min();
	dcs lhs_sketch(error, universe);
	dcs rhs_sketch(error, universe);

   	vector<double> cdf_lhs = cdf_from_samples(lhs_samples, start, end, step);
	vector<double> cdf_rhs = cdf_from_samples(rhs_samples, start, end, step);

	for(size_t i = 0; i < cdf_lhs.size(); i++){
		double distance = fabs(cdf_lhs[i] - cdf_rhs[i]);
		distances_distributions.push_back(distance);
		max_dist_distributions = max(max_dist_distributions, distance);
	}

	for(size_t i = 0; i < lhs_samples.size(); i++){
		ASSERT(lhs_samples[i] >= 0 and lhs_samples[i] < universe);
		lhs_sketch.update(lhs_samples[i], 1);
	}

	for(size_t i = 0; i < rhs_samples.size(); i++){
		ASSERT(rhs_samples[i] >= 0 and rhs_samples[i] < universe);
		rhs_sketch.update(rhs_samples[i], 1);
	}

	for(int i = start; i < end; i += step){
		double sketch_lhs_cdf = lhs_sketch.cdf(i);
		double sketch_rhs_cdf = rhs_sketch.cdf(i);
		double distance = fabs(sketch_lhs_cdf - sketch_rhs_cdf);
		
		sketch_cdf_distribution_lhs.push_back(sketch_lhs_cdf);
		sketch_cdf_distribution_rhs.push_back(sketch_rhs_cdf);
		distances_sketches.push_back(distance);
		
		max_dist_sketches = max(max_dist_sketches, distance);
	}

	return {.distribution_lhs = lhs_samples, .distribution_rhs = rhs_samples, .cdf_distribution_lhs = cdf_lhs, .cdf_distribution_rhs = cdf_rhs, .sketch_cdf_distribution_lhs = sketch_cdf_distribution_lhs, .sketch_cdf_distribution_rhs = sketch_cdf_distribution_rhs, .distances_distributions = distances_distributions, .distances_sketches = distances_sketches, .max_distance_distributions = max_dist_distributions, .max_distance_sketches = max_dist_sketches};
}

ks_compair_test_result_t test_compair_normal_distribution_with_q_digest(const vector<int>& lhs_samples, const vector<int>& rhs_samples, const int& start, const int& end, const int& step, const double& error,  const int& universe){
	vector<double> distances_distributions;
	vector<double> distances_sketches;
	vector<double> sketch_cdf_distribution_lhs;
	vector<double> sketch_cdf_distribution_rhs;
	double max_dist_distributions = numeric_limits<double>::min();
	double max_dist_sketches = numeric_limits<double>::min();
	q_digest lhs_sketch(error, universe);
	q_digest rhs_sketch(error, universe);

   	vector<double> cdf_lhs = cdf_from_samples(lhs_samples, start, end, step);
	vector<double> cdf_rhs = cdf_from_samples(rhs_samples, start, end, step);

	for(size_t i = 0; i < cdf_lhs.size(); i++){
		double distance = fabs(cdf_lhs[i] - cdf_rhs[i]);
		distances_distributions.push_back(distance);
		max_dist_distributions = max(max_dist_distributions, distance);
	}

	for(size_t i = 0; i < lhs_samples.size(); i++){
		ASSERT(lhs_samples[i] >= 0 and lhs_samples[i] < universe);
		lhs_sketch.update(lhs_samples[i], 1);
	}

	for(size_t i = 0; i < rhs_samples.size(); i++){
		ASSERT(rhs_samples[i] >= 0 and rhs_samples[i] < universe);
		rhs_sketch.update(rhs_samples[i], 1);
	}

	for(int i = start; i < end; i += step){
		double sketch_lhs_cdf = lhs_sketch.cdf(i);
		double sketch_rhs_cdf = rhs_sketch.cdf(i);
		double distance = fabs(sketch_lhs_cdf - sketch_rhs_cdf);
		
		sketch_cdf_distribution_lhs.push_back(sketch_lhs_cdf);
		sketch_cdf_distribution_rhs.push_back(sketch_rhs_cdf);
		distances_sketches.push_back(distance);
		
		max_dist_sketches = max(max_dist_sketches, distance);
	}

	return {.distribution_lhs = lhs_samples, .distribution_rhs = rhs_samples, .cdf_distribution_lhs = cdf_lhs, .cdf_distribution_rhs = cdf_rhs, .sketch_cdf_distribution_lhs = sketch_cdf_distribution_lhs, .sketch_cdf_distribution_rhs = sketch_cdf_distribution_rhs, .distances_distributions = distances_distributions, .distances_sketches = distances_sketches, .max_distance_distributions = max_dist_distributions, .max_distance_sketches = max_dist_sketches};
}

ks_compair_test_result_t test_compair_normal_distribution_with_gk(const vector<int>& lhs_samples, const vector<int>& rhs_samples, const int& start, const int& end, const int& step, const double& error){
	vector<double> distances_distributions;
	vector<double> distances_sketches;
	vector<double> sketch_cdf_distribution_lhs;
	vector<double> sketch_cdf_distribution_rhs;
	double max_dist_distributions = numeric_limits<double>::min();
	double max_dist_sketches = numeric_limits<double>::min();
	gk<int> lhs_sketch(error);
	gk<int> rhs_sketch(error);

   	vector<double> cdf_lhs = cdf_from_samples(lhs_samples, start, end, step);
	vector<double> cdf_rhs = cdf_from_samples(rhs_samples, start, end, step);

	for(size_t i = 0; i < cdf_lhs.size(); i++){
		double distance = fabs(cdf_lhs[i] - cdf_rhs[i]);
		distances_distributions.push_back(distance);
		max_dist_distributions = max(max_dist_distributions, distance);
	}

	for(size_t i = 0; i < lhs_samples.size(); i++){
		ASSERT(lhs_samples[i] >= 0);
		lhs_sketch.update(lhs_samples[i]);
	}

	for(size_t i = 0; i < rhs_samples.size(); i++){
		ASSERT(rhs_samples[i] >= 0);
		rhs_sketch.update(rhs_samples[i]);
	}

	for(int i = start; i < end; i += step){
		double sketch_lhs_cdf = lhs_sketch.cdf(i);
		double sketch_rhs_cdf = rhs_sketch.cdf(i);
		double distance = fabs(sketch_lhs_cdf - sketch_rhs_cdf);
		
		sketch_cdf_distribution_lhs.push_back(sketch_lhs_cdf);
		sketch_cdf_distribution_rhs.push_back(sketch_rhs_cdf);
		distances_sketches.push_back(distance);
		
		max_dist_sketches = max(max_dist_sketches, distance);
	}

	return {.distribution_lhs = lhs_samples, .distribution_rhs = rhs_samples, .cdf_distribution_lhs = cdf_lhs, .cdf_distribution_rhs = cdf_rhs, .sketch_cdf_distribution_lhs = sketch_cdf_distribution_lhs, .sketch_cdf_distribution_rhs = sketch_cdf_distribution_rhs, .distances_distributions = distances_distributions, .distances_sketches = distances_sketches, .max_distance_distributions = max_dist_distributions, .max_distance_sketches = max_dist_sketches};
}

pair<double, vector<double>> test_distinct_normal_distribution(const int& samples, const int& start, const int& end, const int& step, const double& mean_lhs, const double& std_lhs, const double& mean_rhs, const double& std_rhs){
	vector<int> lhs = normal_int_stream(samples, mean_lhs, std_lhs, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);
	vector<int> rhs = normal_int_stream(samples, mean_rhs, std_rhs, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);

	return distributions_ks(lhs, rhs, start, end, step);
}

ks_test_result_t test_ks_with_kll(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error){
	vector<double> distances;
	vector<double> sketch_cdfs;
	double max_dist = numeric_limits<double>::min();
	kll<int> sketch(error);

	vector<int> distribution = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);

	for(size_t i = 0; i < distribution.size(); i++){
		ASSERT(distribution[i] >= 0);
		sketch.update(distribution[i]);
	}

	vector<double> cdf_samples = cdf_from_samples(distribution, start, end, step);

	for(int i = start, j = 0; i < end; i += step, j++){
		double sketch_cdf = sketch.cdf(i);
		double distance = fabs(cdf_samples[j] - sketch_cdf);
		
		sketch_cdfs.push_back(sketch_cdf);
		distances.push_back(distance);
		
		max_dist = max(max_dist, distance);
	}
	
	return {.distribution = distribution, .cdf_distribution = cdf_samples, .sketch_cdf_distribution = sketch_cdfs, .distances = distances, .max_distance = max_dist};
}

ks_test_result_t test_ks_with_dcs(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error, const int& universe){
	vector<double> distances;
	vector<double> sketch_cdfs;
	double max_dist = numeric_limits<double>::min();
	dcs sketch(error, universe);

	vector<int> distribution = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);

	for(size_t i = 0; i < distribution.size(); i++){
		ASSERT(distribution[i] >= 0 and distribution[i] < universe);
		sketch.update(distribution[i], 1);
	}

	auto cdf_samples = cdf_from_samples(distribution, start, end, step);

	for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
		double sketch_cdf = sketch.cdf(i);
		double distance = fabs(cdf_samples[j] - sketch_cdf);

		sketch_cdfs.push_back(sketch_cdf);
		distances.push_back(distance);

		max_dist = max(max_dist, distance);
	}

	return {.distribution = distribution, .cdf_distribution = cdf_samples, .sketch_cdf_distribution = sketch_cdfs, .distances = distances, .max_distance = max_dist};
}

ks_test_result_t test_ks_with_q_digest(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error, const int& universe){
	vector<double> distances;
	vector<double> sketch_cdfs;
	double max_dist = numeric_limits<double>::min();
	q_digest sketch(error, universe);

	vector<int> distribution = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);

	for(size_t i = 0; i < distribution.size(); i++){
		ASSERT(distribution[i] >= 0  and distribution[i] < universe);
		sketch.update(distribution[i], 1);
	}

	auto cdf_samples = cdf_from_samples(distribution, start, end, step);

	for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
		double sketch_cdf = sketch.cdf(i);
		double distance = fabs(cdf_samples[j] - sketch_cdf);
		
		sketch_cdfs.push_back(sketch_cdf);
		distances.push_back(distance);

		max_dist = max(max_dist, distance);
	}
	
	return {.distribution = distribution, .cdf_distribution = cdf_samples, .sketch_cdf_distribution = sketch_cdfs, .distances = distances, .max_distance = max_dist};
}

ks_test_result_t test_ks_with_gk(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error){
	vector<double> distances;
	vector<double> sketch_cdfs;
	double max_dist = numeric_limits<double>::min();
	gk<int> sketch(error);

	vector<int> distribution = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);

	for(size_t i = 0; i < distribution.size(); i++){
		ASSERT(distribution[i] >= 0);
		sketch.update(distribution[i]);
	}

	auto cdf_samples = cdf_from_samples(distribution, start, end, step);

	for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
		double sketch_cdf = sketch.cdf(i);
		double distance = fabs(cdf_samples[j] - sketch_cdf);
		
		sketch_cdfs.push_back(sketch_cdf);
		distances.push_back(distance);
		
		max_dist = max(max_dist, distance);
	}

	return {.distribution = distribution, .cdf_distribution = cdf_samples, .sketch_cdf_distribution = sketch_cdfs, .distances = distances, .max_distance = max_dist};
}

void help(){
	cout << "This program shows some results of the test for the kolmogorov-smirnov algorithm." << endl;
	cout << "Usage: " << endl;
	cout << "./ks samples start_point end_point step min_mean_range max_mean_range min_std_range max_std_range error universe" << endl;
	cout << "samples: The number of samples collected by each distribution" << endl;
	cout << "start_point: The start point to collect in the cdf" << endl;
	cout << "end_point: The end point to collect in the cdf" << endl;
	cout << "step: The step between the end and the start point" << endl;
	cout << "min_mean_range: The minimum mean value for the range of tested values for the mean in the distribution" << endl;
	cout << "max_mean_range: The maximum mean value for the range of tested values for the mean in the distribution" << endl;
	cout << "min_std_range: The minimum std value for the range of tested values for the std in the distribution" << endl;
	cout << "max_std_range: The maximum std value for the range of tested values for the std in the distribution" << endl;
	cout << "error: The epsilon error for each sketch" << endl;
	cout << "universe: The universe of values range for the sketches that need it" << endl;
}

ostream& operator<<(ostream& out, const ks_test_result_t& res){
	out << "Distribution : ";
	for(size_t i = 0; i < res.distribution.size(); i++){
		out << res.distribution[i] << " \n"[ i == res.distribution.size() - 1];
	}

	out << "CDF : ";
	for(size_t i = 0; i < res.cdf_distribution.size(); i++){
		out << res.cdf_distribution[i] << " \n"[ i == res.cdf_distribution.size() - 1];
	}

	out << "Sketch CDF : ";
	for(size_t i = 0; i < res.sketch_cdf_distribution.size(); i++){
		out << res.sketch_cdf_distribution[i] << " \n"[ i == res.sketch_cdf_distribution.size() - 1];
	}

	out << "Distances : ";
	for(size_t i = 0; i < res.distances.size(); i++){
		out << res.distances[i] << " \n"[ i == res.distances.size() - 1];
	}

	out << "Max distance : " << res.max_distance << "\n";

	return out;
}

ostream& operator<<(ostream& out, const ks_compair_test_result_t& res){
	out << "Left Distribution : ";
	for(size_t i = 0; i < res.distribution_lhs.size(); i++){
		out << res.distribution_lhs[i] << " \n"[ i == res.distribution_lhs.size() - 1];
	}

	out << "Right Distribution : ";
	for(size_t i = 0; i < res.distribution_rhs.size(); i++){
		out << res.distribution_rhs[i] << " \n"[ i == res.distribution_rhs.size() - 1];
	}

	out << "Left CDF : ";
	for(size_t i = 0; i < res.cdf_distribution_lhs.size(); i++){
		out << res.cdf_distribution_lhs[i] << " \n"[ i == res.cdf_distribution_lhs.size() - 1];
	}

	out << "Right CDF : ";
	for(size_t i = 0; i < res.cdf_distribution_rhs.size(); i++){
		out << res.cdf_distribution_rhs[i] << " \n"[ i == res.cdf_distribution_rhs.size() - 1];
	}

	out << "Left Sketch CDF : ";
	for(size_t i = 0; i < res.sketch_cdf_distribution_lhs.size(); i++){
		out << res.sketch_cdf_distribution_lhs[i] << " \n"[ i == res.sketch_cdf_distribution_lhs.size() - 1];
	}

	out << "Right Sketch CDF : ";
	for(size_t i = 0; i < res.sketch_cdf_distribution_rhs.size(); i++){
		out << res.sketch_cdf_distribution_rhs[i] << " \n"[ i == res.sketch_cdf_distribution_rhs.size() - 1];
	}

	out << "Distances Distributions : ";
	for(size_t i = 0; i < res.distances_distributions.size(); i++){
		out << res.distances_distributions[i] << " \n"[ i == res.distances_distributions.size() - 1];
	}

	out << "Distances Sketches : ";
	for(size_t i = 0; i < res.distances_sketches.size(); i++){
		out << res.distances_sketches[i] << " \n"[ i == res.distances_sketches.size() - 1];
	}

	out << "Max distance in distributions: " << res.max_distance_distributions << "\n";

	out << "Max distance in sketches: " << res.max_distance_sketches << "\n";

	return out;
}

void save_csv_compair(const string& prefix, const vector<ks_compair_test_result_t>& tests){
	string filename = "ks_distances_sketches.csv";
	fstream out;

	out.open(prefix + filename, fstream::out);

	out << "max_distance_distributions,max_distance_sketches\n";

	for(const ks_compair_test_result_t& test : tests){
		out << test.max_distance_distributions << "," << test.max_distance_sketches << "\n";
	}

	out.close();
}

void save_compair_test(const string& prefix, const ks_compair_test_result_t& res){
	string dist_fn = "compair_distribution.dat";
	string cdf_fn = "compair_cdf.dat";
	string sketch_cdf_fn = "compair_sketch_cdf.dat";
	string distances_fn = "compair_distances.dat";
	string max_dist_fn = "compair_max_dist.dat";
	fstream out;

	out.open(prefix + dist_fn, fstream::out);

	ASSERT(res.distribution_lhs.size() == res.distribution_rhs.size());
	for(size_t i = 0; i < res.distribution_lhs.size(); i++){
		out << res.distribution_lhs[i] << "\t" << res.distribution_rhs[i] << "\n";
	}

	out.close();

	out.open(prefix + cdf_fn, fstream::out);

	ASSERT(res.cdf_distribution_lhs.size() == res.cdf_distribution_rhs.size());
	for(size_t i = 0; i < res.cdf_distribution_lhs.size(); i++){
		out << res.cdf_distribution_lhs[i] << "\t" << res.cdf_distribution_rhs[i] << "\n";
	}

	out.close();

	out.open(prefix + sketch_cdf_fn, fstream::out);

	ASSERT(res.sketch_cdf_distribution_lhs.size() == res.sketch_cdf_distribution_rhs.size());
	for(size_t i = 0; i < res.sketch_cdf_distribution_lhs.size(); i++){
		out << res.sketch_cdf_distribution_lhs[i] << "\t" << res.sketch_cdf_distribution_rhs[i] << "\n";
	}

	out.close();

	out.open(prefix + distances_fn, fstream::out);

	ASSERT(res.distances_distributions.size() == res.distances_sketches.size());
	for(size_t i = 0; i < res.distances_distributions.size(); i++){
		out << res.distances_distributions[i] << "\t" << res.distances_sketches[i] << "\n";
	}

	out.close();

	out.open(prefix + max_dist_fn, fstream::out);

	out << res.max_distance_distributions << "\t" << res.max_distance_sketches << "\n";
	
	out.close();

	return;
}

void save_test(const string& prefix, const ks_test_result_t& res){
	string dist_fn = "distribution.dat";
	string cdf_fn = "cdf.dat";
	string sketch_cdf_fn = "sketch_cdf.dat";
	string distances_fn = "distances.dat";
	string max_dist_fn = "max_dist.dat";
	fstream out;

	out.open(prefix + dist_fn, fstream::out);

	for(size_t i = 0; i < res.distribution.size(); i++){
		out << res.distribution[i] << "\n";
	}

	out.close();

	out.open(prefix + cdf_fn, fstream::out);

	for(size_t i = 0; i < res.cdf_distribution.size(); i++){
		out << res.cdf_distribution[i] << "\n";
	}

	out.close();

	out.open(prefix + sketch_cdf_fn, fstream::out);

	for(size_t i = 0; i < res.sketch_cdf_distribution.size(); i++){
		out << res.sketch_cdf_distribution[i] << "\n";
	}

	out.close();

	out.open(prefix + distances_fn, fstream::out);

	for(size_t i = 0; i < res.distances.size(); i++){
		out << res.distances[i] << "\n";
	}

	out.close();

	out.open(prefix + max_dist_fn, fstream::out);

	out << res.max_distance << "\n";
	
	out.close();

	return;
}



#if 0
int main(int argc, char* argv[]){
	deque<string> args = process_args(argc, argv);

	if (args.size() != 10){
		help();
		return -1;
	}

	int samples = stoi(args[0]);
	int start_point = stoi(args[1]);
	int end_point = stoi(args[2]);
	int step = stoi(args[3]);
	double min_mean_range = stod(args[4]);
	double max_mean_range = stod(args[5]);
	double min_std_range = stod(args[6]);
	double max_std_range = stod(args[7]);
	double error = stod(args[8]);
	double universe = stoi(args[9]);
	
	ks_compair_test_result_t kll_compair_res;
	ks_compair_test_result_t dcs_compair_res;
	ks_compair_test_result_t q_digest_compair_res;
	ks_compair_test_result_t gk_compair_res;
	ks_compair_test_result_t worst_compair_kll_res;
	ks_compair_test_result_t worst_compair_dcs_res;
	ks_compair_test_result_t worst_compair_q_digest_res;
	ks_compair_test_result_t worst_compair_gk_res;

	ks_test_result_t kll_res;
	ks_test_result_t dcs_res;
	ks_test_result_t q_digest_res;
	ks_test_result_t gk_res;
	ks_test_result_t worst_kll_res;
	ks_test_result_t worst_dcs_res;
	ks_test_result_t worst_q_digest_res;
	ks_test_result_t worst_gk_res;
	double kll_max_dist = numeric_limits<double>::min();
	double kll_max_diff = numeric_limits<double>::min();
	double dcs_max_dist = numeric_limits<double>::min();
	double dcs_max_diff = numeric_limits<double>::min();
	double q_digest_max_dist = numeric_limits<double>::min();
	double q_digest_max_diff = numeric_limits<double>::min();
	double gk_max_dist = numeric_limits<double>::min();
	double gk_max_diff = numeric_limits<double>::min();

	samples_vector all_pairs = all_pair_distributions_from(samples, min_mean_range, max_mean_range, 100.0, min_std_range, max_std_range, 10.0);
	vector<ks_compair_test_result_t> kll_compair_results;
	vector<ks_compair_test_result_t> dcs_compair_results;
	vector<ks_compair_test_result_t> q_digest_compair_results;
	vector<ks_compair_test_result_t> gk_compair_results;

	for(auto& sample_pair : all_pairs){
		kll_compair_res = test_compair_normal_distribution_with_kll(sample_pair.first, sample_pair.second, start_point, end_point, step, error);
		dcs_compair_res = test_compair_normal_distribution_with_dcs(sample_pair.first, sample_pair.second, start_point, end_point, step, error, universe);
		q_digest_compair_res = test_compair_normal_distribution_with_q_digest(sample_pair.first, sample_pair.second, start_point, end_point, step, error, universe);
		gk_compair_res = test_compair_normal_distribution_with_gk(sample_pair.first, sample_pair.second, start_point, end_point, step, error);
		
		kll_compair_results.push_back(kll_compair_res);
		dcs_compair_results.push_back(dcs_compair_res);
		q_digest_compair_results.push_back(q_digest_compair_res);
		gk_compair_results.push_back(gk_compair_res);

		if(fabs(kll_compair_res.max_distance_distributions - kll_compair_res.max_distance_sketches) >= kll_max_diff){
			worst_compair_kll_res = kll_compair_res;
		}

		/*kll_res = test_ks_with_kll(samples, start_point, end_point, step, mean, std, error);
		
		if(kll_res.max_distance >= kll_max_dist){
			worst_kll_res = kll_res;
		}*/

		
		if(fabs(dcs_compair_res.max_distance_distributions - dcs_compair_res.max_distance_sketches) >= dcs_max_diff){
			worst_compair_dcs_res = dcs_compair_res;
		}

		/*dcs_res = test_ks_with_dcs(samples, start_point, end_point, step, mean, std, error, universe);

		if(dcs_res.max_distance >= dcs_max_dist){
			worst_dcs_res = dcs_res;
		}*/
	

		if(fabs(q_digest_compair_res.max_distance_distributions - q_digest_compair_res.max_distance_sketches) >= q_digest_max_diff){
			worst_compair_q_digest_res = q_digest_compair_res;
		}

		/*req_digest_res = test_ks_with_q_digest(samples, start_point, end_point, step, mean, std, error, universe);

		if(q_digest_res.max_distance >= q_digest_max_dist){
			worst_q_digest_res = q_digest_res;
		}*/

		
		if(fabs(gk_compair_res.max_distance_distributions - gk_compair_res.max_distance_sketches) >= gk_max_diff){
			worst_compair_gk_res = gk_compair_res;
		}

		/*gk_res = test_ks_with_gk(samples, start_point, end_point, step, mean, std, error);

		if(gk_res.max_distance >= gk_max_dist){
			worst_gk_res = gk_res;
		}*/
	}

	cout << "KLL : " << worst_compair_kll_res << endl;
	
	save_csv_compair(string("./kll_") + to_string(error) + string("_"), kll_compair_results);
	//save_compair_test(string("./kll_") + to_string(error) + string("_"), worst_compair_kll_res);
	//save_test(string("./kll_") + to_string(error) + string("_"), worst_kll_res);
	
	cout << "DCS : " << worst_dcs_res << endl;
	
	save_csv_compair(string("./dcs_") + to_string(error) + string("_"), dcs_compair_results);
	//save_compair_test(string("./dcs_") + to_string(error) + string("_"), worst_compair_dcs_res);
	//save_test(string("./dcs_") + to_string(error) + string("_"), worst_dcs_res);
	
	cout << "Q_DIGEST : " << worst_compair_q_digest_res << endl;
	
	save_csv_compair(string("./q_digest_") + to_string(error) + string("_"), q_digest_compair_results);
	//save_compair_test(string("./q_digest_") + to_string(error) + string("_"), worst_compair_q_digest_res);
	//save_test(string("./q_digest_") + to_string(error) + string("_"), worst_q_digest_res);
	
	cout << "GK : " << worst_compair_gk_res << endl;
	
	save_csv_compair(string("./gk_") + to_string(error) + string("_"), gk_compair_results);
	//save_compair_test(string("./gk_") + to_string(error) + string("_"), worst_compair_gk_res);
	//save_test(string("./gk_") + to_string(error) + string("_"), worst_gk_res);

	return 0;
}

#endif


#if 1

int main(void){
    vector<int> samples = {5, 10, 12, 20, 43};

    for(int i = 0; i < 100; i++){
		int r1 = rs2(samples, i);
		int r2 = rank_from_samples(samples, i);

		VDEBUG(r1);
		VDEBUG(r2);

        ASSERT(r1 == r2);
    }

	return 0;
}

#endif