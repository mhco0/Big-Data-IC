#ifndef DCS_H
#define DCS_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../count_sketch/count_sketch.h"

class dcs{
private: 
    int universe;
    double error;
    // for initialize count sketch
    int total_weight;
    int w, d;
    int s;
    int lvls;

    std::vector<std::vector<int>> frequency_counters;
    std::vector<count_sketch> estimators;

public:
    dcs(double err, int universe);
    ~dcs();

    void update(int x, int weight);
    int query(int x);
    int quantile(double quant);

    int get_tree_lvl() const;
    int get_w() const;
    int get_d() const;
    int get_s() const;
    double get_error() const;
    int get_universe() const;
    int get_total_weight() const;
};
#endif