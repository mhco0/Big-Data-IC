#ifndef QSBD_GK_FACTORY_H
#define QSBD_GK_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../gk/gk.hpp"

namespace qsbd {
    template<class Type>
    class gk_factory : public sketch_factory<Type>{
    private:
        double error;
    public:
        gk_factory(double err){
            this->error = err;
        }

        quantile_sketch<Type> * instance() override {
            return new gk<Type>(this->error);
        }
    };
}


#endif