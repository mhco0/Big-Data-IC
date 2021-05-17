#ifndef QSBD_Q_DIGEST_FACTORY_H
#define QSBD_Q_DIGEST_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../q_digest/q_digest.h"

namespace qsbd{
    class q_digest_factory : public sketch_factory<int>{
    private:
        double error;
        int universe;
    public:
        q_digest_factory(double err, int uni){
            this->error = err;
            this->universe = uni;
        }

        quantile_sketch<int> * instance() override {
            return new q_digest(this->error, this->universe);
        }
    };
}

#endif