#include "kolmogorov_smirnov.hpp"

namespace qsbd{

    int rs2(const std::vector<int>& sorted_vector, const int& value){
        size_t left = 0;
        size_t right = sorted_vector.size() - 1;
        int rank = 0;
        size_t middle = (left + right) / 2;

        while(left < right){
            if (value == sorted_vector[middle]){
                break;
            }

            if (value > sorted_vector[middle]) left = middle + 1;
            if (value < sorted_vector[middle]) right = middle - 1;

            middle = (left + right) / 2;
        }

        rank = middle;

        return rank;
    }

    int rank_from_samples(const std::vector<int>& sorted_vector, const int& value){
        int rank = 0;

        for(size_t i = 0; i < sorted_vector.size(); i++){
            if (sorted_vector[i] < value) rank++;
            else break;
        }

        return rank;
    }

    std::vector<double> cdf_from_samples(std::vector<int> samples, const int& start, const int& end, const int& step){
        ASSERT(samples.size() > 0);
        
        std::vector<double> cdf;

        std::sort(samples.begin(), samples.end());

        for(int i = start; i < end; i += step){
            int rank = rank_from_samples(samples, i);

            double probability = rank / (double) samples.size();

            cdf.push_back(probability);
        }

        return cdf;
    }


    std::pair<double, std::vector<double>> distributions_ks(const std::vector<int>& lhs_samples, const std::vector<int>& rhs_samples, const int& start, const int& end, const int& step){
        std::vector<double> lhs_cdf = cdf_from_samples(lhs_samples, start, end, step);
        std::vector<double> rhs_cdf = cdf_from_samples(rhs_samples, start, end, step);

        double max_dist = std::numeric_limits<double>::min();
        std::vector<double> distances;

        for(size_t i = 0; i < lhs_cdf.size(); i++){
            double distance = fabs(lhs_cdf[i] - rhs_cdf[i]);
            distances.push_back(distance);
            max_dist = std::max(max_dist, distance);
        } 

        return {max_dist, distances};
    }
}