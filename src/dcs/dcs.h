#ifndef QSBD_DCS_H
#define QSBD_DCS_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../count_sketch/count_sketch.h"
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd{

    class dcs_factory;

    class dcs : public quantile_sketch<int>{
    private: 
        friend class dcs_factory;

        int universe;
        double error;
        // for initialize count sketch
        int total_weight;
        int w, d;
        int s;
        int lvls;

        std::vector<std::vector<int>> frequency_counters;
        std::vector<count_sketch> estimators;

        void set_params(double err, int univ);
    public:
        dcs(double err, int univ);
        dcs(double err, int univ, const std::vector<std::vector<std::vector<unsigned long long int>>>& hashs_consts);
        ~dcs();

        void update(int x, int weight) override;
        int query(int x) override;
        int quantile(double quant) override;

        quantile_sketch<int> * merge(quantile_sketch<int>& rhs) override;
        void inner_merge(quantile_sketch<int>& rhs) override;

        std::vector<count_sketch> get_estimators() const;
        int get_tree_lvl() const;
        int get_w() const;
        int get_d() const;
        int get_s() const;
        double get_error() const;
        int get_universe() const;
        int get_total_weight() const;
        std::vector<std::vector<std::vector<unsigned long long int>>> get_count_sketchs_hash_functions_constants() const;
    };
}
#endif