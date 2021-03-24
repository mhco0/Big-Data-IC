#include "logger.h"
using namespace std;

namespace qsbd{
    logger::logger(const string& file_name): ofstream(file_name, ofstream::out){

        if(not this->is_open()){
            DEBUG_ERR("Coundn't open file in logger constructor. file name : ");
            DEBUG_ERR(file_name);
        }
    }

    logger::~logger(){
        if(this->is_open()) this->close();
    }
}