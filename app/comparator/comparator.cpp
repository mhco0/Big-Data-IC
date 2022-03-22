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
    vector<string> types_of_plots = {"error", "cumulative_error"};
    vector<string> streams_files;
    string file_prefix = "out/";
    string test_name;
    string output_file;
    string real_ranks_file_name;
    int stream_sizes = 10000000;

    if(args.size() != 3){
        DEBUG_ERR("You need to pass the test name and the option, the requested graph and the ouputfile.\n Ex [gk/ kll/ q_digest/ dcs] [-c/ -cw/ -u/ -uw] [deep]");
        return -1;
    }   

    file_prefix += (args[0] + "_");
    test_name = args[0] + "_test";
    output_file = file_prefix;
    real_ranks_file_name = "out/brute_force_";

    
    if(args[1] == "-c"){
        file_prefix += "city_";
        real_ranks_file_name += "city_";
    }else if(args[1] == "-cw"){
        file_prefix += "city_ww_";
        real_ranks_file_name += "city_ww_";
    }else if(args[1] == "-u"){
        file_prefix += "uniform_";
        real_ranks_file_name += "uniform_";
    }else if(args[1] == "-uw"){
        file_prefix += "uniform_ww_";
        real_ranks_file_name += "uniform_ww_";
    }else{
        DEBUG_ERR("Option on stream not found");
        return -1;
    }


    for(auto& it : types_of_plots){
        if(it == "error"){
            string apprx_file = file_prefix;
            string real_file = real_ranks_file_name;
            
            output_file = file_prefix + it + "_" + to_string(stream_sizes) + "_" + args[2] + ".dat";

            ofstream out(output_file);

            if(not out.is_open()){
                DEBUG_ERR("Couldn't open output error file");

                return -1;
            }

            apprx_file += (to_string(stream_sizes) + "_" + args[2] + ".json");

            ifstream app_infos(apprx_file);

            real_file += (to_string(stream_sizes) + ".json");

            ifstream real_infos(real_file);

            if (app_infos.is_open() and real_infos.is_open()){
                json sketchs_json = json::parse(app_infos);
                json real_ranks_json = json::parse(real_infos);

                vector<int> app_values_query;
                vector<int> app_ranks;
                vector<int> real_values_query;
                vector<int> real_ranks;

                for(auto& it : sketchs_json[test_name]["time"][string("queries_") + to_string(stream_sizes)].items()){
                    int x_label = it.value()["value"].get<int>();
                    int app_rank = it.value()["rank"].get<int>();

                    app_values_query.push_back(x_label);
                    app_ranks.push_back(app_rank);
                }

                for(auto& it : real_ranks_json["queries"].items()){
                    int x_label = it.value()["value"].get<int>();
                    int real_rank = it.value()["rank"].get<int>();

                    real_values_query.push_back(x_label);
                    real_ranks.push_back(real_rank); 
                }  

                ASSERT(app_values_query.size() == real_values_query.size());
                ASSERT(app_ranks.size() == real_ranks.size());


                for(int i = 0; i < app_ranks.size(); i++){
                    ASSERT(app_values_query[i] == real_values_query[i]);
                    
                    out << app_values_query[i] << " " << abs(real_ranks[i] - app_ranks[i]) << endl;
                }
                
                app_infos.close();
                real_infos.close();
            }else{
                DEBUG_ERR("Couldn't open apprx_file or real file. apprx_file:");
                DEBUG_ERR(apprx_file);
                DEBUG_ERR("real file:");
                DEBUG_ERR(real_file);

                return -1;
            }

            out.close();
            
        }else{
        
        }
    }

    return 0;
}