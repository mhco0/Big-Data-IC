#include <nlohmann/json.hpp>
#include <stream_maker/stream_maker.h>
#include <logger/logger.h>
#include <utils/utils.h>
#include <qsbd_debug/qsbd_debug.h>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
using json = nlohmann::json;

void generate_query(const json& stream_file, ofstream& output){
    bool query_ranks = (stream_file["queries"].find("rank") != stream_file["queries"].end());
    bool query_quantiles = (stream_file["queries"].find("quantile") != stream_file["queries"].end());
    bool query_cdf = (stream_file["queries"].find("cdf") != stream_file["queries"].end());
    int n_rank = -1;
    int n_quantile = -1;
    int n_cdf = -1;
    double bounds[4] = {0, 0, 0, 0};

    for(auto& it : stream_file["search_bound"].items()){
        bounds[stoi(it.key())] = it.value();
    }

    json temp;

    temp["queries"] = json::object();

    if(query_ranks){
        temp["queries"]["rank"] = json::array();

        int initial_value = stream_file["queries"]["rank"]["initial_value"].get<int>();
        int final_value = stream_file["queries"]["rank"]["final_value"].get<int>();
        int step = stream_file["queries"]["rank"]["step"].get<int>();

        for(int i = initial_value; i < final_value; i += step){
            auto query_bound = qsbd::stream_maker::random_rectangle_in_region(bounds[0], bounds[1], bounds[2], bounds[3]);
            vector<double> rect;
            pair<int, vector<double>> rank_query;

            rect.push_back(query_bound.first.first);
            rect.push_back(query_bound.first.second);
            rect.push_back(query_bound.second.first);
            rect.push_back(query_bound.second.second);

            rank_query = make_pair(i, rect);
            temp["queries"]["rank"].push_back(rank_query);
        }
    }

    if(query_quantiles){
        temp["queries"]["quantile"] = json::array();

        double initial_value = stream_file["queries"]["quantile"]["initial_value"].get<double>();
        double final_value = stream_file["queries"]["quantile"]["final_value"].get<double>();
        double step = stream_file["queries"]["quantile"]["step"].get<double>();

        for(double i = initial_value; i < final_value; i += step){
            auto query_bound = qsbd::stream_maker::random_rectangle_in_region(bounds[0], bounds[1], bounds[2], bounds[3]);
            vector<double> rect;
            pair<double, vector<double>> quantile_query;

            rect.push_back(query_bound.first.first);
            rect.push_back(query_bound.first.second);
            rect.push_back(query_bound.second.first);
            rect.push_back(query_bound.second.second);

            quantile_query = make_pair(i, rect);
            temp["queries"]["quantile"].push_back(quantile_query);
        }
    }

    if(query_cdf){
        temp["queries"]["cdf"] = json::array();

        double initial_value = stream_file["queries"]["cdf"]["initial_value"].get<double>();
        double final_value = stream_file["queries"]["cdf"]["final_value"].get<double>();
        double step = stream_file["queries"]["cdf"]["step"].get<double>();

        for(double i = initial_value; i < final_value; i += step){
            auto query_bound = qsbd::stream_maker::random_rectangle_in_region(bounds[0], bounds[1], bounds[2], bounds[3]);
            vector<double> rect;
            pair<double, vector<double>> cdf_query;

            rect.push_back(query_bound.first.first);
            rect.push_back(query_bound.first.second);
            rect.push_back(query_bound.second.first);
            rect.push_back(query_bound.second.second);

            cdf_query = make_pair(i, rect);
            temp["queries"]["cdf"].push_back(cdf_query);
        }
    }

    output << temp.dump(4);

    return;
}

int main(int argc, char * argv[]){
    deque<string> args = qsbd::process_args(argc, argv);

    if (args.size() != 2) {
        DEBUG_ERR("You need to pass one json file configuration.");
        return -1;
    }

    if (not qsbd::ends_with(args[0], ".json")){
        DEBUG("The file needs to be a json with the json extension");
    }

    ifstream config_file(args[0]);
   
    if(config_file.is_open()) {
        json config = json::parse(config_file);
        string output_file = args[1];

        if (not qsbd::ends_with(args[0], ".json")){
            DEBUG("The output file needs to be a json with the json extension");
        }

        qsbd::logger out_log(output_file);

        if(out_log.is_open()){
            generate_query(config, out_log);
        }else{
            DEBUG_ERR("Coundn't open/create output file");
            return -1;
        }

        config_file.close();
    }else{
        DEBUG_ERR("Couldn't open the file");
        return -1;
    }

    return 0;
}