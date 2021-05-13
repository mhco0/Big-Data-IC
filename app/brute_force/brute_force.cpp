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

vector<int> brute_force_search(const vector<pair<int, pair<double, double>>>& stream, const qsbd::aabb<int>& region){
    vector<int> ans;

    for(auto& it : stream){
        qsbd::point<int> p(it.second.first, it.second.second);
        if (region.contains(p)){
            ans.push_back(it.first);
        }
    }

    return ans;
}

vector<pair<int, int>> brute_force_search(const vector<pair<pair<int, int>, pair<double, double>>>& stream, const qsbd::aabb<int>& region){
    vector<pair<int, int>> ans;

    for(auto& it : stream){
        qsbd::point<int> p(it.second.first, it.second.second);
        if (region.contains(p)){
            ans.push_back(it.first);
        }
    }

    return ans;
}


json process_ranks(json& stream_file, json& query_file){
    vector<pair<int, pair<double, double>>> stream = stream_file["stream"];
    vector<pair<int, vector<double>>> regions_to_search = query_file["queries"]["rank"].get<vector<pair<int, vector<double>>>>();
    sort(stream.begin(), stream.end());
    json out_info = json::object();
    out_info["queries"] = json::array();

    for(int j = 0; j < regions_to_search.size(); j++){
        qsbd::aabb<int> region(regions_to_search[j].second[0], regions_to_search[j].second[1], regions_to_search[j].second[2], regions_to_search[j].second[3]);
        json region_info;
        int rank = -1;

        auto stream_in_region = brute_force_search(stream, region);
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

json process_ranks_with_weight(json& stream_file, json& query_file){
    vector<pair<pair<int, int>, pair<double, double>>> stream = stream_file["stream"];
    vector<pair<int, vector<double>>> regions_to_search = query_file["queries"]["rank"].get<vector<pair<int, vector<double>>>>();
    sort(stream.begin(), stream.end());
    json out_info = json::object();
    out_info["queries"] = json::array();

    for(int j = 0; j < regions_to_search.size(); j++){
        qsbd::aabb<int> region(regions_to_search[j].second[0], regions_to_search[j].second[1], regions_to_search[j].second[2], regions_to_search[j].second[3]);
        json region_info;
        int rank = -1;

        auto stream_in_region = brute_force_search(stream, region);

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

    if(args.size() != 4){
        DEBUG_ERR("You need to pass the data stream, the requested queries, the ouputfile and the type of the quantile sketch.\n");
        return -1;
    } 

    json stream_file = json::parse(ifstream(args[0]));
    json query_file = json::parse(ifstream(args[1]));
    qsbd::logger out_file(args[2]);

    json out_info;

    if(args[3] == "gk" or args[3] == "kll"){
        out_info = process_ranks(stream_file, query_file);
    }else{
        out_info = process_ranks_with_weight(stream_file, query_file);
    }

    out_info["stream_file"] = args[0];
    out_info["query_file"] = args[1];

    out_file << out_info.dump(4);

    return 0;
}