#include "config.h"
using namespace std;

namespace qsbd {
    configurator::configurator(){
        debug_enable = true;
    }

    configurator::~configurator(){
        if(config_file.is_open()) config_file.close();
    }

    configurator configurator::instance;

    configurator& configurator::get_instance(){
        return instance;
    }

    void configurator::set_file_name(const string& name){
        this->file_name = name;
    }

    void configurator::set_path(const string& path){
        this->config_path = path;
    }

    configurator::options configurator::map(const string& command){
        if(not command.rfind("args=", 0)) return options::ARGS;
        else if(not command.rfind("test=", 0)) return options::TEST_FILE;
        else if(not command.rfind("output=", 0)) return options::OUTPUT_FILE;
        else if(not command.rfind("data=", 0)) return options::DATA_FILE;
        else if(not command.rfind("debug=", 0)) return options::BD_DEBUG_FLAG;
        else return options::UNDEF;
    }

    void configurator::analisy_command(const string& command){
        

        switch (map(command)){
            case options::ARGS:{
                string values = command.substr(5);

                values = erase_special_characters(values, " .");

                args = parse_string(values);
                break;
            }
            case options::TEST_FILE:{
                test_name = erase_special_characters(command.substr(5), "_.");
                break;
            }
            case options::OUTPUT_FILE:{
                output_file_name = erase_special_characters(command.substr(7), "_.");
                break;
            }
            case options::DATA_FILE:{
                data_file_name = erase_special_characters(command.substr(5), "_.");
                break;
            }
            case options::BD_DEBUG_FLAG:{

                string dbo = erase_special_characters(command.substr(6));

                if(dbo == "false" or dbo == "0") debug_enable = false;

                break;
            }
            default:
                break;
        }

        return;
    }

    void configurator::internal_load(const string& file_name, const string& path){
        set_file_name(file_name);
        set_path(path);

        config_file.open(path + file_name);

        if(config_file.is_open()){
            if(args.size() > 0) args.clear();

            string command;

            while(getline(config_file, command)){
                analisy_command(command);
            }
        }else{
            DEBUG_ERR("Couldn't load config file, returning...");
        }

        return;
    }

    void configurator::load(const string& file_name, const string& path){
        get_instance().internal_load(file_name, path);
    }

    vector<string> configurator::get_args(){
        return get_instance().args; 
    }

    string configurator::get_test(){
        return get_instance().test_name; 
    }

    string configurator::get_data_file(){
        return get_instance().data_file_name; 
    }

    string configurator::get_path(){
        return get_instance().config_path; 
    }

    string configurator::get_output_file(){
        return get_instance().output_file_name; 
    }

    string configurator::get_file_name(){
        return get_instance().file_name; 
    }

    bool configurator::debug_mode(){
        return get_instance().debug_enable;
    }
}