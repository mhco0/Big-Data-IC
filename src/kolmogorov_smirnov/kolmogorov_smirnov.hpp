#include "../quantile_sketch/quantile_sketch.hpp"
#include "../utils/utils.h"
#include "../stream_maker/stream_maker.h"
#include "../qsbd_debug/qsbd_debug.h"
#include <random>

namespace qsbd{

    template<class T>
    double kolmogov_smirnov(const quantile_sketch<T>& lhs, const quantile_sketch<T>& rhs, const T& start, const T& end, const T& step){
        double max_dist = std::numeric_limits<double>::min();

        for(T i = start; i < end; i += step){
            double distance = fabs(lhs.cdf(i) - rhs.cdf(i));
            max_dist = std::max(max_dist, distance);
        }
        
        return max_dist;
    }

    int rank_from_samples(const std::vector<int>& sorted_vector, const int& value);

    std::vector<double> cdf_from_samples(std::vector<int> samples, const int& start, const int& end, const int& step);

    std::pair<double, std::vector<double>> distributions_ks(const std::vector<int>& lhs_samples, const std::vector<int>& rhs_samples, const int& start, const int& end, const int& step);
}