#ifndef DCS_FACTORY_H
#define DCS_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../dcs/dcs.h"

class dcs_factory : public sketch_factory<int>{
private:
    double error;
    int universe;
public:
    dcs_factory(double err, int univ){
        this->error = err;
        this->universe = univ;
    }

    quantile_sketch<int> * instance() override {
        quantile_sketch<int> * ret  = new dcs(this->error, this->universe);

        return ret;
    }
};


#endif