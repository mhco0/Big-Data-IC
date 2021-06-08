#ifndef QSBD_UTILS_H
#define QSBD_UTILS_H
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <stdint.h>
#include <cmath>

namespace qsbd{
    bool isPrime(int n);
    int nextPrime(int N);
    std::vector<std::string> parse_string(const std::string& sample, const std::string& delimiter = " ");
    std::string erase_special_characters(const std::string& sample, const std::string& except = {});
    std::deque<std::string> process_args(int argc, char* argv[]);
    bool ends_with(std::string const& value, std::string const& ending);
    int map_coord(double coord, double min_coord, double max_coord, int resolution);
    double map_coord_inv(int coord, double min_coord, double max_coord, int resolution);

    template<class T>
    void insert_sorted(std::vector<T>& vec, const T& data){
        // The vector must be sorted

        typename std::vector<T>::iterator insert_position;

        insert_position = std::lower_bound(vec.begin(), vec.end(), data);
        vec.insert(insert_position, data);

        return;
    }

    template<class T>
    double lp_norm(std::vector<T>& vec, const int& norm){
        double sum = 0.0;

        for(auto& it : vec){
            sum += pow(it, norm);
        }

        return pow(sum, 1.0 / norm);
    }
}

#endif