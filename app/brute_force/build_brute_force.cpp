#include <utils/utils.h>
#include <qsbd_debug/qsbd_debug.h>
#include <map>
#include <iostream>
#include <fstream>
#include <functional>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[]){
    deque<string> args = qsbd::process_args(argc, argv);
    string stream_file_prefix = "data/stream_";
    string query_file_prefix = "query/query_"; // temporary
    string output_file_prefix = "out/";
    int stream_sizes = 10000000;
    vector<double> bounds = {0.0, 0.0, 1280.0, 720.0};
    int max_deep = 10;

	if(args.size() != 2){
        DEBUG_ERR("You need to pass the test name and the option for the stream\n Ex [gk/ kll/ q_digest/ dcs] [-c/ -cw/ -u/ -uw]");
        return -1;
    }

    output_file_prefix += ("brute_force_");

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

        for(int i = stream_sizes; i <= stream_sizes; i *= 10){
            string stream_file = stream_file_prefix + to_string(i) + ".json";
            string query_file = query_file_prefix + to_string(i) + ".json";
            string output_file = output_file_prefix + to_string(i) + "_" + to_string(deep) + ".json";

            DEBUG(stream_file);
            DEBUG(query_file);
            DEBUG(output_file);

            DEBUG(string("./bin/brute_force ") + stream_file + " " + query_file + " " + output_file + " " + args[0] + " " + to_string(bounds[0]) + " " + to_string(bounds[1]) + " " + to_string(bounds[2]) + " " + to_string(bounds[3]) + " " + to_string(deep) );
            DEBUG("\n");

            if (system((string("./bin/brute_force ") + stream_file + " " + query_file + " " + output_file + " " + args[0] + " " + to_string(bounds[0]) + " " + to_string(bounds[1]) + " " + to_string(bounds[2]) + " " + to_string(bounds[3]) + " " + to_string(deep) ).c_str()) != 0){
                DEBUG_ERR("Some Error Brute Force");
                return -1;
            }
        }
    }

    return 0;
}
