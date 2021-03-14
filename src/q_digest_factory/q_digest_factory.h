#ifndef Q_DIGEST_FACTORY_H
#define Q_DIGEST_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../q_digest/q_digest.h"

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
        quantile_sketch<int> * ret  = new q_digest(this->error, this->universe);

        return ret;
    }
};

#endif