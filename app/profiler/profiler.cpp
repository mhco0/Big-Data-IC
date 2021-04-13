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

void gk_run(){
    // STREAM
    int vector_size = 1000000; 
    int min_v = 0;
    int max_v = 10000;
    double minx = 0.0;
    double miny = 0.0;
    double maxx = 1280.0;
    double maxy = 780.0;
    int citys = 100;
    double max_radius = 5.0;

    // TREE
    double error = 0.3;
    int attempts = 1;
    int deep = 6;
    double bounds[4] = {0.0, 0.0, 1280.0, 720.0};

    // QUERIES
    int initial_value = 0;
    int final_value = 1000;
    int step = 1;

    vector<pair<int, pair<double, double>>> stream = qsbd::stream_maker::random_stream_city(vector_size, minx, miny, maxx, maxy, min_v, max_v, citys, max_radius);
    vector<pair<int, vector<double>>> regions_to_search;
    
    qsbd::aabb resolution(bounds[0], bounds[1], bounds[2], bounds[3]);
    
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

    string progress = "0%";
    cout << progress;
    cout.flush();
    for(int i = 0; i < attempts; i++){
        qsbd::gk_factory<int> factory(error);
        qsbd::quantile_quadtree<int> qq_test(resolution, deep, &factory);

        for(auto& it : stream){
            qq_test.update(qsbd::point<int>(it.second.first, it.second.second), it.first);
        }

        for(int j = 0; j < regions_to_search.size(); j++){
            qsbd::aabb region(regions_to_search[j].second[0], regions_to_search[j].second[1], regions_to_search[j].second[2], regions_to_search[j].second[3]);

            int rank = qq_test.query(region, regions_to_search[j].first);
        }

        int progress_pct = (int) ((i * 100) / attempts);
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

void q_digest_run(){
    // STREAM
    int vector_size = 100; 
    int min_v = 0;
    int max_v = 10000;
    double minx = 0.0;
    double miny = 0.0;
    double maxx = 1280.0;
    double maxy = 780.0;
    int min_w = 0;
    int max_w = 70;

    // TREE
    double error = 0.3;
    int universe = 1024;
    int attempts = 10;
    int deep = 10;
    double bounds[4] = {0.0, 0.0, 1280.0, 720.0};

    // QUERIES
    int initial_value = 0;
    int final_value = 1000;
    int step = 1;

    vector<pair<pair<int, int>, pair<double, double>>> stream = qsbd::stream_maker::random_stream_in_region_with_weight(vector_size, minx, miny, maxx, maxy, min_v, max_v, min_w, max_w);
    vector<pair<int, vector<double>>> regions_to_search;
    
    qsbd::aabb resolution(bounds[0], bounds[1], bounds[2], bounds[3]);
    
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

    string progress = "0%";
    cout << progress;
    cout.flush();
    for(int i = 0; i < attempts; i++){
        qsbd::q_digest_factory factory(error, universe);
        qsbd::quantile_quadtree<int> qq_test(resolution, deep, &factory);

        for(auto& it : stream){
            qq_test.update(qsbd::point<int>(it.second.first, it.second.second), it.first.first, it.first.second);
        }

        for(int j = 0; j < regions_to_search.size(); j++){
            qsbd::aabb region(regions_to_search[j].second[0], regions_to_search[j].second[1], regions_to_search[j].second[2], regions_to_search[j].second[3]);

            int rank = qq_test.query(region, regions_to_search[j].first);
        }

        int progress_pct = (int) ((i * 100) / attempts);
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

void dcs_run(){
    // STREAM
    int vector_size = 100; 
    int min_v = 0;
    int max_v = 10000;
    double minx = 0.0;
    double miny = 0.0;
    double maxx = 1280.0;
    double maxy = 780.0;
    int min_w = 0;
    int max_w = 70;

    // TREE
    double error = 0.3;
    int universe = 1024;
    int attempts = 100;
    int deep = 10;
    double bounds[4] = {0.0, 0.0, 1280.0, 720.0};

    // QUERIES
    int initial_value = 0;
    int final_value = 1000;
    int step = 1;

    vector<pair<pair<int, int>, pair<double, double>>> stream = qsbd::stream_maker::random_stream_in_region_with_weight(vector_size, minx, miny, maxx, maxy, min_v, max_v, min_w, max_w);
    vector<pair<int, vector<double>>> regions_to_search;
    
    qsbd::aabb resolution(bounds[0], bounds[1], bounds[2], bounds[3]);
    
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

    string progress = "0%";
    cout << progress;
    cout.flush();
    for(int i = 0; i < attempts; i++){
        qsbd::dcs_factory factory(error, universe);
        qsbd::quantile_quadtree<int> qq_test(resolution, deep, &factory);


        for(auto& it : stream){
            qq_test.update(qsbd::point<int>(it.second.first, it.second.second), it.first.first, it.first.second);
        }

        DEBUG("OK_UPDATE_ALL");

        for(int j = 0; j < regions_to_search.size(); j++){
            qsbd::aabb region(regions_to_search[j].second[0], regions_to_search[j].second[1], regions_to_search[j].second[2], regions_to_search[j].second[3]);

            int rank = qq_test.query(region, regions_to_search[j].first);
        }

        int progress_pct = (int) ((i * 100) / attempts);
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

void kll_run(){
    // STREAM
    int vector_size = 100; 
    int min_v = 0;
    int max_v = 10000;
    double minx = 0.0;
    double miny = 0.0;
    double maxx = 1280.0;
    double maxy = 780.0;
    int citys = 100;
    double max_radius = 5.0;

    // TREE
    double error = 0.3;
    int attempts = 100;
    int deep = 10;
    double bounds[4] = {0.0, 0.0, 1280.0, 720.0};

    // QUERIES
    int initial_value = 0;
    int final_value = 1000;
    int step = 1;

    vector<pair<int, pair<double, double>>> stream = qsbd::stream_maker::random_stream_city(vector_size, minx, miny, maxx, maxy, min_v, max_v, citys, max_radius);
    vector<pair<int, vector<double>>> regions_to_search;
    
    qsbd::aabb resolution(bounds[0], bounds[1], bounds[2], bounds[3]);
    
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

    string progress = "0%";
    cout << progress;
    cout.flush();
    for(int i = 0; i < attempts; i++){
        qsbd::kll_factory<int> factory(error);
        qsbd::quantile_quadtree<int> qq_test(resolution, deep, &factory);

        for(auto& it : stream){
            qq_test.update(qsbd::point<int>(it.second.first, it.second.second), it.first);
        }

        for(int j = 0; j < regions_to_search.size(); j++){
            qsbd::aabb region(regions_to_search[j].second[0], regions_to_search[j].second[1], regions_to_search[j].second[2], regions_to_search[j].second[3]);

            int rank = qq_test.query(region, regions_to_search[j].first);
        }

        int progress_pct = (int) ((i * 100) / attempts);
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
    deque<string> args = qsbd::process_args(argc, argv);

    if (args.size() != 1){
        DEBUG_ERR("You need pass only the test name. [ kll / q_digest / dcs / gk]");
        return -1;
    }

    if(args[0] == "kll") kll_run();
    else if(args[0] == "gk") gk_run();
    else if(args[0] == "q_digest") q_digest_run();
    else if(args[0] == "dcs") dcs_run();
    else{
        DEBUG_ERR("Invalid test arg.");
        return -1;
    }

    return 0;
}