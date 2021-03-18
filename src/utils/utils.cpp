#include "utils.h"
using namespace std;

vector<string> parse_string(const string& sample, const string& delimiter){
    vector<string> values;

    string copy_str(sample);

    size_t pos = 0;

    while(pos = copy_str.find(delimiter), pos != string::npos){
        string pre_str = copy_str.substr(0, pos);

        values.push_back(pre_str);

        copy_str.erase(0, pos + delimiter.length());
    }

    values.push_back(copy_str);

    return values;
}

string erase_special_characters(const string& sample, const string& except){
    string copy_str(sample);
    
    copy_str.erase(remove_if(copy_str.begin(), copy_str.end(),
    [&except](char c) {
        bool is_excepiton = false;

        for(auto &it : except){ 
            if(c == it){
                is_excepiton = true;
            }
        }

        return not (isalpha(c) || isdigit(c) || is_excepiton);
    }), 
    copy_str.end());

    return copy_str;
}

deque<string> process_args(int argc, char* argv[]){
	deque<string> args;

	for(int i = 1; i < argc; i++){
		string arg(argv[i]);

		args.push_back(arg);
	}
    
	return args;
}
