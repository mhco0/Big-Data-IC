/**
 * @file kolmogorov_smirnov.hpp
 * @brief QSBD Kolmogorov Smirnov utility functions module
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../utils/utils.h"
#include "../stream_maker/stream_maker.h"
#include "../qsbd_debug/qsbd_debug.h"
#include <random>

namespace qsbd{

    /**
	 * @brief This function calculate the Kolmogorov Smirnov distance between two quantile sketchs in a interval.
	 * 
	 * The Kolmogorov Smirnov distance is defined as the maximum distance between two cdf functions.
	 * 
	 * @tparam T The type of quantile sketch.
	 * @param lhs The left hand side sketch.
     * @param rhs The right hand side sketch.
     * @param start The start of the range for the cdf query.
     * @param end The end of the range for the cdf query.
     * @param step The step of the range for the cdf query.
	 * @return The Kolmogorov Smirnov distance
	 * 
     * @see quantile_sketch\<T\>
	*/
    template<class T>
    double kolmogorov_smirnov(const quantile_sketch<T>& lhs, const quantile_sketch<T>& rhs, const T& start, const T& end, const T& step){
        double max_dist = std::numeric_limits<double>::min();

        for(T i = start; i < end; i += step){
            double distance = fabs(lhs.cdf(i) - rhs.cdf(i));
            max_dist = std::max(max_dist, distance);
        }
        
        return max_dist;
    }

    /**
     * @brief This function return the rank for some @p value in the @p sorted_vector .
     *
     * The rank here is defined as the sum of the number of values less than @p value
     *
     * @param sorted_vector A vector of values.
     * @param value The value to be queried.
     * @return The rank for @p value .
     *
     * @warning
     * The vector must be sorted
    */
    int rank_from_samples(const std::vector<int>& sorted_vector, const int& value);

    /**
     * @brief This function returns the cdf values for some vector of @p samples .
     *
     * This function calculates all cdf values of some vector @p samples quering points in the range [ @p start , @p end ] with step @p step . 
     *
     * @param samples A vector of samples.
     * @param start The start of the values to search in the cdf.
     * @param end The end of the values to search in the cdf.
     * @param step The step between the start and the end.
     * @return A vector of cdf values in [start, end, step].
    */
    std::vector<double> cdf_from_samples(std::vector<int> samples, const int& start, const int& end, const int& step);

    /**
     * @brief This function calculates the Kolmogorov Smirnov distance between two samples vectors.
     *
     * This function calculates all diferences between the cdf of two vectors.
     *
     * @param lhs_samples  The left hand side vector.
     * @param rhs_samples  The right hand side vector.
     * @param start The start of the values to search in the cdf.
     * @param end The end of the values to search in the cdf.
     * @param step The step between the start and the end.
     * @return A pair of values. The first value is the Kolmogorov Smirnov distance between the two vectors and the second value is all the distances between the two cdfs.
    */
    std::pair<double, std::vector<double>> distributions_ks(const std::vector<int>& lhs_samples, const std::vector<int>& rhs_samples, const int& start, const int& end, const int& step);
}