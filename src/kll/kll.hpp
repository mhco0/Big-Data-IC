#ifndef KLL_H
#define KLL_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"

template<class T>
class kll{

private:
    void insert_sorted(std::vector<T> &buffer, const T &elem);

    std::uniform_int_distribution<int> coin;
    std::vector<std::vector<T>> buffers_array;
    int height;
    double buffer_diff;
    double capacity_max;
    double error;

public:
    kll(double err);

    void compress();
    void update(T elem);
    int query(T elem);
    void print();
};

#include "kll.tpp"
#endif