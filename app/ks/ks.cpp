#include <kolmogorov_smirnov/kolmogorov_smirnov.hpp>
#include <kll/kll.hpp>
#include <gk/gk.hpp>
#include <q_digest/q_digest.h>
#include <dcs/dcs.h>
#include <iostream>
#define MIN_VALUE_IN_STREAM 0
#define MAX_VALUE_IN_STREAM 1023
using namespace std;
using namespace qsbd;
using namespace stream_maker;

struct ks_test_result_t{
    vector<int> distribution;
    vector<double> cdf_distribution;
    vector<double> sketch_cdf_distribution;
    vector<double> distances;
    double max_distance;
};

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

ks_test_result_t test_ks_with_kll(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error){
    vector<double> distances;
    vector<double> sketch_cdfs;
    double max_dist = numeric_limits<double>::min();
    kll<int> sketch(error);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        ASSERT(distribution[i] >= 0);
        sketch.update(distribution[i]);
    }

    vector<double> cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end; i += step, j++){
        double sketch_cdf = sketch.cdf(i);
        double distance = fabs(cdf_samples[j] - sketch_cdf);
        
        sketch_cdfs.push_back(sketch_cdf);
        distances.push_back(distance);
        
        max_dist = max(max_dist, distance);
    }
    
	return {.distribution = distribution, .cdf_distribution = cdf_samples, .sketch_cdf_distribution = sketch_cdfs, .distances = distances, .max_distance = max_dist};
}

ks_test_result_t test_ks_with_dcs(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error, const int& universe){
    vector<double> distances;
    vector<double> sketch_cdfs;
    double max_dist = numeric_limits<double>::min();
    dcs sketch(error, universe);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        ASSERT(distribution[i] >= 0 and distribution[i] < universe);
        sketch.update(distribution[i], 1);
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
        double sketch_cdf = sketch.cdf(i);
        double distance = fabs(cdf_samples[j] - sketch_cdf);

        sketch_cdfs.push_back(sketch_cdf);
        distances.push_back(distance);

        max_dist = max(max_dist, distance);
    }

	return {.distribution = distribution, .cdf_distribution = cdf_samples, .sketch_cdf_distribution = sketch_cdfs, .distances = distances, .max_distance = max_dist};
}

ks_test_result_t test_ks_with_q_digest(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error, const int& universe){
    vector<double> distances;
    vector<double> sketch_cdfs;
    double max_dist = numeric_limits<double>::min();
    q_digest sketch(error, universe);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        ASSERT(distribution[i] >= 0 and distribution[i] < universe);
        sketch.update(distribution[i], 1);
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
        double sketch_cdf = sketch.cdf(i);
        double distance = fabs(cdf_samples[j] - sketch_cdf);
        
        sketch_cdfs.push_back(sketch_cdf);
        distances.push_back(distance);

        max_dist = max(max_dist, distance);
    }
    
	return {.distribution = distribution, .cdf_distribution = cdf_samples, .sketch_cdf_distribution = sketch_cdfs, .distances = distances, .max_distance = max_dist};
}

ks_test_result_t test_ks_with_gk(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error){
    vector<double> distances;
    vector<double> sketch_cdfs;
    double max_dist = numeric_limits<double>::min();
    gk<int> sketch(error);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        ASSERT(distribution[i] >= 0);
        sketch.update(distribution[i]);
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
        double sketch_cdf = sketch.cdf(i);
        double distance = fabs(cdf_samples[j] - sketch_cdf);
        
        sketch_cdfs.push_back(sketch_cdf);
        distances.push_back(distance);
        
        max_dist = max(max_dist, distance);
    }

	return {.distribution = distribution, .cdf_distribution = cdf_samples, .sketch_cdf_distribution = sketch_cdfs, .distances = distances, .max_distance = max_dist};
}

void help(){
    cout << "This program shows some results of the test for the kolmogorov-smirnov algorithm." << endl;
    cout << "Usage: " << endl;
    cout << "./ks samples start_point end_point step min_mean_range max_mean_range min_std_range max_std_range error universe" << endl;
    cout << "samples: The number of samples collected by each distribution" << endl;
    cout << "start_point: The start point to collect in the cdf" << endl;
    cout << "end_point: The end point to collect in the cdf" << endl;
    cout << "step: The step between the end and the start point" << endl;
    cout << "min_mean_range: The minimum mean value for the range of tested values for the mean in the distribution" << endl;
    cout << "max_mean_range: The maximum mean value for the range of tested values for the mean in the distribution" << endl;
    cout << "min_std_range: The minimum std value for the range of tested values for the std in the distribution" << endl;
    cout << "max_std_range: The maximum std value for the range of tested values for the std in the distribution" << endl;
    cout << "error: The epsilon error for each sketch" << endl;
    cout << "universe: The universe of values range for the sketchs that need it" << endl;
}

ostream& operator<<(ostream& out, const ks_test_result_t& res){
    out << "Distribution : ";
    for(size_t i = 0; i < res.distribution.size(); i++){
        out << res.distribution[i] << " \n"[ i == res.distribution.size() - 1];
    }

    out << "CDF : ";
    for(size_t i = 0; i < res.cdf_distribution.size(); i++){
        out << res.cdf_distribution[i] << " \n"[ i == res.cdf_distribution.size() - 1];
    }

    out << "Sketch CDF : ";
    for(size_t i = 0; i < res.sketch_cdf_distribution.size(); i++){
        out << res.sketch_cdf_distribution[i] << " \n"[ i == res.sketch_cdf_distribution.size() - 1];
    }

    out << "Distances : ";
    for(size_t i = 0; i < res.distances.size(); i++){
        out << res.distances[i] << " \n"[ i == res.distances.size() - 1];
    }

    out << "Max distance : " << res.max_distance << "\n";

    return out;
}

int main(int argc, char* argv[]){
    deque<string> args = process_args(argc, argv);

    if (args.size() != 10){
        help();
        return -1;
    }

    int samples = stoi(args[0]);
    int start_point = stoi(args[1]);
    int end_point = stoi(args[2]);
    int step = stoi(args[3]);
    double min_mean_range = stod(args[4]);
    double max_mean_range = stod(args[5]);
    double min_std_range = stod(args[6]);
    double max_std_range = stod(args[7]);
    double error = stod(args[8]);
    double universe = stoi(args[9]);
     
    //test_normal_distribution(10000, 0, 1000, 1, 700.0, 200.0);
    //test_distinct_normal_distribution(10000, 0, 1000, 1, 300.0, 100.0, 700.0, 100.0);
    ks_test_result_t res = test_ks_with_kll(samples, start_point, end_point, step, max_mean_range, max_std_range, error);
    cout << "kll : \n" << res << endl;
    
    res = test_ks_with_dcs(samples, start_point, end_point, step, max_mean_range, max_std_range, error, universe);
    cout << "dcs : \n" << res << endl;

    res = test_ks_with_q_digest(samples, start_point, end_point, step, max_mean_range, max_std_range, error, universe);
    cout << "q_digest : \n" << res << endl;
    
    res = test_ks_with_gk(samples, start_point, end_point, step, max_mean_range, max_std_range, error);
    cout << "gk : \n" << res << endl;

    return 0;
}
