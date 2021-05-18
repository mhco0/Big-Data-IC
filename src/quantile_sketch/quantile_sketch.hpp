#ifndef QSBD_QUANTILE_SKETCH
#define QSBD_QUANTILE_SKETCH

namespace qsbd {
    template<class Type>
    class quantile_sketch{
    private:
        // IDEA : change query to be relative to quantile after implements quantile_quadtree
        // IDEA : put cdf method to return Pr[X < value] = [rank(x) / # elements]
    public:
        virtual ~quantile_sketch(){}
        virtual void init(){}
        virtual void update(Type elem){}
        virtual void update(Type elem, int weight){}
        virtual Type quantile(double quant){return Type();}
        virtual int query(Type elem){return 0;}
        virtual quantile_sketch<Type>* merge(quantile_sketch<Type>& rhs) = 0;
        virtual void inner_merge(quantile_sketch<Type>& rhs) = 0;
    };
}

#endif