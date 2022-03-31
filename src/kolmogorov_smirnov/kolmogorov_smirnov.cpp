#include "kolmogorov_smirnov.hpp"

namespace qsbd{

    #if 0
    int rs2(const std::vector<int>& sorted_vector, const int& value){
        auto low = std::lower_bound(sorted_vector.begin(), sorted_vector.end(), value);
        int rank = (low - sorted_vector.begin()); 
        return rank;
    }
    #endif

    int rank_from_samples(const std::vector<int>& sorted_vector, const int& value){
        #if 0
        int rank = 0;

        for(size_t i = 0; i < sorted_vector.size(); i++){
            if (sorted_vector[i] < value) rank++;
            else break;
        }

        return rank;
        
        #else

        auto low = std::lower_bound(sorted_vector.begin(), sorted_vector.end(), value);
        int rank = (low - sorted_vector.begin()); 
        return rank;

        #endif
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