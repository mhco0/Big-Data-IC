#ifndef KLL_FACTORY_H
#define KLL_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../kll/kll.hpp"

template<class Type>
class kll_factory : public sketch_factory<Type>{
private:
    double error;
public:
    kll_factory(double err){
        this->error = err;
    }

    quantile_sketch<Type> * instance() override {
        quantile_sketch<Type> * ret  = new kll<Type>(this->error);

        return ret;
    }
};

#endif  