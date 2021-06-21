#include <quantile_quadtree/quantile_quadtree.hpp>
#include <gk_factory/gk_factory.hpp>
#include <kll_factory/kll_factory.hpp>
#include <dcs_factory/dcs_factory.h>
#include <q_digest_factory/q_digest_factory.h>
#include <qsbd_debug/qsbd_debug.h>
#include <utils/utils.h>
#include <stream_maker/stream_maker.h>
#include <fstream>
#include <iostream>
using namespace std;

class dummy_sketch : public qsbd::quantile_sketch<int> {
public:
    dummy_sketch(){
    }

    ~dummy_sketch(){
    }

    void update(int x, int weight) override {
        return;
    }

    int query(int x) override {
        return 0;
    }

    void inner_merge(qsbd::quantile_sketch<int>& rhs) override{
        return;
    }

    qsbd::quantile_sketch<int>* merge(qsbd::quantile_sketch<int>& rhs) override {
        return nullptr;
    }
};

class dummy_factory : public qsbd::sketch_factory<int> {
    int count;
public:
    dummy_factory():count(0){
    }

    qsbd::quantile_sketch<int>* instance() override {
        this->count++;
        return new dummy_sketch();
    }

    int allocations(){
        return this->count;
    }
};

void empty_run(bool only_update){
      // STREAM
    int vector_size = 100000000; 
    int min_v = 0;
    int max_v = 1000;
    double minx = 0.0;
    double miny = 0.0;
    double maxx = 1280.0;
    double maxy = 720.0;
    int min_w = 0;
    int max_w = 50;

    // TREE
    double error = 0.3;
    int universe = 1024;
    int depth = 10;
    double bounds[4] = {0.0, 0.0, 1280.0, 720.0};
    int discrete_bounds[4] = {0, 0, 0, 0};

    // QUERIES
    int initial_value = 0;
    int final_value = 1000;
    int step = 1;

    vector<pair<pair<int, int>, pair<double, double>>> stream = qsbd::stream_maker::random_stream_in_region_with_weight(vector_size, minx, miny, maxx, maxy, min_v, max_v, min_w, max_w);
    vector<pair<int, vector<double>>> regions_to_search;
    
    for(int i = 0; i < 4; i++){
        // 0 -> 0 2
        // 1 -> 1 3
        // 2 -> 0 2 
        // 3 -> 1 3
        if(i & 1){
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[1], bounds[3], depth);
        }else{
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[0], bounds[2], depth);
        }
    }

    qsbd::aabb<int> resolution(discrete_bounds[0], discrete_bounds[1], discrete_bounds[2], discrete_bounds[3]);
    
    for(int i = initial_value; i < final_value; i += step){
        auto query_bound = qsbd::stream_maker::random_rectangle_in_region(bounds[0], bounds[1], bounds[2], bounds[3]);
        vector<double> rect;
        pair<int, vector<double>> rank_query;

        rect.push_back(query_bound.first.first);
        rect.push_back(query_bound.first.second);
        rect.push_back(query_bound.second.first);
        rect.push_back(query_bound.second.second);

        rank_query = make_pair(i, rect);
        regions_to_search.push_back(rank_query);
    }

    cout << "Update := ";
    cout.flush();

    string progress = "0%";
    cout << progress;
    cout.flush();

    dummy_factory factory;
    qsbd::quantile_quadtree<int> qq_test(resolution, depth, &factory);

    for(int i = 0; i < stream.size(); i++){
        qsbd::point<int> coord(qsbd::map_coord(stream[i].second.first, bounds[0], bounds[2], depth), qsbd::map_coord(stream[i].second.second, bounds[1], bounds[3], depth));
        qq_test.update(coord, stream[i].first.first, stream[i].first.second);
        int progress_pct = (int) ((i * 100) / stream.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }

    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;

    if (only_update) return;

    cout << "Queries := ";
    cout.flush();

    progress = "0%";
    cout << progress;
    cout.flush();

    for(int j = 0; j < regions_to_search.size(); j++){
        int region_discrete_bound[4] = {0, 0, 0, 0};

        for(int z = 0; z < 4; z++){
            if(z & 1){
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[1], bounds[3], depth);
            }else{
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[0], bounds[2], depth);
            }
        }

        qsbd::aabb<int> region(region_discrete_bound[0], region_discrete_bound[1], region_discrete_bound[2], region_discrete_bound[3]);

        int rank = qq_test.query(region, regions_to_search[j].first);

        int progress_pct = (int) ((j * 100) / regions_to_search.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }

    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;
}

void gk_run(bool only_update){
    // STREAM
    int vector_size = 1000; 
    int min_v = 0;
    int max_v = 1000;
    double minx = 0.0;
    double miny = 0.0;
    double maxx = 1280.0;
    double maxy = 720.0;
    int citys = 100;
    double max_radius = 5.0;

    // TREE
    double error = 0.3;
    int depth = 6;
    double bounds[4] = {0.0, 0.0, 1280.0, 720.0};
    int discrete_bounds[4] = {0, 0, 0, 0};

    // QUERIES
    int initial_value = 0;
    int final_value = 1000;
    int step = 1;

    vector<pair<int, pair<double, double>>> stream = qsbd::stream_maker::random_stream_city(vector_size, minx, miny, maxx, maxy, min_v, max_v, citys, max_radius);
    vector<pair<int, vector<double>>> regions_to_search;
    
    for(int i = 0; i < 4; i++){
        // 0 -> 0 2
        // 1 -> 1 3
        // 2 -> 0 2 
        // 3 -> 1 3
        if(i & 1){
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[1], bounds[3], depth);
        }else{
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[0], bounds[2], depth);
        }
    }

    qsbd::aabb<int> resolution(discrete_bounds[0], discrete_bounds[1], discrete_bounds[2], discrete_bounds[3]);
    
     for(int i = initial_value; i < final_value; i += step){
        auto query_bound = qsbd::stream_maker::random_rectangle_in_region(bounds[0], bounds[1], bounds[2], bounds[3]);
        vector<double> rect;
        pair<int, vector<double>> rank_query;

        rect.push_back(query_bound.first.first);
        rect.push_back(query_bound.first.second);
        rect.push_back(query_bound.second.first);
        rect.push_back(query_bound.second.second);

        rank_query = make_pair(i, rect);
        regions_to_search.push_back(rank_query);
    }

    cout << "Update := ";
    cout.flush();

    string progress = "0%";
    cout << progress;
    cout.flush();

    qsbd::gk_factory<int> factory(error);
    qsbd::quantile_quadtree<int> qq_test(resolution, depth, &factory);

    for(int i = 0; i < stream.size(); i++){
        qsbd::point<int> coord(qsbd::map_coord(stream[i].second.first, bounds[0], bounds[2], depth), qsbd::map_coord(stream[i].second.second, bounds[1], bounds[3], depth));
        qq_test.update(coord, stream[i].first);

        int progress_pct = (int) ((i * 100) / stream.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }

    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;

    if (only_update) return;

    cout << "Queries := ";
    cout.flush();

    progress = "0%";
    cout << progress;
    cout.flush();

    for(int j = 0; j < regions_to_search.size(); j++){
        int region_discrete_bound[4] = {0, 0, 0, 0};

        for(int z = 0; z < 4; z++){
            if(z & 1){
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[1], bounds[3], depth);
            }else{
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[0], bounds[2], depth);
            }
        }

        qsbd::aabb<int> region(region_discrete_bound[0], region_discrete_bound[1], region_discrete_bound[2], region_discrete_bound[3]);

        int rank = qq_test.query(region, regions_to_search[j].first);

        int progress_pct = (int) ((j * 100) / regions_to_search.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }

    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;
}

void q_digest_run(bool only_update){
    // STREAM
    int vector_size = 100000; 
    int min_v = 0;
    int max_v = 1000;
    double minx = 0.0;
    double miny = 0.0;
    double maxx = 1280.0;
    double maxy = 720.0;
    int min_w = 0;
    int max_w = 70;

    // TREE
    double error = 0.3;
    int universe = 1024;
    int depth = 10;
    double bounds[4] = {0.0, 0.0, 1280.0, 720.0};
    int discrete_bounds[4] = {0, 0, 0, 0};

    // QUERIES
    int initial_value = 0;
    int final_value = 1000;
    int step = 1;

    vector<pair<pair<int, int>, pair<double, double>>> stream = qsbd::stream_maker::random_stream_in_region_with_weight(vector_size, minx, miny, maxx, maxy, min_v, max_v, min_w, max_w);
    vector<pair<int, vector<double>>> regions_to_search;
    
    
    for(int i = 0; i < 4; i++){
        // 0 -> 0 2
        // 1 -> 1 3
        // 2 -> 0 2 
        // 3 -> 1 3
        if(i & 1){
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[1], bounds[3], depth);
        }else{
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[0], bounds[2], depth);
        }
    }

    qsbd::aabb<int> resolution(discrete_bounds[0], discrete_bounds[1], discrete_bounds[2], discrete_bounds[3]);
    
    for(int i = initial_value; i < final_value; i += step){
        auto query_bound = qsbd::stream_maker::random_rectangle_in_region(bounds[0], bounds[1], bounds[2], bounds[3]);
        vector<double> rect;
        pair<int, vector<double>> rank_query;

        rect.push_back(query_bound.first.first);
        rect.push_back(query_bound.first.second);
        rect.push_back(query_bound.second.first);
        rect.push_back(query_bound.second.second);

        rank_query = make_pair(i, rect);
        regions_to_search.push_back(rank_query);
    }

    cout << "Update := ";
    cout.flush();

    string progress = "0%";
    cout << progress;
    cout.flush();

    qsbd::q_digest_factory factory(error, universe);
    qsbd::quantile_quadtree<int> qq_test(resolution, depth, &factory);


    for(int i = 0; i < stream.size(); i++){
        qsbd::point<int> coord(qsbd::map_coord(stream[i].second.first, bounds[0], bounds[2], depth), qsbd::map_coord(stream[i].second.second, bounds[1], bounds[3], depth));
        qq_test.update(coord, stream[i].first.first, stream[i].first.second);

        int progress_pct = (int) ((i * 100) / stream.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }

    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;

    if (only_update) return;

    cout << "Queries := ";
    cout.flush();

    progress = "0%";
    cout << progress;
    cout.flush();

    for(int j = 0; j < regions_to_search.size(); j++){
        int region_discrete_bound[4] = {0, 0, 0, 0};

        for(int z = 0; z < 4; z++){
            if(z & 1){
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[1], bounds[3], depth);
            }else{
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[0], bounds[2], depth);
            }
        }

        qsbd::aabb<int> region(region_discrete_bound[0], region_discrete_bound[1], region_discrete_bound[2], region_discrete_bound[3]);

        int rank = qq_test.query(region, regions_to_search[j].first);

        int progress_pct = (int) ((j * 100) / regions_to_search.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }

    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;
}

void dcs_run(bool only_update){
    // STREAM
    int vector_size = 100000; 
    int min_v = 0;
    int max_v = 1000;
    double minx = 0.0;
    double miny = 0.0;
    double maxx = 1280.0;
    double maxy = 720.0;
    int min_w = 0;
    int max_w = 50;

    // TREE
    double error = 0.3;
    int universe = 1024;
    int depth = 10;
    double bounds[4] = {0.0, 0.0, 1280.0, 720.0};
    int discrete_bounds[4] = {0, 0, 0, 0};

    // QUERIES
    int initial_value = 0;
    int final_value = 1000;
    int step = 1;

    qsbd::dcs t(error, universe);

    cout << t.get_d() << endl;
    cout << t.get_w() << endl;
    cout << t.get_s() << endl;

    vector<pair<pair<int, int>, pair<double, double>>> stream = qsbd::stream_maker::random_stream_in_region_with_weight(vector_size, minx, miny, maxx, maxy, min_v, max_v, min_w, max_w);
    vector<pair<int, vector<double>>> regions_to_search;
    
    for(int i = 0; i < 4; i++){
        // 0 -> 0 2
        // 1 -> 1 3
        // 2 -> 0 2 
        // 3 -> 1 3
        if(i & 1){
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[1], bounds[3], depth);
        }else{
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[0], bounds[2], depth);
        }
    }

    qsbd::aabb<int> resolution(discrete_bounds[0], discrete_bounds[1], discrete_bounds[2], discrete_bounds[3]);
    
    for(int i = initial_value; i < final_value; i += step){
        auto query_bound = qsbd::stream_maker::random_rectangle_in_region(bounds[0], bounds[1], bounds[2], bounds[3]);
        vector<double> rect;
        pair<int, vector<double>> rank_query;

        rect.push_back(query_bound.first.first);
        rect.push_back(query_bound.first.second);
        rect.push_back(query_bound.second.first);
        rect.push_back(query_bound.second.second);

        rank_query = make_pair(i, rect);
        regions_to_search.push_back(rank_query);
    }

    cout << "Update := ";
    cout.flush();

    string progress = "0%";
    cout << progress;
    cout.flush();

    qsbd::dcs_factory factory(error, universe);
    qsbd::quantile_quadtree<int> qq_test(resolution, depth, &factory);

    for(int i = 0; i < stream.size(); i++){
        qsbd::point<int> coord(qsbd::map_coord(stream[i].second.first, bounds[0], bounds[2], depth), qsbd::map_coord(stream[i].second.second, bounds[1], bounds[3], depth));
        qq_test.update(coord, stream[i].first.first, stream[i].first.second);
        int progress_pct = (int) ((i * 100) / stream.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }

    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;

    if (only_update) return;

    cout << "Queries := ";
    cout.flush();

    progress = "0%";
    cout << progress;
    cout.flush();

    for(int j = 0; j < regions_to_search.size(); j++){
        int region_discrete_bound[4] = {0, 0, 0, 0};

        for(int z = 0; z < 4; z++){
            if(z & 1){
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[1], bounds[3], depth);
            }else{
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[0], bounds[2], depth);
            }
        }

        qsbd::aabb<int> region(region_discrete_bound[0], region_discrete_bound[1], region_discrete_bound[2], region_discrete_bound[3]);

        int rank = qq_test.query(region, regions_to_search[j].first);

        int progress_pct = (int) ((j * 100) / regions_to_search.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }

    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;
}

void dcs_run2(bool only_update){
    // STREAM
    //int min_v = 0;
    //int max_v = 1000;
    //double minx = 0.0;
    //double miny = 0.0;
    //double maxx = 1280.0;
    //double maxy = 720.0;

    // TREE
    double error = 0.3;
    int universe = 1024;
    int depth = 10;
    //double bounds[4] = {0.0, 0.0, 1280.0, 720.0};
    //int discrete_bounds[4] = {0, 0, 0, 0};

    // QUERIES
    //int initial_value = 0;
    //int final_value = 1000;
    //int step = 1;

    //vector<pair<pair<int, int>, pair<int, int>>> stream;
    //vector<pair<int, vector<double>>> regions_to_search;

    /*for(int i = 0; i < 1024; i++){
        for(int j = 0; j < 1024; j++){
            uniform_int_distribution<int> v(min_v, max_v);

            stream.push_back({{v(qsbd::generator), 1}, {i, j}});
        }
    }*/
    
    /*for(int i = 0; i < 4; i++){
        // 0 -> 0 2
        // 1 -> 1 3
        // 2 -> 0 2 
        // 3 -> 1 3
        if(i & 1){
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[1], bounds[3], depth);
        }else{
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[0], bounds[2], depth);
        }
    }*/

    qsbd::aabb<int> resolution(0, 0, 1 << depth, 1 << depth);
    
    /*(for(int i = initial_value; i < final_value; i += step){
        auto query_bound = qsbd::stream_maker::random_rectangle_in_region(bounds[0], bounds[1], bounds[2], bounds[3]);
        vector<double> rect;
        pair<int, vector<double>> rank_query;

        rect.push_back(query_bound.first.first);
        rect.push_back(query_bound.first.second);
        rect.push_back(query_bound.second.first);
        rect.push_back(query_bound.second.second);

        rank_query = make_pair(i, rect);
        regions_to_search.push_back(rank_query);
    }*/

    //cout << "Update := ";
    //cout.flush();

    //string progress = "0%";
    //cout << progress;
    //cout.flush();

    qsbd::dcs_factory factory(error, universe);
	//dummy_factory factory;
   	qsbd::quantile_quadtree<int> qq_test(resolution, depth, &factory, true);

   	for(int k = 0; k < 1; k++){
 		for(int i = 0; i < (1 << depth); i++){
			for(int j = 0; j < (1 << depth); j++){
				uniform_int_distribution<int> v(0, universe - 1);		
        		qsbd::point<int> coord(i, j);
        		qq_test.update(coord, v(qsbd::generator), 1);
        		//int progress_pct = (int) (((i * 1024 + j) * 100) / (1024 * 1024));
        
        		//cout << string(progress.size(), '\b');
        		//cout.flush();
        
        		//progress = to_string(progress_pct) + "%";
        		//cout << progress;
        		//cout.flush();
			}
    	}
	}

    VDEBUG(qq_test.query(resolution, universe - 1));

	/*VDEBUG(factory.allocations());
    qsbd::dcs* test = new qsbd::dcs(error, universe);
    qsbd::k_wise_family * f2 = new qsbd::k_wise_family(2, universe);
    qsbd::count_sketch * cs = new qsbd::count_sketch(4, 19);
    VDEBUG(sizeof(*f2));
    VDEBUG(2 * sizeof(int));
    VDEBUG(sizeof(*cs));
    VDEBUG(2 * sizeof(double) + sizeof(qsbd::k_wise_family**) + sizeof(int**) + 2 * sizeof(int));
    VDEBUG(4 * (sizeof(*f2) + 2 * sizeof(int)));
    VDEBUG(4 * 19 * sizeof(int));
    VDEBUG(sizeof(*test));
    VDEBUG(sizeof(double) + 6 * sizeof(int) + sizeof(vector<vector<int>>) + sizeof(vector<qsbd::count_sketch>));
    VDEBUG((test->get_s() + 1) * (sizeof(*cs) + 4 * 19 * sizeof(int) + 4 * (sizeof(*f2) + 2 * sizeof(int))));
    VDEBUG((test->get_tree_lvl() - (test->get_s() + 1)) * (sizeof(vector<int>) + 126 * sizeof(int)));
    VDEBUG(sizeof(dummy_sketch));
    VDEBUG(test->get_tree_lvl());
    VDEBUG(test->get_w());
    VDEBUG(test->get_d());
    VDEBUG(test->get_s());

    delete test;
    delete f2;
    delete cs;*/

    //cout << string(progress.size(), '\b');
    //cout.flush();
    
    //cout << "Done!" << endl;

    if (only_update) return;

    /*cout << "Queries := ";
    cout.flush();

    progress = "0%";
    cout << progress;
    cout.flush();

    for(int j = 0; j < regions_to_search.size(); j++){
        int region_discrete_bound[4] = {0, 0, 0, 0};

        for(int z = 0; z < 4; z++){
            if(z & 1){
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[1], bounds[3], depth);
            }else{
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[0], bounds[2], depth);
            }
        }

        qsbd::aabb<int> region(region_discrete_bound[0], region_discrete_bound[1], region_discrete_bound[2], region_discrete_bound[3]);

        int rank = qq_test.query(region, regions_to_search[j].first);

        int progress_pct = (int) ((j * 100) / regions_to_search.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }

    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;*/
}

void kll_run(bool only_update){
    // STREAM
    int vector_size = 100000; 
    int min_v = 0;
    int max_v = 1000;
    double minx = 0.0;
    double miny = 0.0;
    double maxx = 1280.0;
    double maxy = 720.0;
    int citys = 100;
    double max_radius = 5.0;

    // TREE
    double error = 0.3;
    int depth = 10;
    double bounds[4] = {0.0, 0.0, 1280.0, 720.0};
    int discrete_bounds[4] = {0, 0, 0, 0};

    // QUERIES
    int initial_value = 0;
    int final_value = 1000;
	int step = 1;
     
	vector<pair<int, pair<double, double>>> stream = qsbd::stream_maker::random_stream_city(vector_size, minx, miny, maxx, maxy, min_v, max_v, citys, max_radius);
    vector<pair<int, vector<double>>> regions_to_search;
    
    for(int i = 0; i < 4; i++){
        // 0 -> 0 2
        // 1 -> 1 3
        // 2 -> 0 2 
        // 3 -> 1 3
        if(i & 1){
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[1], bounds[3], depth);
        }else{
            discrete_bounds[i] = qsbd::map_coord(bounds[i], bounds[0], bounds[2], depth);
        }
    }

    qsbd::aabb<int> resolution(discrete_bounds[0], discrete_bounds[1], discrete_bounds[2], discrete_bounds[3]);

    for(int i = initial_value; i < final_value; i += step){
        auto query_bound = qsbd::stream_maker::random_rectangle_in_region(bounds[0], bounds[1], bounds[2], bounds[3]);
        vector<double> rect;
        pair<int, vector<double>> rank_query;

        rect.push_back(query_bound.first.first);
        rect.push_back(query_bound.first.second);
        rect.push_back(query_bound.second.first);
        rect.push_back(query_bound.second.second);

        rank_query = make_pair(i, rect);
        regions_to_search.push_back(rank_query);
    }

    cout << "Update := ";
    cout.flush();

    string progress = "0%";
    cout << progress;
    cout.flush();
   
    qsbd::kll_factory<int> factory(error);
    qsbd::quantile_quadtree<int> qq_test(resolution, depth, &factory);

    for(int i = 0; i < stream.size(); i++){
        qsbd::point<int> coord(qsbd::map_coord(stream[i].second.first, bounds[0], bounds[2], depth), qsbd::map_coord(stream[i].second.second, bounds[1], bounds[3], depth));
        qq_test.update(coord, stream[i].first);
        int progress_pct = (int) ((i * 100) / stream.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }
    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;

    if (only_update) return;

    cout << "Queries := ";
    cout.flush();

    progress = "0%";
    cout << progress;
    cout.flush();

    for(int j = 0; j < regions_to_search.size(); j++){
        int region_discrete_bound[4] = {0, 0, 0, 0};

        for(int z = 0; z < 4; z++){
            if(z & 1){
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[1], bounds[3], depth);
            }else{
                region_discrete_bound[z] = qsbd::map_coord(regions_to_search[j].second[z], bounds[0], bounds[2], depth);
            }
        }

        qsbd::aabb<int> region(region_discrete_bound[0], region_discrete_bound[1], region_discrete_bound[2], region_discrete_bound[3]);

        int rank = qq_test.query(region, regions_to_search[j].first);

        int progress_pct = (int) ((j * 100) / regions_to_search.size());
        
        cout << string(progress.size(), '\b');
        cout.flush();
        
        progress = to_string(progress_pct) + "%";
        cout << progress;
        cout.flush();
    }
    

    cout << string(progress.size(), '\b');
    cout.flush();
    
    cout << "Done!" << endl;
}

int main(int argc, char* argv[]){
    dcs_run2(true);
	/*deque<string> args = qsbd::process_args(argc, argv);

    bool only_update = false;

    if (not (args.size() == 1 or args.size() == 2 or args.size() == 3)){
        DEBUG_ERR("You need pass only the test name. [ kll / q_digest / dcs / gk] [seed (optional)] [-u (optional)]");
        return -1;
    }

    if(args.size() > 1) {
        if(args[1] == "-u"){
            only_update = true;
        }else{
            qsbd::generator.seed(stoul(args[1]));
        }
    }

    if(args.size() == 3){
        if(args[2] == "-u"){
            only_update = true;
        }else{
            DEBUG_ERR("Invalid option on third argument.");
        }
    }

    if(args[0] == "kll") kll_run(only_update);
    else if(args[0] == "gk") gk_run(only_update);
    else if(args[0] == "q_digest") q_digest_run(only_update);
    else if(args[0] == "dcs") dcs_run2(only_update);
    else if(args[0] == "empty") empty_run(only_update);
    else{
        DEBUG_ERR("Invalid test arg.");
        return -1;
    }

    if(args.size() == 1 or (args.size() == 2 and only_update)){
        cout << "Seed used : " << qsbd::seed << endl;
    }*/
    return 0;
}

