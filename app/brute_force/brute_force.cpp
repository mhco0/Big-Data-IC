#include <stream_maker/stream_maker.cpp>
#include <aabb/aabb.hpp>
#include <nlohmann/json.hpp>
#include <logger/logger.h>
#include <utils/utils.h>
#include <qsbd_debug/qsbd_debug.h>
#include <map>
#include <iostream>
#include <fstream>
#include <functional>
using namespace std;
using json = nlohmann::json;

vector<int> brute_force_search(const vector<pair<int, pair<double, double>>>& stream, const qsbd::aabb<int>& region, double bounds[4], int depth){
    vector<int> ans;

    for(auto& it : stream){
        qsbd::point<int> p(qsbd::map_coord(it.second.first, bounds[0], bounds[2], depth), qsbd::map_coord(it.second.second, bounds[1], bounds[3], depth));
        if (region.contains(p)){
            ans.push_back(it.first);
        }
    }

    return ans;
}

vector<pair<int, int>> brute_force_search(const vector<pair<pair<int, int>, pair<double, double>>>& stream, const qsbd::aabb<int>& region, double bounds[4], int depth){
    vector<pair<int, int>> ans;

    for(auto& it : stream){
        qsbd::point<int> p(qsbd::map_coord(it.second.first, bounds[0], bounds[2], depth), qsbd::map_coord(it.second.second, bounds[1], bounds[3], depth));
        if (region.contains(p)){
            ans.push_back(it.first);
        }
    }

    return ans;
}


json process_ranks(json& stream_file, json& query_file, double bounds[4], int depth){
    vector<pair<int, pair<double, double>>> stream = stream_file["stream"];
    vector<pair<int, vector<double>>> regions_to_search = query_file["queries"]["rank"].get<vector<pair<int, vector<double>>>>();
    sort(stream.begin(), stream.end());
    json out_info = json::object();
    out_info["queries"] = json::array();

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
        json region_info;
        int rank = -1;

        auto stream_in_region = brute_force_search(stream, region, bounds, depth);
        if(stream_in_region.size() > 0){
            rank = 0;

            for(auto& it : stream_in_region){
                if(it < regions_to_search[j].first){
                    rank++;
                }else{
                    break;
                }
            }
        }

        region_info["region"] = regions_to_search[j].second;
        region_info["value"] = regions_to_search[j].first;
        region_info["rank"] = rank;

        out_info["queries"].push_back(region_info);
    }

    return out_info;
}

json process_ranks_with_weight(json& stream_file, json& query_file, double bounds[4], int depth){
    vector<pair<pair<int, int>, pair<double, double>>> stream = stream_file["stream"];
    vector<pair<int, vector<double>>> regions_to_search = query_file["queries"]["rank"].get<vector<pair<int, vector<double>>>>();
    sort(stream.begin(), stream.end());
    json out_info = json::object();
    out_info["queries"] = json::array();

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
        json region_info;
        int rank = -1;

        auto stream_in_region = brute_force_search(stream, region, bounds, depth);

        if(stream_in_region.size() > 0){
            rank = 0;

            for(auto& it : stream_in_region){
                if(it.first < regions_to_search[j].first){
                    rank += it.second;
                }else{
                    break;
                }
            }
        }

        region_info["region"] = regions_to_search[j].second;
        region_info["value"] = regions_to_search[j].first;
        region_info["rank"] = rank;

        out_info["queries"].push_back(region_info);
    }

    return out_info;
}

int main(int argc, char* argv[]){
    deque<string> args = qsbd::process_args(argc, argv);
    double bounds[4] = {0.0, 0.0, 0.0, 0.0};
    int depth = 0;


    if(args.size() != 9){
        DEBUG_ERR("You need to pass the data stream, the requested queries, the ouputfile and the type of the quantile sketch.\n");
        return -1;
    } 

    json stream_file = json::parse(ifstream(args[0]));
    json query_file = json::parse(ifstream(args[1]));
    qsbd::logger out_file(args[2]);

    bounds[0] = stod(args[4]);
    bounds[1] = stod(args[5]);
    bounds[2] = stod(args[6]);
    bounds[3] = stod(args[7]);

    depth = stoi(args[8]);

    json out_info;

    if(args[3] == "gk" or args[3] == "kll"){
        out_info = process_ranks(stream_file, query_file, bounds, depth);
    }else{
        out_info = process_ranks_with_weight(stream_file, query_file, bounds, depth);
    }

    out_info["stream_file"] = args[0];
    out_info["query_file"] = args[1];

    out_file << out_info.dump(4);

    return 0;
}