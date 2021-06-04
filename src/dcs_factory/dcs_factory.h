#ifndef QSBD_DCS_FACTORY_H
#define QSBD_DCS_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../dcs/dcs.h"

namespace qsbd{
    class dcs_factory : public sketch_factory<int>{
    private:
        double error;
        int universe;
        std::vector<std::vector<std::vector<unsigned long long int>>> hashs_consts;
    public:
        dcs_factory(double err, int univ){
            this->error = err;
            this->universe = univ;

            dcs dummy(this->error, this->universe);

            hashs_consts = std::move(dummy.get_count_sketchs_hash_functions_constants());
        }

        quantile_sketch<int> * instance() override {
            return new dcs(this->error, this->universe, this->hashs_consts);
        }
    };
}

#endif