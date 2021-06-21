#include <nlohmann/json.hpp>
#include <utils/utils.h>
#include <qsbd_debug/qsbd_debug.h>
#include <map>
#include <iostream>
#include <fstream>
#include <functional>
#include <cstdlib>
using namespace std;
using json = nlohmann::json;

int main(int argc, char* argv[]){
    deque<string> args = qsbd::process_args(argc, argv);
    string stream_file_prefix = "data/stream_";
    string query_file_prefix = "query/query_"; // temporary
    string output_file_prefix = "out/";
    string temp_file = "temp_file_";
    vector<double> bounds = {0.0, 0.0, 1280.0, 720.0};
    int max_deep = 10;
    int stream_sizes = 10000000;
    double error = 0.3;
    int universe = 1024;
    bool only_leafs = true;
    
    if(args.size() != 2){
        DEBUG_ERR("You need to pass the test name and the option for the stream\n Ex [gk/ kll/ q_digest/ dcs] [-c/ -cw/ -u/ -uw]");
        return -1;
    }

    output_file_prefix += (args[0] + "_");
    temp_file += args[0] + ".json";

    if(args[1] == "-c"){
        output_file_prefix += "city_";
        stream_file_prefix += "city_";
    }else if(args[1] == "-cw"){
        output_file_prefix += "city_ww_";
        stream_file_prefix += "city_ww_";
    }else if(args[1] == "-u"){
        output_file_prefix += "uniform_";
        stream_file_prefix += "uniform_";
    }else if(args[1] == "-uw"){
        output_file_prefix += "uniform_ww_";
        stream_file_prefix += "uniform_ww_";
    }else{
        DEBUG_ERR("Option on stream not found");
        return -1;
    }

    for(int deep = 5; deep <= max_deep; deep += 5){
        json config = json::object();

        ofstream temp_config(temp_file);

        if(not temp_config.is_open()){
            DEBUG_ERR("Couldn't open temporary file.");
            return -1;
        }

        config["sketch"]["type"] = args[0];
        config["sketch"]["error"] = error;

        if(args[0] == "dcs" or args[0] == "q_digest"){
            config["sketch"]["universe"] = universe;
        }

        config["bound_box"] = bounds;
        config["deep"] = deep;
        config["only_leafs"] = only_leafs;

        temp_config << config.dump(4);
        temp_config.close();

        for(int i = stream_sizes; i <= stream_sizes; i *= 10){
            string stream_file = stream_file_prefix + to_string(i) + ".json";
            string query_file = query_file_prefix + to_string(i) + "_uniform.json";
            string output_file = output_file_prefix + to_string(i) + "_" + to_string(deep) + "_leaf.json";

            DEBUG(stream_file);
            DEBUG(query_file);
            DEBUG(output_file);

            DEBUG(string("./bin/quantile_quadtree_experiment ") + stream_file + " " + query_file + " " + temp_file + " " + output_file);
            DEBUG("\n");

            if (system((string("./bin/quantile_quadtree_experiment ") + stream_file + " " + query_file + " " + temp_file + " " + output_file).c_str()) != 0){
                DEBUG_ERR("Some Error happen experiment");
                return -1;
            }
        }

        if(remove(temp_file.c_str()) != 0){
            DEBUG_ERR("ERROR. Couldn't remove temporary file");

            return -1;
        }
    }

    return 0;
}
