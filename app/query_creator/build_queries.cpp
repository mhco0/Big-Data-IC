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
    string output_path = "query/";
    string temp_file = "config_temp.json";
    vector<double> bounds = {0.0, 0.0, 1280.0, 720.0};
    int initial_value = 0;
    int final_value = 10000;
    int step = 100;
    int stream_sizes = 10000000;

    json config = json::object();

    config["queries"]["rank"]["initial_value"] = initial_value;
    config["queries"]["rank"]["step"] = step;

    config["search_bound"] = bounds;

    for(int i = stream_sizes; i <= stream_sizes; i *= 10){
        final_value = min(i, 10000);
        config["queries"]["rank"]["final_value"] = final_value;

        string data_file_name = "query_" + to_string(i) + ".json";
        data_file_name = output_path + data_file_name;
        
        ofstream temp_config(temp_file);

        if(not temp_config.is_open()){
            DEBUG_ERR("Couldn't open temporary file.");
            return -1;
        }

        temp_config << config.dump(4);
        temp_config.close();

        if (system((string("./bin/query_creator config_temp.json ") + data_file_name).c_str()) != 0){
            DEBUG_ERR("Some Error happen in stream creator");
            return -1;
        }

    }

    if(remove(temp_file.c_str()) != 0){
        DEBUG_ERR("ERROR. Couldn't remove temporary file");
        return -1;
    }

    return 0;
}