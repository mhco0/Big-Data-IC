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

void generate_stream(const json& config, ofstream& out){
    string method = config["method"].get<string>();
    json temp;

    if (method == "random_int_stream"){
        int vector_size = config["args"]["vector_size"].get<int>();
        int min_v = config["args"]["min_v"].get<int>();
        int max_v = config["args"]["max_v"].get<int>();

        auto stream = qsbd::stream_maker::random_int_stream(vector_size, min_v, max_v);
        temp["stream"] = stream;
    }else if (method == "random_int_stream_with_weigth"){
        int vector_size = config["args"]["vector_size"].get<int>();
        int min_v = config["args"]["min_v"].get<int>();
        int max_v = config["args"]["max_v"].get<int>();
        int min_w = config["args"]["min_w"].get<int>();
        int max_w = config["args"]["max_w"].get<int>();

        auto stream = qsbd::stream_maker::random_int_stream_with_weight(vector_size, min_v, max_v, min_w, max_w);
        temp["stream"] = stream;
    }else if(method == "normal_int_stream_with_weight"){
        int vector_size = config["args"]["vector_size"].get<int>();
        double mean = config["args"]["mean"].get<double>();
        double stddev = config["args"]["stddev"].get<double>();
        int min_w = config["args"]["min_w"].get<int>();
        int max_w = config["args"]["max_w"].get<int>();

        auto stream = qsbd::stream_maker::normal_int_stream_with_weight(vector_size, mean, stddev, min_w, max_w);
        temp["stream"] = stream;
    }else if(method == "random_stream_in_region"){
        int vector_size = config["args"]["vector_size"].get<int>();
        double minx = config["args"]["minx"].get<double>();
        double miny = config["args"]["miny"].get<double>();
        double maxx = config["args"]["maxx"].get<double>();
        double maxy = config["args"]["maxy"].get<double>();
	int min_v = config["args"]["min_v"].get<int>();
	int max_v = config["args"]["max_v"].get<int>();

        auto stream = qsbd::stream_maker::random_stream_in_region(vector_size, minx, miny, maxx, maxy, min_v, max_v);
        temp["stream"] = stream;
    }else if(method == "random_stream_in_region_with_weight"){
        int vector_size = config["args"]["vector_size"].get<int>();
        double minx = config["args"]["minx"].get<double>();
        double miny = config["args"]["miny"].get<double>();
        double maxx = config["args"]["maxx"].get<double>();
        double maxy = config["args"]["maxy"].get<double>();
        int min_v = config["args"]["min_v"].get<int>();
        int max_v = config["args"]["max_v"].get<int>();
        int min_w = config["args"]["min_w"].get<int>();
        int max_w = config["args"]["max_w"].get<int>();

        auto stream = qsbd::stream_maker::random_stream_in_region_with_weight(vector_size, minx, miny, maxx, maxy, min_v, max_v, min_w, max_w);
        
        temp["stream"] = stream;
    }else if(method == "random_stream_city"){
        int vector_size = config["args"]["vector_size"].get<int>();
        double minx = config["args"]["minx"].get<double>();
        double miny = config["args"]["miny"].get<double>();
        double maxx = config["args"]["maxx"].get<double>();
        double maxy = config["args"]["maxy"].get<double>();
        int min_v = config["args"]["min_v"].get<int>();
        int max_v = config["args"]["max_v"].get<int>();
        int citys = config["args"]["citys"].get<int>();
        double max_radius = config["args"]["max_radius"].get<double>();

        auto stream = qsbd::stream_maker::random_stream_city(vector_size, minx, miny, maxx, maxy, min_v, max_v, citys, max_radius);
        temp["stream"] = stream;
    }else if(method == "random_stream_city_with_weight"){
        int vector_size = config["args"]["vector_size"].get<int>();
        double minx = config["args"]["minx"].get<double>();
        double miny = config["args"]["miny"].get<double>();
        double maxx = config["args"]["maxx"].get<double>();
        double maxy = config["args"]["maxy"].get<double>();
        int min_v = config["args"]["min_v"].get<int>();
        int max_v = config["args"]["max_v"].get<int>();
        int min_w = config["args"]["min_w"].get<int>();
        int max_w = config["args"]["max_w"].get<int>();
        int citys = config["args"]["citys"].get<int>();
        double max_radius = config["args"]["max_radius"].get<double>();

        auto stream = qsbd::stream_maker::random_stream_city_with_weight(vector_size, minx, miny, maxx, maxy, min_v, max_v, min_w, max_w, citys, max_radius);
        temp["stream"] = stream;
    }else{
        DEBUG_ERR("Method not supported");
    }

    out << temp.dump(4);
    return;
}

int main(int argc, char* argv[]){
    deque<string> args = qsbd::process_args(argc, argv);

    if (args.size() != 2) {
        DEBUG_ERR("You need to pass one json file configuration and one output file json.");
        return -1;
    }

    if (not qsbd::ends_with(args[0], ".json")){
        DEBUG("The file needs to be a json with the json extension");
    }

    ifstream config_file(args[0]);
    json config = json::parse(config_file);

    if(config_file.is_open()) {
        string output_file = args[1];

        if (not qsbd::ends_with(args[0], ".json")){
            DEBUG("The output file needs to be a json with the json extension");
        }

        qsbd::logger out_log(output_file);

        if(out_log.is_open()){
            generate_stream(config, out_log);
        }else{
            DEBUG_ERR("Coundn't open/create output file");
            return -1;
        }

        config_file.close();
    }else{
        DEBUG_ERR("Couldn't open the config file");
        return -1;
    }

    return 0;
}
