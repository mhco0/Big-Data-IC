#ifndef QSBD_SKETCH_FACTORY
#define QSBD_SKETCH_FACTORY
#include "../quantile_sketch/quantile_sketch.hpp"

namespace qsbd{
    template<class Type>
    class sketch_factory{
    private:
    public:
        virtual quantile_sketch<Type> * instance() = 0;    
    };  
}
#endif