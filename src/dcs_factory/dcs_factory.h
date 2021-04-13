#ifndef QSBD_DCS_FACTORY_H
#define QSBD_DCS_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../dcs/dcs.h"

namespace qsbd{
    class dcs_factory : public sketch_factory<int>{
    private:
        double error;
        int universe;
        std::vector<count_sketch> estimators;
    public:
        dcs_factory(double err, int univ){
            this->error = err;
            this->universe = univ;

            dcs dummy(this->error, this->universe);

            auto est = dummy.get_estimators();

            for(auto& it : est){
                estimators.push_back(it);
            }
        }

        quantile_sketch<int> * instance() override {
            quantile_sketch<int> * ret  = new dcs(this->error, this->universe, this->estimators);


            return ret;
        }
    };
}

#endif