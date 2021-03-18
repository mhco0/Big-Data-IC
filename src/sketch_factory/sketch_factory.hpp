#ifndef SKETCH_FACTORY
#define SKETCH_FACTORY
#include "../quantile_sketch/quantile_sketch.hpp"

template<class Type>
class sketch_factory{
private:
public:
    virtual quantile_sketch<Type> * instance() = 0;    
};

#endif