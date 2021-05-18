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


int memory_summary(const string& test_name, const json& info){
    int total_bytes = 0;

    for(auto& it : info[test_name]["memory"].items()){
        for(auto & it2 : it.value()["memory_used"].items()){
            if ((it2.value()["allocated_type"]) != "[unknown]"){
                total_bytes += it2.value()["bytes"].get<int>();
            }
        }
    }

    return total_bytes;
}

double standard_deviation(const string& test_name, const json& info, vector<int>& sample_values, vector<double>& sample_queries){
    double avg_query_time = info[test_name]["time"]["avg_query_time"].get<double>();
    double variance = 0;
    int queries_size = info[test_name]["time"]["queries"].size();

    for(auto& it : info[test_name]["time"]["queries"].items()){
        double query_time = it.value()["query_time"].get<double>();
        int value = it.value()["value"].get<int>();
        sample_queries.push_back(query_time);
        sample_values.push_back(value);

        variance += (query_time - avg_query_time) * (query_time - avg_query_time);
    }

    variance /= queries_size;

    return sqrt(variance);
}

int main(int argc, char* argv[]){
    deque<string> args = qsbd::process_args(argc, argv);
    //"update_time", "construct_time", "query_time", "memory", 
    vector<string> types_of_plots = {"query_std_dev"};
    vector<string> streams_files;
    string file_prefix = "out/";
    string test_name;
    string output_file;
    int stream_sizes = 10000000;

    if(args.size() != 3){
        DEBUG_ERR("You need to pass the test name and the option, the requested graph and the ouputfile.\n Ex [gk/ kll/ q_digest/ dcs] [-c/ -cw/ -u/ -uw] [deep]");
        return -1;
    }   

    file_prefix += (args[0] + "_");
    test_name = args[0] + "_test";
    output_file = file_prefix;

    
    if(args[1] == "-c"){
        file_prefix += "city_";
    }else if(args[1] == "-cw"){
        file_prefix += "city_ww_";
    }else if(args[1] == "-u"){
        file_prefix += "uniform_";
    }else if(args[1] == "-uw"){
        file_prefix += "uniform_ww_";
    }else{
        DEBUG_ERR("Option on stream not found");
        return -1;
    }

    for(auto& it : types_of_plots){
        output_file = file_prefix + it + "_" + args[2] + ".dat";

        ofstream out(output_file);

        if(not out.is_open()){
            DEBUG_ERR("Couldn't open output file");

            return -1;
        }

        for(int i = 1; i <= stream_sizes; i *= 10){
            string file = file_prefix;

            file += (to_string(i) + "_" + args[2] + ".json");

            ifstream infos(file);

            if (infos.is_open()){
                json all_infos = json::parse(infos);

                int x_label = i;
                double y_label = 0.0;
                
                if (it == "update_time"){   
                    y_label = all_infos[test_name]["time"]["avg_update_time"].get<double>();
                }else if(it == "construct_time"){
                    y_label = all_infos[test_name]["time"]["update_time_overall"].get<double>();
                }else if(it == "query_time"){
                    y_label = all_infos[test_name]["time"]["avg_query_time"].get<double>();
                }else if(it == "memory"){
                    y_label = memory_summary(test_name, all_infos);
                }else if(it == "query_std_dev"){
                    vector<int> values;
                    vector<double> queries_time;
                    y_label = standard_deviation(test_name, all_infos, values, queries_time);

                    out << x_label << " " << all_infos[test_name]["time"]["avg_query_time"].get<double>() << " " << y_label << endl;
                    continue;
                }else{
                    DEBUG_ERR("Option to build y_label not found");
                    return -1;
                }
                
                out << x_label << " " << y_label << endl;

                infos.close();   
            }else{
                DEBUG_ERR("Couldn't open file. File:");
                DEBUG_ERR(file);

                return -1;
            }
        }

        out.close();
    }

    return 0;
}