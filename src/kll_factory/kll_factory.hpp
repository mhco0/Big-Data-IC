#ifndef QSBD_KLL_FACTORY_H
#define QSBD_KLL_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../kll/kll.hpp"

namespace qsbd {
    template<class Type>
    class kll_factory : public sketch_factory<Type>{
    private:
        double error;
    public:
        kll_factory(double err){
            this->error = err;
        }

        quantile_sketch<Type> * instance() override {
            return new kll<Type>(this->error);
        }
    };
}

#endif  