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
    vector<string> types_of_plots = {"real_rank", "app_rank"};
    vector<string> streams_files;
    string file_prefix = "out/";
    string test_name;
    string output_file;
    string real_ranks_file;
    int stream_sizes = 10000000;

    if(args.size() != 3){
        DEBUG_ERR("You need to pass the test name and the option, the requested graph and the ouputfile.\n Ex [gk/ kll/ q_digest/ dcs] [-c/ -cw/ -u/ -uw] [deep]");
        return -1;
    }   

    file_prefix += (args[0] + "_");
    test_name = args[0] + "_test";
    output_file = file_prefix;
    real_ranks_file = "out/brute_force_";

    
    if(args[1] == "-c"){
        file_prefix += "city_";
        real_ranks_file += "city_";
    }else if(args[1] == "-cw"){
        file_prefix += "city_ww_";
        real_ranks_file += "city_ww_";
    }else if(args[1] == "-u"){
        file_prefix += "uniform_";
        real_ranks_file += "uniform_";
    }else if(args[1] == "-uw"){
        file_prefix += "uniform_ww_";
        real_ranks_file += "uniform_ww_";
    }else{
        DEBUG_ERR("Option on stream not found");
        return -1;
    }


    for(auto& it : types_of_plots){
        if(it == "app_rank"){
            for(int i = 1; i <= stream_sizes; i *= 10){
                string file = file_prefix;
                output_file = file_prefix + it + "_" + to_string(i) + "_" + args[2] + ".dat";

                ofstream out(output_file);

                if(not out.is_open()){
                    DEBUG_ERR("Couldn't open output app rank file");

                    return -1;
                }

                file += (to_string(i) + "_" + args[2] + ".json");


                ifstream infos(file);

                if (infos.is_open()){
                    json all_infos = json::parse(infos);

                    for(auto& it : all_infos[test_name]["time"]["queries"].items()){
                        int x_label = it.value()["value"].get<int>();
                        int y_label = it.value()["rank"].get<int>();

                        out << x_label << " " << y_label << endl;
                    }
                    
                    infos.close();
                       
                }else{
                    DEBUG_ERR("Couldn't open file. File:");
                    DEBUG_ERR(file);

                    return -1;
                }

                out.close();
            }

            
        }else{
            for(int i = 1; i <= stream_sizes; i *= 10){
                output_file = real_ranks_file + it + "_" + to_string(i) + ".dat";

                ofstream out(output_file);

                if(not out.is_open()){
                    DEBUG_ERR("Couldn't open output real rank file");

                    return -1;
                }

                string file = real_ranks_file;

                file += (to_string(i) + ".json");

                ifstream infos(file);

                if (infos.is_open()){
                    json all_infos = json::parse(infos);

                    for(auto& it : all_infos["queries"].items()){
                        int x_label = it.value()["value"].get<int>();
                        int y_label = it.value()["rank"].get<int>();

                        out << x_label << " " << y_label << endl;
                    }
                    
                    infos.close();
                       
                }else{
                    DEBUG_ERR("Couldn't open file. File:");
                    DEBUG_ERR(file);

                    return -1;
                }

                out.close();
            }
        }
    }


    return 0;
}