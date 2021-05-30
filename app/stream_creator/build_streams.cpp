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

int main(){
    //"random_stream_city_with_weight", "random_stream_in_region_with_weight"
    vector<string> methods = {"random_stream_city", "random_stream_in_region"};
    string output_path = "data/";
    string temp_file = "config_temp.json";

    double minx = 0.0, miny = 0.0, maxx = 1280.0, maxy = 720.0;
    int min_v = 0, max_v = 10000;
    int min_w = 1, max_w = 100; 
    int citys = 100;
    int stream_sizes = 100000000;
    double max_radius = 5.0;

    for(auto& method : methods){
        json config = json::object();

        if(method == "random_stream_city"){
            config["method"] = method;
            config["args"] = json::object();
            config["args"]["minx"] = minx;
            config["args"]["miny"] = miny;
            config["args"]["maxx"] = maxx;
            config["args"]["maxy"] = maxy;
            config["args"]["min_v"] = min_v;
            config["args"]["max_v"] = max_v;
            config["args"]["citys"] = citys;
            config["args"]["max_radius"] = max_radius;

            string progress = "0%";
            cout << progress;
            cout.flush();

            for(int i = stream_sizes; i <= stream_sizes; i *= 10){
                config["args"]["vector_size"] = i;

                string data_file_name = "stream_city_" + to_string(i) + ".json";
                data_file_name = output_path + data_file_name;
                
                ofstream temp_config(temp_file);

                if(not temp_config.is_open()){
                    DEBUG_ERR("Couldn't open temporary file.");
                    return -1;
                }

                temp_config << config.dump(4);
                temp_config.close();

                int progress_pct = (int) ((i * 100) / stream_sizes);
        
                cout << string(progress.size(), '\b');
                cout.flush();
                
                progress = to_string(progress_pct) + "%";
                cout << progress;
                cout.flush();

                if (system((string("./bin/stream_creator config_temp.json ") + data_file_name).c_str()) != 0){
                    DEBUG_ERR("Some Error happen in stream creator");
                    return -1;
                }

            }

            cout << string(progress.size(), '\b');
            cout.flush();
            
            cout << "Done!" << endl;
        }else if(method == "random_stream_in_region"){
            config["method"] = method;
            config["args"] = json::object();
            config["args"]["minx"] = minx;
            config["args"]["miny"] = miny;
            config["args"]["maxx"] = maxx;
            config["args"]["maxy"] = maxy;

            string progress = "0%";
            cout << progress;
            cout.flush();

            for(int i = stream_sizes; i <= stream_sizes; i *= 10){
                config["args"]["vector_size"] = i;

                string data_file_name = "stream_uniform_" + to_string(i) + ".json";
                data_file_name = output_path + data_file_name;
                
                ofstream temp_config(temp_file);

                if(not temp_config.is_open()){
                    DEBUG_ERR("Couldn't open temporary file.");
                    return -1;
                }

                temp_config << config.dump(4);
                temp_config.close();

                int progress_pct = (int) ((i * 100) / stream_sizes);
        
                cout << string(progress.size(), '\b');
                cout.flush();
                
                progress = to_string(progress_pct) + "%";
                cout << progress;
                cout.flush();

                if (system((string("./bin/stream_creator config_temp.json ") + data_file_name).c_str()) != 0){
                    DEBUG_ERR("Some Error happen in stream creator");
                    return -1;
                }

            }

            cout << string(progress.size(), '\b');
            cout.flush();
            
            cout << "Done!" << endl;

        }else if(method == "random_stream_in_region_with_weight"){
            config["method"] = method;
            config["args"] = json::object();
            config["args"]["minx"] = minx;
            config["args"]["miny"] = miny;
            config["args"]["maxx"] = maxx;
            config["args"]["maxy"] = maxy;
            config["args"]["min_v"] = min_v;
            config["args"]["max_v"] = max_v;
            config["args"]["min_w"] = min_w;
            config["args"]["max_w"] = max_w;

            string progress = "0%";
            cout << progress;
            cout.flush();

            for(int i = stream_sizes; i <= stream_sizes; i *= 10){
                config["args"]["vector_size"] = i;

                string data_file_name = "stream_uniform_ww_" + to_string(i) + ".json";
                data_file_name = output_path + data_file_name;
                
                ofstream temp_config(temp_file);

                if(not temp_config.is_open()){
                    DEBUG_ERR("Couldn't open temporary file.");
                    return -1;
                }

                temp_config << config.dump(4);
                temp_config.close();

                int progress_pct = (int) ((i * 100) / stream_sizes);
        
                cout << string(progress.size(), '\b');
                cout.flush();
                
                progress = to_string(progress_pct) + "%";
                cout << progress;
                cout.flush();

                if (system((string("./bin/stream_creator config_temp.json ") + data_file_name).c_str()) != 0){
                    DEBUG_ERR("Some Error happen in stream creator");
                    return -1;
                }

            }

            cout << string(progress.size(), '\b');
            cout.flush();
            
            cout << "Done!" << endl;
        }else if(method == "random_stream_city_with_weight"){
            config["method"] = method;
            config["args"] = json::object();
            config["args"]["minx"] = minx;
            config["args"]["miny"] = miny;
            config["args"]["maxx"] = maxx;
            config["args"]["maxy"] = maxy;
            config["args"]["min_v"] = min_v;
            config["args"]["max_v"] = max_v;
            config["args"]["min_w"] = min_w;
            config["args"]["max_w"] = max_w;
            config["args"]["citys"] = citys;
            config["args"]["max_radius"] = max_radius;

            string progress = "0%";
            cout << progress;
            cout.flush();

            for(int i = stream_sizes; i <= stream_sizes; i *= 10){
                config["args"]["vector_size"] = i;

                string data_file_name = "stream_city_ww_" + to_string(i) + ".json";
                data_file_name = output_path + data_file_name;
                
                ofstream temp_config(temp_file);

                if(not temp_config.is_open()){
                    DEBUG_ERR("Couldn't open temporary file.");
                    return -1;
                }

                temp_config << config.dump(4);
                temp_config.close();

                int progress_pct = (int) ((i * 100) / stream_sizes);
        
                cout << string(progress.size(), '\b');
                cout.flush();
                
                progress = to_string(progress_pct) + "%";
                cout << progress;
                cout.flush();

                if (system((string("./bin/stream_creator config_temp.json ") + data_file_name).c_str()) != 0){
                    DEBUG_ERR("Some Error happen in stream creator");
                    return -1;
                }

            }

            cout << string(progress.size(), '\b');
            cout.flush();
            
            cout << "Done!" << endl;
        }
    }

    if(remove(temp_file.c_str()) != 0){
        DEBUG_ERR("ERROR. Couldn't remove temporary file");
        return -1;
    }

    return 0;
}
