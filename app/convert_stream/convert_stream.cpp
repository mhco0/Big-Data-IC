#include <stream_maker/stream_maker.cpp>
#include <nlohmann/json.hpp>
#include <logger/logger.h>
#include <utils/utils.h>
#include <qsbd_debug/qsbd_debug.h>
#include <map>
#include <iostream>
#include <fstream>
#include <functional>
using namespace std;
using json = nlohmann::json;

json remove_weight_from_stream(const json& stream_json){
    vector<pair<pair<int, int>, pair<double, double>>> stream = stream_json["stream"].get<vector<pair<pair<int, int>, pair<double, double>>>>();
    vector<pair<int, pair<double, double>>> converted_stream;
    json converted_json = json::object();

    for(auto& it : stream){
        while(it.first.second--){
            converted_stream.push_back({it.first.first, {it.second}});
        }
    }

    converted_json["stream"] = converted_stream;

    return converted_json;
}

json add_weight_to_stream(const json& stream_json){
    vector<pair<int, pair<double, double>>> stream = stream_json["stream"].get<vector<pair<int, pair<double, double>>>>();
    vector<pair<pair<int, int>, pair<double, double>>> converted_stream;
    json converted_json = json::object();

    for(auto& it : stream){
        converted_stream.push_back({{it.first, 1}, {it.second}});
    }

    converted_json["stream"] = converted_stream;

    return converted_json;
}


int main(int argc, char* argv[]){
    deque<string> args = qsbd::process_args(argc, argv);

    if (args.size() != 3) {
        DEBUG_ERR("You need to pass one json file to be converted, the type to convert the stream and the output file json to be the stream converted.");
        return -1;
    }

    if (not qsbd::ends_with(args[0], ".json")){
        DEBUG("The file needs to be a json with the json extension");
    }

    ifstream input_file(args[0]);

    if (not qsbd::ends_with(args[2], ".json")){
        DEBUG("The file needs to be a json with the json extension");
    }

    ofstream output_file(args[2]);
    json stream_json = json::parse(input_file);

    json converted_json;

    if (args[1] == "-wtnw"){
        converted_json = remove_weight_from_stream(stream_json);
    }else if(args[1] == "-nwtw"){
        converted_json = add_weight_to_stream(stream_json);
    }else{
        DEBUG_ERR("invalid convert option, try -wtnw, to transform with weight stream to no weigth stream and -nwtw otherwise");
        return -1;
    }

    output_file << converted_json.dump(4);

    input_file.close();
    output_file.close();

    return 0;
}