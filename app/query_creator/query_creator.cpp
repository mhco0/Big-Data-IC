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
    int queries_number = stream_file["queries"].get<int>();
    double bounds[4] = {0, 0, 0, 0};
    for(auto& it : stream_file["search_bound"].items()){
        bounds[stoi(it.key())] = it.value();
    }

    json temp;

    temp["queries"] = json::array();

    for(int i = 0; i < queries_number; i++){
        auto query_bound = qsbd::stream_maker::random_rectangle_in_region(bounds[0], bounds[1], bounds[2], bounds[3]);
        vector<double> rect;

        rect.push_back(query_bound.first.first);
        rect.push_back(query_bound.first.second);
        rect.push_back(query_bound.second.first);
        rect.push_back(query_bound.second.second);

        temp["queries"].push_back(rect);

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