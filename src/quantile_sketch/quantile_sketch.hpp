#ifndef QUANTILE_SKETCH
#define QUANTILE_SKETCH

template<class type>
class quantile_sketch{
private:
    // IDEA : change query to be relative to quantile after implements quantile_quadtree
    // IDEA : put cdf method to return Pr[X < value] = [rank(x) / # elements]
public: 
    virtual void init(){}
    virtual void update(type elem){}
    virtual void update(type elem, int weight){}
    virtual type quantile(double quant){type dummy; return dummy;}
    virtual int query(type elem){return 0;}
    virtual quantile_sketch<type>* merge(const quantile_sketch<type>& rhs) = 0;
};

#endif