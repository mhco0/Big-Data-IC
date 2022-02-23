#include <kolmogorov_smirnov/kolmogorov_smirnov.hpp>
#include <kll/kll.hpp>
#include <gk/gk.hpp>
#include <q_digest/q_digest.h>
#include <dcs/dcs.h>
#include <iostream>
using namespace std;
using namespace qsbd;
using namespace stream_maker;

deque<string> g_args;

pair<double, vector<double>> test_normal_distribution(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std){
    vector<int> lhs = normal_int_stream(samples, mean, std);
    vector<int> rhs = normal_int_stream(samples, mean, std);

   	return distributions_ks(lhs, rhs, start, end, step);
}

pair<double, vector<double>> test_distinct_normal_distribution(const int& samples, const int& start, const int& end, const int& step, const double& mean_lhs, const double& std_lhs, const double& mean_rhs, const double& std_rhs){
    vector<int> lhs = normal_int_stream(samples, mean_lhs, std_lhs);
    vector<int> rhs = normal_int_stream(samples, mean_rhs, std_rhs);

    return distributions_ks(lhs, rhs, start, end, step);
}

pair<double, vector<double>> test_ks_with_kll(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error){
    vector<double> distances;
    double max_dist = numeric_limits<double>::min();
    kll<int> sketch(error);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        sketch.update(max(0, min(1023, distribution[i]))); // tries to take away this min and max bounds
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end; i += step, j++){
        double distance = fabs(cdf_samples[j] - sketch.cdf(i));
        distances.push_back(distance);
        max_dist = max(max_dist, distance);
    }
    
	return {max_dist, distances};
}

pair<double, vector<double>> test_ks_with_dcs(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error, const int& universe){
    vector<double> distances;
    double max_dist = numeric_limits<double>::min();
    dcs sketch(error, universe);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        sketch.update(max(0, min(1023, distribution[i])), 1);
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
        double distance = fabs(cdf_samples[j] - sketch.cdf(i));
        distances.push_back(distance);
        max_dist = max(max_dist, distance);
    }

	return {max_dist, distances};
}

pair<double, vector<double>> test_ks_with_q_digest(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error, const int& universe){
    vector<double> distances;
    double max_dist = numeric_limits<double>::min();
    q_digest sketch(error, universe);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        sketch.update(max(0, min(1023, distribution[i])), 1);
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
        double distance = fabs(cdf_samples[j] - sketch.cdf(i));
        distances.push_back(distance);
        max_dist = max(max_dist, distance);
    }
    
	return {max_dist, distances};
}

pair<double, vector<double>> test_ks_with_gk(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error){
    vector<double> distances;
    double max_dist = numeric_limits<double>::min();
    gk<int> sketch(error);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        sketch.update(max(0, min(1023, distribution[i])));
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
        double distance = fabs(cdf_samples[j] - sketch.cdf(i));
        distances.push_back(distance);
        max_dist = max(max_dist, distance);
    }

	return {max_dist, distances};
}

int main(int argc, char* argv[]){
    test_normal_distribution(10000, 0, 1000, 1, 700.0, 200.0);
    test_distinct_normal_distribution(10000, 0, 1000, 1, 300.0, 100.0, 700.0, 100.0);
    test_ks_with_kll(10000, 0, 1000, 1, 700.0, 100.0, 0.003);
    test_ks_with_dcs(10000, 0, 1000, 1, 700.0, 100.0, 0.003, 1024);
    test_ks_with_q_digest(10000, 0, 1000, 1, 700.0, 100.0, 0.003, 1024);
    test_ks_with_gk(10000, 0, 1000, 1, 700.0, 100.0, 0.003);

    return 0;
}
