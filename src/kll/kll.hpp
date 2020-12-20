#ifndef KLL_H
#define KLL_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"

template<class T>
class kll{
protected:
    
    std::uniform_int_distribution<int> coin;
    std::vector<std::vector<T>> buffers_array;
    int height;
    double buffer_diff;
    double capacity_max;
    double error;
    
public:
    kll(double err);

    static void insert_sorted(std::vector<T> &buffer, const T &elem);
    void compress();
    void update(T elem);
    int query(T elem);
    kll<T> merge(kll<T>& lhs);

    //DEBUG
    void print();
    void set_buffers(std::vector<std::vector<T>>& buffers);
    std::vector<std::vector<T>> get_buffers();
    double get_error();
    double get_capacity();
};

#include "kll.tpp"
#endif