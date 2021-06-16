#include <nlohmann/json.hpp>
#include <memory_tracker/memory_tracker.h>
#include <quantile_quadtree/quantile_quadtree.hpp>
#include <gk_factory/gk_factory.hpp>
#include <kll_factory/kll_factory.hpp>
#include <dcs_factory/dcs_factory.h>
#include <q_digest_factory/q_digest_factory.h>
#include <qsbd_debug/qsbd_debug.h>
#include <logger/logger.h>
#include <utils/utils.h>
#include <timer/timer.h>
#include <fstream>
#include <iostream>
using namespace std;
using json = nlohmann::json;

/*
* TODO : cdf queries and quantile queries
*
*/

json q_digest_test(const vector<pair<pair<int, int>, pair<double, double>>>& stream, const vector<pair<int, vector<double>>>& regions_to_search, double error, int universe, int* discrete_bounds, double* bounds, int depth){
    qsbd::aabb<int> bound_box(discrete_bounds[0], discrete_bounds[1], discrete_bounds[2], discrete_bounds[3]);
    
    json out_info = json::object();
    out_info["q_digest_test"] = json::object();

    cout << "Running...";
    cout.flush();

    qsbd::q_digest_factory factory(error, universe);
    qsbd::quantile_quadtree<int> qq_test(bound_box, depth, &factory);
    json loop_info;
    double update_time_acc = 0.0;

    for(int i = 0, j = 10; i < stream.size(); i++){
        qsbd::timer update_once;
        qsbd::point<int> coord(qsbd::map_coord(stream[i].second.first, bounds[0], bounds[2], depth), qsbd::map_coord(stream[i].second.second, bounds[1], bounds[3], depth));

        update_once.start();
        qq_test.update(coord, stream[i].first.first, stream[i].first.second);
        update_once.end();

        update_time_acc += update_once.count();
        if ((i > 100000) and (i == (j - 1) or i == ((j - 1) / 2))){
            double avg_update_time = update_time_acc / (i + 1);
            loop_info["time"][string("update_time_acc_") + to_string(i + 1)] = update_time_acc;
            loop_info["time"][string("avg_update_time_") + to_string(i + 1)] = avg_update_time;

            double query_time_acc = 0.0;
            loop_info["time"][string("queries_") + to_string(i + 1)] = json::array();

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
                qsbd::timer query_once;
                json region_info;

                query_once.start();
                int rank = qq_test.query(region, regions_to_search[j].first);
                query_once.end();

                query_time_acc += query_once.count();
                
                region_info["region"] = regions_to_search[j].second;
                region_info["value"] = regions_to_search[j].first;
                region_info["rank"] = rank;
                region_info["query_time"] = query_once.count();

                loop_info["time"][string("queries_") + to_string(i + 1)].push_back(region_info);
            }

            double avg_query_time = query_time_acc / regions_to_search.size();

            loop_info["time"][string("avg_query_time_") + to_string(i + 1)] = avg_query_time;
            loop_info[string("memory_") + to_string(i + 1)] = {};

            qsbd::mem_track::track_list_memory_usage(loop_info[string("memory_") + to_string(i + 1)]);

        }
        
        if(i == (j - 1)){
            j *= 10;
        }
    }

    out_info["q_digest_test"] = loop_info;

    cout << "Done!" << endl;

    return out_info;
}

json kll_test(const vector<pair<int, pair<double, double>>>& stream, const vector<pair<int, vector<double>>>& regions_to_search, double error, int* discrete_bounds, double* bounds, int depth){
    qsbd::aabb<int> bound_box(discrete_bounds[0], discrete_bounds[1], discrete_bounds[2], discrete_bounds[3]);
    
    json out_info = json::object();
    out_info["kll_test"] = json::object();

    cout << "Running...";
    cout.flush();

    qsbd::kll_factory<int> factory(error);
    qsbd::quantile_quadtree<int> qq_test(bound_box, depth, &factory);
    json loop_info;
    double update_time_acc = 0.0;

    for(int i = 0, j = 10; i < stream.size(); i++){
        qsbd::timer update_once;
        qsbd::point<int> coord(qsbd::map_coord(stream[i].second.first, bounds[0], bounds[2], depth), qsbd::map_coord(stream[i].second.second, bounds[1], bounds[3], depth));

        update_once.start();
        qq_test.update(coord, stream[i].first);
        update_once.end();

        update_time_acc += update_once.count();
        if ((i > 100000) and (i == (j - 1) or i == ((j - 1) / 2))){
            double avg_update_time = update_time_acc / (i + 1);
            loop_info["time"][string("update_time_acc_") + to_string(i + 1)] = update_time_acc;
            loop_info["time"][string("avg_update_time_") + to_string(i + 1)] = avg_update_time;

            double query_time_acc = 0.0;
            loop_info["time"][string("queries_") + to_string(i + 1)] = json::array();

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
                qsbd::timer query_once;
                json region_info;

                query_once.start();
                int rank = qq_test.query(region, regions_to_search[j].first);
                query_once.end();

                query_time_acc += query_once.count();
                
                region_info["region"] = regions_to_search[j].second;
                region_info["value"] = regions_to_search[j].first;
                region_info["rank"] = rank;
                region_info["query_time"] = query_once.count();

                loop_info["time"][string("queries_") + to_string(i + 1)].push_back(region_info);
            }

            double avg_query_time = query_time_acc / regions_to_search.size();

            loop_info["time"][string("avg_query_time_") + to_string(i + 1)] = avg_query_time;
            loop_info[string("memory_") + to_string(i + 1)] = {};

            qsbd::mem_track::track_list_memory_usage(loop_info[string("memory_") + to_string(i + 1)]);

        }
        
        if(i == (j - 1)){
            j *= 10;
        }
    }

    out_info["kll_test"] = loop_info;

    cout << "Done!" << endl;

    return out_info;
}

json dcs_test(const vector<pair<pair<int, int>, pair<double, double>>>& stream, const vector<pair<int, vector<double>>>& regions_to_search, double error, int universe, int* discrete_bounds, double* bounds, int depth){
    qsbd::aabb<int> bound_box(discrete_bounds[0], discrete_bounds[1], discrete_bounds[2], discrete_bounds[3]);
    
    json out_info = json::object();
    out_info["dcs_test"] = json::object();

    cout << "Running...";
    cout.flush();

    qsbd::dcs_factory factory(error, universe);
    qsbd::quantile_quadtree<int> qq_test(bound_box, depth, &factory);
    json loop_info;
    double update_time_acc = 0.0;

    for(int i = 0, j = 10; i < stream.size(); i++){
        qsbd::timer update_once;
        qsbd::point<int> coord(qsbd::map_coord(stream[i].second.first, bounds[0], bounds[2], depth), qsbd::map_coord(stream[i].second.second, bounds[1], bounds[3], depth));

        update_once.start();
        qq_test.update(coord, stream[i].first.first, stream[i].first.second);
        update_once.end();

        update_time_acc += update_once.count();
        if ((i > 100000) and (i == (j - 1) or i == ((j - 1) / 2))){
            double avg_update_time = update_time_acc / (i + 1);
            loop_info["time"][string("update_time_acc_") + to_string(i + 1)] = update_time_acc;
            loop_info["time"][string("avg_update_time_") + to_string(i + 1)] = avg_update_time;

            double query_time_acc = 0.0;
            loop_info["time"][string("queries_") + to_string(i + 1)] = json::array();

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
                qsbd::timer query_once;
                json region_info;

                query_once.start();
                int rank = qq_test.query(region, regions_to_search[j].first);
                query_once.end();

                query_time_acc += query_once.count();
                
                region_info["region"] = regions_to_search[j].second;
                region_info["value"] = regions_to_search[j].first;
                region_info["rank"] = rank;
                region_info["query_time"] = query_once.count();

                loop_info["time"][string("queries_") + to_string(i + 1)].push_back(region_info);
            }

            double avg_query_time = query_time_acc / regions_to_search.size();

            loop_info["time"][string("avg_query_time_") + to_string(i + 1)] = avg_query_time;
            loop_info[string("memory_") + to_string(i + 1)] = {};

            qsbd::mem_track::track_list_memory_usage(loop_info[string("memory_") + to_string(i + 1)]);

        }
        
        if(i == (j - 1)){
            j *= 10;
        }
    }

    out_info["dcs_test"] = loop_info;

    cout << "Done!" << endl;
    
    return out_info;
}

json gk_test(const vector<pair<int, pair<double, double>>>& stream, const vector<pair<int, vector<double>>>& regions_to_search, double error, int* discrete_bounds, double* bounds, int depth){
    qsbd::aabb<int> bound_box(discrete_bounds[0], discrete_bounds[1], discrete_bounds[2], discrete_bounds[3]);
    
    json out_info = json::object();
    out_info["gk_test"] = json::object();

    cout << "Running...";
    cout.flush();

    qsbd::gk_factory<int> factory(error);
    qsbd::quantile_quadtree<int> qq_test(bound_box, depth, &factory);
    json loop_info;
    double update_time_acc = 0.0;

    for(int i = 0, j = 10; i < stream.size(); i++){
        qsbd::timer update_once;
        qsbd::point<int> coord(qsbd::map_coord(stream[i].second.first, bounds[0], bounds[2], depth), qsbd::map_coord(stream[i].second.second, bounds[1], bounds[3], depth));

        update_once.start();
        qq_test.update(coord, stream[i].first);
        update_once.end();

        update_time_acc += (double) update_once.count();

        if ((i > 100000) and (i == (j - 1) or i == ((j - 1) / 2))){
            double avg_update_time = update_time_acc / (i + 1);
            loop_info["time"][string("update_time_acc_") + to_string(i + 1)] = update_time_acc;
            loop_info["time"][string("avg_update_time_") + to_string(i + 1)] = avg_update_time;

            double query_time_acc = 0.0;
            loop_info["time"][string("queries_") + to_string(i + 1)] = json::array();

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
                qsbd::timer query_once;
                json region_info;

                query_once.start();
                int rank = qq_test.query(region, regions_to_search[j].first);
                query_once.end();

                query_time_acc += query_once.count();
                
                region_info["region"] = regions_to_search[j].second;
                region_info["value"] = regions_to_search[j].first;
                region_info["rank"] = rank;
                region_info["query_time"] = query_once.count();

                loop_info["time"][string("queries_") + to_string(i + 1)].push_back(region_info);
            }

            double avg_query_time = query_time_acc / regions_to_search.size();

            loop_info["time"][string("avg_query_time_") + to_string(i + 1)] = avg_query_time;
            loop_info[string("memory_") + to_string(i + 1)] = {};

            qsbd::mem_track::track_list_memory_usage(loop_info[string("memory_") + to_string(i + 1)]);

        }
        
        if(i == (j - 1)){
            j *= 10;
        }
    }

    out_info["gk_test"] = loop_info;

    cout << "Done!" << endl;

    return out_info;
}

int main(int argc, char * argv[]){
    deque<string> args = qsbd::process_args(argc, argv);
	vector<pair<int, vector<double>>> regions_to_search;
	vector<pair<pair<int, int>, pair<double, double>>> stream_ww;
	vector<pair<int, pair<double, double>>> stream;
    json out_info;
	string sketch_to_test;
	int depth;
	int universe;
	double error;
	double bounds[4] = {0.0, 0.0, 0.0, 0.0};
	int discrete_bounds[4] = {0, 0, 0, 0};
	

    if(args.size() != 4){
        DEBUG_ERR("You need to pass 3 json files and 1 output file. ex: [stream] [querys] [config] [output]");
        return -1;
    }

    for(int i = 0; i < 4; i++){
        if(not qsbd::ends_with(args[i], ".json")){
            DEBUG_ERR("All files passed need to be a json");
        }
    }

    cout << "Parsing Files...";
    cout.flush();

	{
		ifstream stream_file(args[0]);
		if(not stream_file.is_open()){
			DEBUG_ERR("Coulnd't open stream file");		

			return -1;
		}

		ifstream query_file(args[1]);
		if(not query_file.is_open()){
			stream_file.close();
			DEBUG_ERR("Couldn't open query file");
		
			return -1;
		}

		ifstream test_file(args[2]);
		if(not test_file.is_open()){
			stream_file.close();
			query_file.close();
			DEBUG_ERR("Couldn't	open test file");

			return -1;
		}

    	json stream_json = json::parse(stream_file);
    	json query_json = json::parse(query_file);
    	json test_json = json::parse(test_file);

    	cout << "Done." << endl;

		stream_file.close();
		query_file.close();
		test_file.close();

    	sketch_to_test = test_json["sketch"]["type"].get<string>();
    	depth  = test_json["deep"].get<int>();
    	error = test_json["sketch"]["error"].get<double>();
    
		cout << "Using bounds : " << endl;
    	for(auto& it : test_json["bound_box"].items()){
        	bounds[stoi(it.key())] = it.value();
			cout << it.value() << " ";
    	}
		cout << endl;

		cout << "Discrete bounds : " << endl;
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

			cout << discrete_bounds[i] << " ";
    	}
		cout << endl;
	
    	regions_to_search = query_json["queries"]["rank"].get<vector<pair<int, vector<double>>>>();


		if(sketch_to_test == "q_digest" or sketch_to_test == "dcs"){
   			stream_ww = stream_json["stream"].get<vector<pair<pair<int, int>, pair<double, double>>>>();
			universe = test_json["sketch"]["universe"].get<int>();
		}else{
			stream = stream_json["stream"].get<vector<pair<int, pair<double, double>>>>();
		}
	}
    
	cout << "Sketch used : " << sketch_to_test << endl;

	if(sketch_to_test == "q_digest" or sketch_to_test == "dcs"){
	
		cout << "Using error & universe : " << error << " " << universe << endl; 
    	cout << "Running Experiment..." << endl;
		if(sketch_to_test == "dcs"){
			out_info = dcs_test(stream_ww, regions_to_search, error, universe, discrete_bounds, bounds, depth);
		}else{
			out_info = q_digest_test(stream_ww, regions_to_search, error, universe, discrete_bounds, bounds, depth);
		}
	}else if(sketch_to_test == "kll" or sketch_to_test == "gk"){

    	cout << "Using error : " << error << endl;
		cout << "Running Experiment..." << endl;
		if(sketch_to_test == "gk"){
			out_info = gk_test(stream, regions_to_search, error, discrete_bounds, bounds, depth);
		}else{
			out_info = kll_test(stream, regions_to_search, error, discrete_bounds, bounds, depth);
		}
	}else{
        DEBUG_ERR("This sketch isn't supported for the quantile quadtree");
        return -1;
	}

    cout << "Done." << endl;

    out_info["stream_file"] = args[0];
    out_info["query_file"] = args[1];
    out_info["test_file"] = args[2];

    cout << "Writing in the logger...";
    cout.flush();
	qsbd::logger out_file(args[3]);
    out_file << out_info.dump(4);
    cout << "Done." << endl;

    cout << "Exiting." << endl;

    return 0;
}
