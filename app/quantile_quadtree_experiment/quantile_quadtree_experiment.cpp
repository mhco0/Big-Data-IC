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

json q_digest_test(const json& stream_file, const json& query_file, const json& test_file){
    vector<pair<pair<int, int>, pair<double, double>>> stream = stream_file["stream"];
    vector<pair<int, vector<double>>> regions_to_search = query_file["queries"]["rank"].get<vector<pair<int, vector<double>>>>();
    double error = test_file["sketch"]["error"].get<double>();
    int universe = test_file["sketch"]["universe"].get<int>();
    int deep = test_file["deep"].get<int>();
    double bounds[4] = {0, 0, 0, 0};
    
    for(auto& it : test_file["bound_box"].items()){
        bounds[stoi(it.key())] = it.value();
    }

    qsbd::aabb<int> bound_box(bounds[0], bounds[1], bounds[2], bounds[3]);
    
    json out_info = json::object();
    out_info["q_digest_test"] = json::object();

    cout << "Update...";
    cout.flush();

    qsbd::q_digest_factory factory(error, universe);
    qsbd::quantile_quadtree<int> qq_test(bound_box, deep, &factory);
    json loop_info;
    double update_time_acc = 0.0;

    qsbd::timer update_overall;

    update_overall.start();
    for(int i = 0, j = 10; i < stream.size(); i++){
        qsbd::timer update_once;

        update_once.start();
        qq_test.update(qsbd::point<int>(stream[i].second.first, stream[i].second.second), stream[i].first.first, stream[i].first.second);
        update_once.end();

        update_time_acc += (double) update_once.count();
        if (i == (j - 1) or i == ((j - 1) / 2)){
            loop_info["time"][string("update_time_acc_") + to_string(j)] = update_time_acc;

            if(i == (j - 1)){
                j *= 10;
            }
        }
    }
    update_overall.end();

    double avg_update_time = update_time_acc / stream.size();

    loop_info["time"]["avg_update_time"] = avg_update_time;
    loop_info["time"]["update_time_overall"] = update_overall.count();

    cout << "Done!" << endl;
    cout << "Queries...";
    cout.flush();

    loop_info["time"]["queries"] = json::array();

    qsbd::timer query_overall;
    double query_time_acc = 0.0;

    query_overall.start();
    for(int j = 0; j < regions_to_search.size(); j++){
        qsbd::aabb<int> region(regions_to_search[j].second[0], regions_to_search[j].second[1], regions_to_search[j].second[2], regions_to_search[j].second[3]);
        qsbd::timer query_once;
        json region_info;
        int rank = -1;

        query_once.start();
        rank = qq_test.query(region, regions_to_search[j].first);
        query_once.end();

        query_time_acc += (double) query_once.count();

        region_info["region"] = regions_to_search[j].second;
        region_info["value"] = regions_to_search[j].first;
        region_info["rank"] = rank;
        region_info["query_time"] = query_overall.count();

        loop_info["time"]["queries"].push_back(region_info);
    }
    query_overall.end();

    double avg_query_time = query_time_acc / regions_to_search.size();

    loop_info["time"]["avg_query_time"] = avg_query_time;
    loop_info["time"]["query_time_overall"] = query_overall.count();

    cout << "Done!" << endl;
    cout << "Memory...";
    cout.flush();

    loop_info["memory"] = {};

    qsbd::mem_track::track_list_memory_usage(loop_info["memory"]);

    out_info["q_digest_test"] = loop_info;

    cout << "Done!" << endl;

    return out_info;
}

json kll_test(const json& stream_file, const json& query_file, const json& test_file){
    vector<pair<int, pair<double, double>>> stream = stream_file["stream"];
    vector<pair<int, vector<double>>> regions_to_search = query_file["queries"]["rank"].get<vector<pair<int, vector<double>>>>();
    double error = test_file["sketch"]["error"].get<double>();
    int deep = test_file["deep"].get<int>();
    double bounds[4] = {0, 0, 0, 0};
    
    for(auto& it : test_file["bound_box"].items()){
        bounds[stoi(it.key())] = it.value();
    }

    qsbd::aabb<int> bound_box(bounds[0], bounds[1], bounds[2], bounds[3]);
    
    json out_info = json::object();
    out_info["kll_test"] = json::object();

    cout << "Update...";
    cout.flush();

    qsbd::kll_factory<int> factory(error);
    qsbd::quantile_quadtree<int> qq_test(bound_box, deep, &factory);
    json loop_info;
    double update_time_acc = 0.0;

    qsbd::timer update_overall;

    update_overall.start();
    for(int i = 0, j = 10; i < stream.size(); i++){
        qsbd::timer update_once;

        update_once.start();
        qq_test.update(qsbd::point<int>(stream[i].second.first, stream[i].second.second), stream[i].first);
        update_once.end();

        update_time_acc += (double) update_once.count();
        if (i == (j - 1) or i == ((j - 1) / 2)){
            loop_info["time"][string("update_time_acc_") + to_string(j)] = update_time_acc;

            if(i == (j - 1)){
                j *= 10;
            }
        }
    }
    update_overall.end();

    double avg_update_time = update_time_acc / stream.size();

    loop_info["time"]["avg_update_time"] = avg_update_time;
    loop_info["time"]["update_time_overall"] = update_overall.count();

    cout << "Done!" << endl;
    cout << "Queries...";
    cout.flush();

    loop_info["time"]["queries"] = json::array();

    qsbd::timer query_overall;
    double query_time_acc = 0.0;

    query_overall.start();
    for(int j = 0; j < regions_to_search.size(); j++){
        qsbd::aabb<int> region(regions_to_search[j].second[0], regions_to_search[j].second[1], regions_to_search[j].second[2], regions_to_search[j].second[3]);
        qsbd::timer query_once;
        json region_info;

        query_once.start();
        int rank = qq_test.query(region, regions_to_search[j].first);
        query_once.end();

        query_time_acc += (double) query_once.count();
    
        region_info["region"] = regions_to_search[j].second;
        region_info["value"] = regions_to_search[j].first;
        region_info["rank"] = rank;
        region_info["query_time"] = query_overall.count();

        loop_info["time"]["queries"].push_back(region_info);
    }

    query_overall.end();

    double avg_query_time = query_time_acc / regions_to_search.size();

    loop_info["time"]["avg_query_time"] = avg_query_time;
    loop_info["time"]["query_time_overall"] = query_overall.count();

    cout << "Done!" << endl;
    cout << "Memory...";
    cout.flush();

    loop_info["memory"] = {};

    qsbd::mem_track::track_list_memory_usage(loop_info["memory"]);

    out_info["kll_test"] = loop_info;

    cout << "Done!" << endl;

    return out_info;
}

json dcs_test(const json& stream_file, const json& query_file, const json& test_file){
    vector<pair<pair<int, int>, pair<double, double>>> stream = stream_file["stream"];
    vector<pair<int, vector<double>>> regions_to_search = query_file["queries"]["rank"].get<vector<pair<int, vector<double>>>>();
    double error = test_file["sketch"]["error"].get<double>();
    int universe = test_file["sketch"]["universe"].get<int>();
    int deep = test_file["deep"].get<int>();
    double bounds[4] = {0, 0, 0, 0};
    
    for(auto& it : test_file["bound_box"].items()){
        bounds[stoi(it.key())] = it.value();
    }

    qsbd::aabb<int> bound_box(bounds[0], bounds[1], bounds[2], bounds[3]);
    
    json out_info = json::object();
    out_info["dcs_test"] = json::object();

    cout << "Update...";
    cout.flush();

    qsbd::dcs_factory factory(error, universe);
    qsbd::quantile_quadtree<int> qq_test(bound_box, deep, &factory);
    json loop_info;
    double update_time_acc = 0.0;

    qsbd::timer update_overall;

    update_overall.start();
    for(int i = 0, j = 10; i < stream.size(); i++){
        qsbd::timer update_once;

        update_once.start();
        qq_test.update(qsbd::point<int>(stream[i].second.first, stream[i].second.second), stream[i].first.first, stream[i].first.second);
        update_once.end();

        update_time_acc += (double) update_once.count();
        if (i == (j - 1) or i == ((j - 1) / 2)){
            loop_info["time"][string("update_time_acc_") + to_string(j)] = update_time_acc;

            if(i == (j - 1)){
                j *= 10;
            }
        }
    }
    update_overall.end();

    double avg_update_time = update_time_acc / stream.size();

    loop_info["time"]["avg_update_time"] = avg_update_time;
    loop_info["time"]["update_time_overall"] = update_overall.count();

    cout << "Done!" << endl;
    cout << "Queries...";
    cout.flush();

    loop_info["time"]["queries"] = json::array();

    qsbd::timer query_overall;
    double query_time_acc = 0.0;

    query_overall.start();
    for(int j = 0; j < regions_to_search.size(); j++){
        qsbd::aabb<int> region(regions_to_search[j].second[0], regions_to_search[j].second[1], regions_to_search[j].second[2], regions_to_search[j].second[3]);
        qsbd::timer query_once;
        json region_info;

        query_once.start();
        int rank = qq_test.query(region, regions_to_search[j].first);
        query_once.end();

        query_time_acc += (double) query_once.count();

        region_info["region"] = regions_to_search[j].second;
        region_info["value"] = regions_to_search[j].first;
        region_info["rank"] = rank;
        region_info["query_time"] = query_overall.count();

        loop_info["time"]["queries"].push_back(region_info);
    }
    query_overall.end();

    double avg_query_time = query_time_acc / regions_to_search.size();

    loop_info["time"]["avg_query_time"] = avg_query_time;
    loop_info["time"]["query_time_overall"] = query_overall.count();

    cout << "Done!" << endl;
    cout << "Memory...";
    cout.flush();

    loop_info["memory"] = {};

    qsbd::mem_track::track_list_memory_usage(loop_info["memory"]);

    out_info["dcs_test"] = loop_info;

    cout << "Done!" << endl;
    
    return out_info;
}

json gk_test(const json& stream_file, const json& query_file, const json& test_file){
    vector<pair<int, pair<double, double>>> stream = stream_file["stream"];
    vector<pair<int, vector<double>>> regions_to_search = query_file["queries"]["rank"].get<vector<pair<int, vector<double>>>>();
    double error = test_file["sketch"]["error"].get<double>();
    int deep = test_file["deep"].get<int>();
    double bounds[4] = {0, 0, 0, 0};
    
    for(auto& it : test_file["bound_box"].items()){
        bounds[stoi(it.key())] = it.value();
    }

    qsbd::aabb<int> resolution(bounds[0], bounds[1], bounds[2], bounds[3]);
    
    json out_info = json::object();
    out_info["gk_test"] = json::object();

    cout << "Update...";
    cout.flush();

    qsbd::gk_factory<int> factory(error);
    qsbd::quantile_quadtree<int> qq_test(resolution, deep, &factory);
    json loop_info;
    double update_time_acc = 0.0;

    qsbd::timer update_overall;

    update_overall.start();
    for(int i = 0, j = 10; i < stream.size(); i++){
        qsbd::timer update_once;

        update_once.start();
        qq_test.update(qsbd::point<int>(stream[i].second.first, stream[i].second.second), stream[i].first);
        update_once.end();

        update_time_acc += (double) update_once.count();
        if (i == (j - 1) or i == ((j - 1) / 2)){
            loop_info["time"][string("update_time_acc_") + to_string(j)] = update_time_acc;

            if(i == (j - 1)){
                j *= 10;
            }
        }
    }
    update_overall.end();

    double avg_update_time = update_time_acc / stream.size();

    loop_info["time"]["avg_update_time"] = avg_update_time;
    loop_info["time"]["update_time_overall"] = update_overall.count();

    loop_info["time"]["queries"] = json::array();

    qsbd::timer query_overall;
    double query_time_acc = 0.0;

    cout << "Done!" << endl;
    cout << "Queries...";
    cout.flush();

    query_overall.start();
    for(int j = 0; j < regions_to_search.size(); j++){
        qsbd::aabb<int> region(regions_to_search[j].second[0], regions_to_search[j].second[1], regions_to_search[j].second[2], regions_to_search[j].second[3]);
        qsbd::timer query_once;
        json region_info;

        query_once.start();
        int rank = qq_test.query(region, regions_to_search[j].first);
        query_once.end();

        query_time_acc += (double) query_once.count();
        
        region_info["region"] = regions_to_search[j].second;
        region_info["value"] = regions_to_search[j].first;
        region_info["rank"] = rank;
        region_info["query_time"] = query_overall.count();

        loop_info["time"]["queries"].push_back(region_info);
    }
    query_overall.end();

    double avg_query_time = query_time_acc / regions_to_search.size();

    cout << "Done!" << endl;
    cout << "Memory...";
    cout.flush();

    loop_info["time"]["avg_query_time"] = avg_query_time;
    loop_info["time"]["query_time_overall"] = query_overall.count();
    loop_info["memory"] = {};

    qsbd::mem_track::track_list_memory_usage(loop_info["memory"]);

    out_info["gk_test"] = loop_info;

    cout << "Done!" << endl;

    return out_info;
}

int main(int argc, char * argv[]){
    deque<string> args = qsbd::process_args(argc, argv);

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

    // see if i can open the file write after
    json stream_file = json::parse(ifstream(args[0]));
    json query_file = json::parse(ifstream(args[1]));
    json test_file = json::parse(ifstream(args[2]));
    json out_info;

    cout << "Done." << endl;

    string sketch_to_test = test_file["sketch"]["type"].get<string>();
    qsbd::logger out_file(args[3]);

    cout << "Sketch used : " << sketch_to_test << endl;

    cout << "Running Experiment..." << endl;

    if(sketch_to_test == "q_digest"){
        out_info = q_digest_test(stream_file, query_file, test_file);
    }else if(sketch_to_test == "kll"){ 
        out_info = kll_test(stream_file, query_file, test_file);
    }else if(sketch_to_test == "dcs"){
        out_info = dcs_test(stream_file, query_file, test_file);
    }else if(sketch_to_test == "gk"){
        out_info = gk_test(stream_file, query_file, test_file);
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
    out_file << out_info.dump(4);
    cout << "Done." << endl;

    cout << "Exiting." << endl;

    return 0;
}