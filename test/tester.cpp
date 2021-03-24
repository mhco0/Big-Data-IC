#include <config/config.h>
#include <utils/utils.h>
#include <qsbd_debug/qsbd_debug.h>
#include <cstdlib>
using namespace std;
using namespace qsbd;

int main(int argc, char * argv[]){
    deque<string> args = process_args(argc, argv);

    if (args.size() == 2){
        configurator::load(args[0], args[1]);
    }else if(args.size() == 1){
        configurator::load(args[0]);
    }else{
        DEBUG_ERR("You need to pass the configuration file_name");
        return -1;
    }

    string compile = "cmake --build . --target ";

    string command = "./bin/";
    command += configurator::get_test();
    compile += configurator::get_test();
    auto sub_program_args = configurator::get_args();

    for(auto& sub_arg : sub_program_args){
        command += " ";
        command += sub_arg;
    }

    command += " ";
    command += configurator::get_data_file();
    command += " ";
    command += configurator::get_output_file();

    if (not configurator::debug_mode()) command += " -DDISABLE_DEBUG";


    cout << "compiling... " << compile << endl;
    system(compile.c_str());
    cout << "executing... " << command << endl;
    system(command.c_str());

    return 0;
}