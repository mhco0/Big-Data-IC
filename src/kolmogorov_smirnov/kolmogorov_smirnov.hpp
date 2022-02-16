#include "../quantile_sketch/quantile_sketch.hpp"
#include "../utils/utils.h"

namespace qsbd{

    template<class T>
    double kolmogov_smirnov(const quantile_sketch<T>& lhs, const quantile_sketch<T>& rhs, const T& start, const T& end, int resolution){
        double max_dist = std::numeric_limits<double>::min();
        T step = (end - start) / (1 << resolution);

        for(T i = start; i < end; i += step){
            T value = ((i - start) * (1 << resolution)) / (end - start);
            double distance = abs(lhs.cdf(value) - rhs.cdf(value));
            max_dist = std::max(max_dist, distance);
        }
        
        return max_dist;
    }
}