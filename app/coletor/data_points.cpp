#include <nlohmann/json.hpp>
#include <utils/utils.h>
#include <qsbd_debug/qsbd_debug.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <cstdlib>
using namespace std;
using namespace qsbd;
using json = nlohmann::json;

void write_out_from_json(json& stream_json, ofstream& out, bool with_weight){
    if(not with_weight){
        vector<pair<int, pair<double, double>>> stream = stream_json["stream"];
        
        for(auto& it : stream){
            out << it.second.first << " " << it.second.second << " " << it.first << endl;
        }
    }else{
        vector<pair<pair<int, int>, pair<double, double>>> stream = stream_json["stream"];

        for(auto& it : stream){
            out << it.second.first << " " << it.second.second << " " << it.first.first << " " << it.first.second << endl;
        }    
    }

    return;
}

/*
    Help usage function
*/
void help(){
    cout << "Data Points usage : " << endl;
    cout << "./data_points stream_file stream_type output_file " << endl;
    cout << "stream_file: the path for the stream file to be converted" << endl;
    cout << "stream_type: (-ww | -nw), -ww for streams with weight and -nw otherwise" << endl;
    cout << "output_file : the path for same stream file to be saved as points on gnuplot" << endl;
    cout << "EX: " << endl;
    cout << "./data_points data/stream_uniform_1.json out/stream_uniform_1.dat" << endl;
}

int main(int argc, char* argv[]){
    deque<string> args = process_args(argc, argv);

    if(args.size() != 3){
        DEBUG_ERR("Wrong args size.");
        help();
        return -1;
    }else{ 
        ifstream json_file(args[0]);
        ofstream out(args[2]);

        if(not json_file.is_open() or not out.is_open()){
            DEBUG_ERR("Cound't open one or both files");
            return -1;
        }


        cout << "Parsing Files...";
        cout.flush();

        json stream = json::parse(json_file);

        cout << "Done" << endl;

        write_out_from_json(stream, out, args[1] == "-ww");

        json_file.close();
        out.close();
    }

    return 0;
}