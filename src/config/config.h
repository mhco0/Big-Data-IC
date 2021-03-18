#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "../bd_debug/bd_debug.h"
#include "../utils/utils.h"

class configurator{
private:
    enum class options{
        TEST_FILE,
        ARGS,
        DATA_FILE,
        OUTPUT_FILE,
        BD_DEBUG_FLAG,
        UNDEF
    };

    static configurator instance;
    static configurator& get_instance();

    bool debug_enable;
    std::string config_path;
    std::string file_name;
    std::string data_file_name;
    std::string output_file_name;
    std::string test_name;

    std::ifstream config_file;

    std::vector<std::string> args;

    configurator();
    configurator(const configurator& other) = delete;
    configurator& operator=(const configurator& other) = delete;    
    ~configurator();

    configurator::options map(const std::string& command);
    void analisy_command(const std::string& command);
    void internal_load(const std::string& file_name, const std::string& path);
    void set_file_name(const std::string& name);
    void set_path(const std::string& path);
public:
    static void load(const std::string& file_name, const std::string& path = "./");
    static std::vector<std::string> get_args();
    static std::string get_test();
    static std::string get_data_file();
    static std::string get_path();
    static std::string get_output_file();
    static std::string get_file_name();
    static bool debug_mode();
};

#endif