#ifndef QSBD_LOG_H
#define QSBD_LOG_H
#include "../qsbd_debug/qsbd_debug.h"
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

namespace qsbd{
    class logger : public std::ofstream{
    private:
        std::ofstream out;
    public:
        logger(){}
        logger(const std::string& file_name);
        ~logger();
    };

    template<class T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& data){
        os << "[";
        for (int i = 0; i < data.size(); i++) {
            os << data[i];
            if (i != data.size() - 1) os << ", ";
        }
        os << "]";

        return os;
    }

    template<class T, class U>
    std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& data){
        os << "(" << data.first << ", " << data.second << ")";

        return os;
    }

    template<class T, class U>
    std::ostream& operator<<(std::ostream& os, const std::map<T, U>& data){
        os << "{";
        for(auto& it : data){
            os << it.first << ": " << it.second << ", ";
        }
        os << "}";

        return os;
    }
}

#endif