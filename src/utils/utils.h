#ifndef QSBD_UTILS_H
#define QSBD_UTILS_H
#include <algorithm>
#include <string>
#include <vector>
#include <deque>

namespace qsbd{
    bool isPrime(int n);
    int nextPrime(int N);
    std::vector<std::string> parse_string(const std::string& sample, const std::string& delimiter = " ");
    std::string erase_special_characters(const std::string& sample, const std::string& except = {});
    std::deque<std::string> process_args(int argc, char* argv[]);
    bool ends_with(std::string const& value, std::string const& ending);

    template<class T>
    void insert_sorted(std::vector<T>& vec, const T& data){
        // The vector must be sorted

        typename std::vector<T>::iterator insert_position;

        insert_position = std::lower_bound(vec.begin(), vec.end(), data);
        vec.insert(insert_position, data);

        return;
    }
}

#endif