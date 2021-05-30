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


void x_label_by_stream_size(vector<int>& x_samples, int stream_sizes){
    for(int i = 0, j = 10; i < stream_sizes; i++){
        if (i == (j - 1) or i == ((j - 1) / 2)){
            x_samples.push_back((i + 1));
            if(i == (j - 1)){
                j *= 10;
            }
        }
    }
}

void update_time_summary(const string& test_name, const json& info, vector<double>& y_samples, int stream_sizes){
    for(int i = 0, j = 10; i < stream_sizes; i++){
        if (i == (j - 1) or i == ((j - 1) / 2)){
            double avg_upd_time = info[test_name]["time"][string("avg_update_time_") + to_string(i + 1)].get<double>();
            
            y_samples.push_back(avg_upd_time);
            
            if(i == (j - 1)){
                j *= 10;
            }
        }
    }
}

void construct_time_summary(const string& test_name, const json& info, vector<double>& y_samples, int stream_sizes){
    for(int i = 0, j = 10; i < stream_sizes; i++){
        if (i == (j - 1) or i == ((j - 1) / 2)){
            double acc_upd_time = info[test_name]["time"][string("update_time_acc_") + to_string(i + 1)].get<double>();
            
            y_samples.push_back(acc_upd_time);
            
            if(i == (j - 1)){
                j *= 10;
            }
        }
    }
}

void query_time_summary(const string& test_name, const json& info, vector<double>& y_samples, int stream_sizes){
    for(int i = 0, j = 10; i < stream_sizes; i++){
        if (i == (j - 1) or i == ((j - 1) / 2)){
            double avg_query_time = info[test_name]["time"][string("avg_query_time_") + to_string(i + 1)].get<double>();
            
            y_samples.push_back(avg_query_time);
            
            if(i == (j - 1)){
                j *= 10;
            }
        }
    }
}


void memory_summary(const string& test_name, const json& info, vector<double>& y_samples, int stream_sizes){
    for(int i = 0, j = 10; i < stream_sizes; i++){
        if (i == (j - 1) or i == ((j - 1) / 2)){
            
            int total_bytes = 0;

            for(auto& it : info[test_name][string("memory_") + to_string(i + 1)].items()){
                for(auto & it2 : it.value()["memory_used"].items()){
                    if ((it2.value()["allocated_type"]) != "[unknown]"){
                        total_bytes += it2.value()["bytes"].get<int>();
                    }
                }
            }

            y_samples.push_back(total_bytes);
            
            if(i == (j - 1)){
                j *= 10;
            }
        }
    }
}

void standard_deviation(const string& test_name, const json& info, vector<double>& y_samples1, vector<double>& y_samples2, int stream_sizes){
    for(int i = 0, j = 10; i < stream_sizes; i++){
        if (i == (j - 1) or i == ((j - 1) / 2)){
            
            double avg_query_time = info[test_name]["time"][string("avg_query_time_") + to_string(i + 1)].get<double>();
            double variance = 0;
            int queries_size = info[test_name]["time"][string("queries_") + to_string(i + 1)].size();

            for(auto& it : info[test_name]["time"][string("queries_") + to_string(i + 1)].items()){
                double query_time = it.value()["query_time"].get<double>();
                int value = it.value()["value"].get<int>();


                variance += (query_time - avg_query_time) * (query_time - avg_query_time);
            }

            variance /= queries_size;
            
            y_samples1.push_back(avg_query_time);
            y_samples2.push_back(sqrt(variance));
                  
            if(i == (j - 1)){
                j *= 10;
            }
        }
    }
}

int main(int argc, char* argv[]){
    deque<string> args = qsbd::process_args(argc, argv);
    //"update_time", "construct_time",
    //, "memory", "query_std_dev"
    vector<string> types_of_plots = { "query_time"};
    vector<string> streams_files;
    string file_prefix = "out/";
    string test_name;
    string output_file;
    int stream_sizes = 10000000;

    if(args.size() != 3){
        DEBUG_ERR("You need to pass the test name and the option, the requested graph and the ouputfile.\n Ex [gk/ kll/ q_digest/ dcs] [-c/ -cw/ -u/ -uw] [depth]");
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
        output_file = file_prefix + it + "_" + args[2] + "_2.dat";

        ofstream out(output_file);

        if(not out.is_open()){
            DEBUG_ERR("Couldn't open output file");

            return -1;
        }

        string file = file_prefix;

        file += (to_string(stream_sizes) + "_" + args[2] + "_2.json");

        ifstream infos(file);

        if (infos.is_open()){
            json all_infos = json::parse(infos);

            vector<int> x_label;
            vector<double> y_label;

            x_label_by_stream_size(x_label, stream_sizes);
            
            if (it == "update_time"){   
                update_time_summary(test_name, all_infos, y_label, stream_sizes);
            }else if(it == "construct_time"){
                construct_time_summary(test_name, all_infos, y_label, stream_sizes);
            }else if(it == "query_time"){
                query_time_summary(test_name, all_infos, y_label, stream_sizes);
            }else if(it == "memory"){
                memory_summary(test_name, all_infos, y_label, stream_sizes);
            }else if(it == "query_std_dev"){
                vector<double> queries_time;
                standard_deviation(test_name, all_infos, queries_time, y_label, stream_sizes);

                ASSERT(y_label.size() == queries_time.size());

                for(int i = 0; i < x_label.size(); i++){
                    out << x_label[i] << " " << queries_time[i] << " " << y_label[i] << endl;
                }
            }else{
                DEBUG_ERR("Option to build y_label not found");
                return -1;
            }

            ASSERT(x_label.size() == y_label.size());

            if(it != "query_std_dev"){
                for(int i = 0; i < x_label.size(); i++){
                    out << x_label[i] << " " << y_label[i] << endl;
                }
            }

            infos.close();   
        }else{
            DEBUG_ERR("Couldn't open file. File:");
            DEBUG_ERR(file);

            return -1;
        }
        

        out.close();
    }

    return 0;
}