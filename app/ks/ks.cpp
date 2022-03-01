#include <kolmogorov_smirnov/kolmogorov_smirnov.hpp>
#include <kll/kll.hpp>
#include <gk/gk.hpp>
#include <q_digest/q_digest.h>
#include <dcs/dcs.h>
#include <iostream>
#include <fstream>
#define MIN_VALUE_IN_STREAM 0
#define MAX_VALUE_IN_STREAM 1023
using namespace std;
using namespace qsbd;
using namespace stream_maker;

struct ks_compair_test_result_t{
    vector<int> distribution_lhs;
    vector<int> distribution_rhs;
    vector<double> cdf_distribution_lhs;
    vector<double> cdf_distribution_rhs;
    vector<double> sketch_cdf_distribution_lhs;
    vector<double> sketch_cdf_distribution_rhs;
    vector<double> distances_distributions;
    vector<double> distances_sketches;
    double max_distance_distributions;
    double max_distance_sketches;
};

struct ks_test_result_t{
    vector<int> distribution;
    vector<double> cdf_distribution;
    vector<double> sketch_cdf_distribution;
    vector<double> distances;
    double max_distance;
};

ks_compair_test_result_t test_compair_normal_distribution_with_kll(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error){
    vector<int> lhs_samples = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);
    vector<int> rhs_samples = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);
    vector<double> distances_distributions;
    vector<double> distances_sketches;
    vector<double> sketch_cdf_distribution_lhs;
    vector<double> sketch_cdf_distribution_rhs;
    double max_dist_distributions = numeric_limits<double>::min();
    double max_dist_sketches = numeric_limits<double>::min();
    kll<int> lhs_sketch(error);
    kll<int> rhs_sketch(error);

   	vector<double> cdf_lhs = cdf_from_samples(lhs_samples, start, end, step);
    vector<double> cdf_rhs = cdf_from_samples(rhs_samples, start, end, step);

    for(size_t i = 0; i < cdf_lhs.size(); i++){
        double distance = fabs(cdf_lhs[i] - cdf_rhs[i]);
        distances_distributions.push_back(distance);
        max_dist_distributions = max(max_dist_distributions, distance);
    }

    for(size_t i = 0; i < lhs_samples.size(); i++){
        ASSERT(lhs_samples[i] >= 0);
        lhs_sketch.update(lhs_samples[i]);
    }

    for(size_t i = 0; i < rhs_samples.size(); i++){
        ASSERT(rhs_samples[i] >= 0);
        rhs_sketch.update(rhs_samples[i]);
    }

    for(int i = start; i < end; i += step){
        double sketch_lhs_cdf = lhs_sketch.cdf(i);
        double sketch_rhs_cdf = rhs_sketch.cdf(i);
        double distance = fabs(sketch_lhs_cdf - sketch_rhs_cdf);
        
        sketch_cdf_distribution_lhs.push_back(sketch_lhs_cdf);
        sketch_cdf_distribution_rhs.push_back(sketch_rhs_cdf);
        distances_sketches.push_back(distance);
        
        max_dist_sketches = max(max_dist_sketches, distance);
    }

    return {.distribution_lhs = lhs_samples, .distribution_rhs = rhs_samples, .cdf_distribution_lhs = cdf_lhs, .cdf_distribution_rhs = cdf_rhs, .sketch_cdf_distribution_lhs = sketch_cdf_distribution_lhs, .sketch_cdf_distribution_rhs = sketch_cdf_distribution_rhs, .distances_distributions = distances_distributions, .distances_sketches = distances_sketches, .max_distance_distributions = max_dist_distributions, .max_distance_sketches = max_dist_sketches};
}

ks_compair_test_result_t test_compair_normal_distribution_with_dcs(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error,  const int& universe){
    vector<int> lhs_samples = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);
    vector<int> rhs_samples = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);
    vector<double> distances_distributions;
    vector<double> distances_sketches;
    vector<double> sketch_cdf_distribution_lhs;
    vector<double> sketch_cdf_distribution_rhs;
    double max_dist_distributions = numeric_limits<double>::min();
    double max_dist_sketches = numeric_limits<double>::min();
    dcs lhs_sketch(error, universe);
    dcs rhs_sketch(error, universe);

   	vector<double> cdf_lhs = cdf_from_samples(lhs_samples, start, end, step);
    vector<double> cdf_rhs = cdf_from_samples(rhs_samples, start, end, step);

    for(size_t i = 0; i < cdf_lhs.size(); i++){
        double distance = fabs(cdf_lhs[i] - cdf_rhs[i]);
        distances_distributions.push_back(distance);
        max_dist_distributions = max(max_dist_distributions, distance);
    }

    for(size_t i = 0; i < lhs_samples.size(); i++){
        ASSERT(lhs_samples[i] >= 0 and lhs_samples[i] < universe);
        lhs_sketch.update(lhs_samples[i], 1);
    }

    for(size_t i = 0; i < rhs_samples.size(); i++){
        ASSERT(rhs_samples[i] >= 0 and rhs_samples[i] < universe);
        rhs_sketch.update(rhs_samples[i], 1);
    }

    for(int i = start; i < end; i += step){
        double sketch_lhs_cdf = lhs_sketch.cdf(i);
        double sketch_rhs_cdf = rhs_sketch.cdf(i);
        double distance = fabs(sketch_lhs_cdf - sketch_rhs_cdf);
        
        sketch_cdf_distribution_lhs.push_back(sketch_lhs_cdf);
        sketch_cdf_distribution_rhs.push_back(sketch_rhs_cdf);
        distances_sketches.push_back(distance);
        
        max_dist_sketches = max(max_dist_sketches, distance);
    }

    return {.distribution_lhs = lhs_samples, .distribution_rhs = rhs_samples, .cdf_distribution_lhs = cdf_lhs, .cdf_distribution_rhs = cdf_rhs, .sketch_cdf_distribution_lhs = sketch_cdf_distribution_lhs, .sketch_cdf_distribution_rhs = sketch_cdf_distribution_rhs, .distances_distributions = distances_distributions, .distances_sketches = distances_sketches, .max_distance_distributions = max_dist_distributions, .max_distance_sketches = max_dist_sketches};
}

ks_compair_test_result_t test_compair_normal_distribution_with_q_digest(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error,  const int& universe){
    vector<int> lhs_samples = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);
    vector<int> rhs_samples = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);
    vector<double> distances_distributions;
    vector<double> distances_sketches;
    vector<double> sketch_cdf_distribution_lhs;
    vector<double> sketch_cdf_distribution_rhs;
    double max_dist_distributions = numeric_limits<double>::min();
    double max_dist_sketches = numeric_limits<double>::min();
    q_digest lhs_sketch(error, universe);
    q_digest rhs_sketch(error, universe);

   	vector<double> cdf_lhs = cdf_from_samples(lhs_samples, start, end, step);
    vector<double> cdf_rhs = cdf_from_samples(rhs_samples, start, end, step);

    for(size_t i = 0; i < cdf_lhs.size(); i++){
        double distance = fabs(cdf_lhs[i] - cdf_rhs[i]);
        distances_distributions.push_back(distance);
        max_dist_distributions = max(max_dist_distributions, distance);
    }

    for(size_t i = 0; i < lhs_samples.size(); i++){
        ASSERT(lhs_samples[i] >= 0 and lhs_samples[i] < universe);
        lhs_sketch.update(lhs_samples[i], 1);
    }

    for(size_t i = 0; i < rhs_samples.size(); i++){
        ASSERT(rhs_samples[i] >= 0 and rhs_samples[i] < universe);
        rhs_sketch.update(rhs_samples[i], 1);
    }

    for(int i = start; i < end; i += step){
        double sketch_lhs_cdf = lhs_sketch.cdf(i);
        double sketch_rhs_cdf = rhs_sketch.cdf(i);
        double distance = fabs(sketch_lhs_cdf - sketch_rhs_cdf);
        
        sketch_cdf_distribution_lhs.push_back(sketch_lhs_cdf);
        sketch_cdf_distribution_rhs.push_back(sketch_rhs_cdf);
        distances_sketches.push_back(distance);
        
        max_dist_sketches = max(max_dist_sketches, distance);
    }

    return {.distribution_lhs = lhs_samples, .distribution_rhs = rhs_samples, .cdf_distribution_lhs = cdf_lhs, .cdf_distribution_rhs = cdf_rhs, .sketch_cdf_distribution_lhs = sketch_cdf_distribution_lhs, .sketch_cdf_distribution_rhs = sketch_cdf_distribution_rhs, .distances_distributions = distances_distributions, .distances_sketches = distances_sketches, .max_distance_distributions = max_dist_distributions, .max_distance_sketches = max_dist_sketches};
}

ks_compair_test_result_t test_compair_normal_distribution_with_gk(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error){
    vector<int> lhs_samples = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);
    vector<int> rhs_samples = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);
    vector<double> distances_distributions;
    vector<double> distances_sketches;
    vector<double> sketch_cdf_distribution_lhs;
    vector<double> sketch_cdf_distribution_rhs;
    double max_dist_distributions = numeric_limits<double>::min();
    double max_dist_sketches = numeric_limits<double>::min();
    gk<int> lhs_sketch(error);
    gk<int> rhs_sketch(error);

   	vector<double> cdf_lhs = cdf_from_samples(lhs_samples, start, end, step);
    vector<double> cdf_rhs = cdf_from_samples(rhs_samples, start, end, step);

    for(size_t i = 0; i < cdf_lhs.size(); i++){
        double distance = fabs(cdf_lhs[i] - cdf_rhs[i]);
        distances_distributions.push_back(distance);
        max_dist_distributions = max(max_dist_distributions, distance);
    }

    for(size_t i = 0; i < lhs_samples.size(); i++){
        ASSERT(lhs_samples[i] >= 0);
        lhs_sketch.update(lhs_samples[i]);
    }

    for(size_t i = 0; i < rhs_samples.size(); i++){
        ASSERT(rhs_samples[i] >= 0);
        rhs_sketch.update(rhs_samples[i]);
    }

    for(int i = start; i < end; i += step){
        double sketch_lhs_cdf = lhs_sketch.cdf(i);
        double sketch_rhs_cdf = rhs_sketch.cdf(i);
        double distance = fabs(sketch_lhs_cdf - sketch_rhs_cdf);
        
        sketch_cdf_distribution_lhs.push_back(sketch_lhs_cdf);
        sketch_cdf_distribution_rhs.push_back(sketch_rhs_cdf);
        distances_sketches.push_back(distance);
        
        max_dist_sketches = max(max_dist_sketches, distance);
    }

    return {.distribution_lhs = lhs_samples, .distribution_rhs = rhs_samples, .cdf_distribution_lhs = cdf_lhs, .cdf_distribution_rhs = cdf_rhs, .sketch_cdf_distribution_lhs = sketch_cdf_distribution_lhs, .sketch_cdf_distribution_rhs = sketch_cdf_distribution_rhs, .distances_distributions = distances_distributions, .distances_sketches = distances_sketches, .max_distance_distributions = max_dist_distributions, .max_distance_sketches = max_dist_sketches};
}

pair<double, vector<double>> test_distinct_normal_distribution(const int& samples, const int& start, const int& end, const int& step, const double& mean_lhs, const double& std_lhs, const double& mean_rhs, const double& std_rhs){
    vector<int> lhs = normal_int_stream(samples, mean_lhs, std_lhs, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);
    vector<int> rhs = normal_int_stream(samples, mean_rhs, std_rhs, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);

    return distributions_ks(lhs, rhs, start, end, step);
}

ks_test_result_t test_ks_with_kll(const int& samples, const int& start, const int& end, const int& step, const double& mean, const double& std, const double& error){
    vector<double> distances;
    vector<double> sketch_cdfs;
    double max_dist = numeric_limits<double>::min();
    kll<int> sketch(error);

    vector<int> distribution = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);

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

    vector<int> distribution = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);

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

    vector<int> distribution = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);

    for(size_t i = 0; i < distribution.size(); i++){
        ASSERT(distribution[i] >= 0  and distribution[i] < universe);
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

    vector<int> distribution = normal_int_stream(samples, mean, std, MIN_VALUE_IN_STREAM, MAX_VALUE_IN_STREAM);

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
    cout << "universe: The universe of values range for the sketches that need it" << endl;
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

void save_test(const string& prefix, const ks_test_result_t& res){
    string dist_fn = "distribution.dat";
    string cdf_fn = "cdf.dat";
    string sketch_cdf_fn = "sketch_cdf.dat";
    string distances_fn = "distances.dat";
    string max_dist_fn = "max_dist.dat";
    fstream out;

    out.open(prefix + dist_fn, fstream::out);

    for(size_t i = 0; i < res.distribution.size(); i++){
        out << res.distribution[i] << "\n";
    }

    out.close();

    out.open(prefix + cdf_fn, fstream::out);

    for(size_t i = 0; i < res.cdf_distribution.size(); i++){
        out << res.cdf_distribution[i] << "\n";
    }

    out.close();

    out.open(prefix + sketch_cdf_fn, fstream::out);

    for(size_t i = 0; i < res.sketch_cdf_distribution.size(); i++){
        out << res.sketch_cdf_distribution[i] << "\n";
    }

    out.close();

    out.open(prefix + distances_fn, fstream::out);

    for(size_t i = 0; i < res.distances.size(); i++){
        out << res.distances[i] << "\n";
    }

    out.close();

    out.open(prefix + max_dist_fn, fstream::out);

    out << res.max_distance << "\n";
    

    out.close();


    return;
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
    
    ks_test_result_t res;
    ks_test_result_t worst_kll_res;
    ks_test_result_t worst_dcs_res;
    ks_test_result_t worst_q_digest_res;
    ks_test_result_t worst_gk_res;
    double max_dist = numeric_limits<double>::min();

    //test_normal_distribution(10000, 0, 1000, 1, 700.0, 200.0);
    //test_distinct_normal_distribution(10000, 0, 1000, 1, 300.0, 100.0, 700.0, 100.0);

    for(double mean = min_mean_range; mean <= max_mean_range; mean += 50.0){
        for(double std = min_std_range; std <= max_std_range; std += 50.0){
            res = test_ks_with_kll(samples, start_point, end_point, step, mean, std, error);
            
            if(res.max_distance >= max_dist){
                worst_kll_res = res;
            }
        }
    }

    // cout << "KLL : " << worst_kll_res << endl;
    save_test(string("./kll_") + to_string(error) + string("_"), worst_kll_res);


    max_dist = numeric_limits<double>::min();
    
    for(double mean = min_mean_range; mean <= max_mean_range; mean += 50.0){
        for(double std = min_std_range; std <= max_std_range; std += 50.0){
            res = test_ks_with_dcs(samples, start_point, end_point, step, mean, std, error, universe);

            if(res.max_distance >= max_dist){
                worst_dcs_res = res;
            }
        }
    }

    //cout << "DCS : " << worst_dcs_res << endl;
    save_test(string("./dcs_") + to_string(error) + string("_"), worst_dcs_res);

    max_dist = numeric_limits<double>::min();

    for(double mean = min_mean_range; mean <= max_mean_range; mean += 50.0){
        for(double std = min_std_range; std <= max_std_range; std += 50.0){
            res = test_ks_with_q_digest(samples, start_point, end_point, step, mean, std, error, universe);

            if(res.max_distance >= max_dist){
                worst_q_digest_res = res;
            }
        }
    }

    //cout << "Q_DIGEST : " << worst_q_digest_res << endl;
    save_test(string("./q_digest_") + to_string(error) + string("_"), worst_q_digest_res);

    max_dist = numeric_limits<double>::min();

    for(double mean = min_mean_range; mean <= max_mean_range; mean += 50.0){
        for(double std = min_std_range; std <= max_std_range; std += 50.0){
            res = test_ks_with_gk(samples, start_point, end_point, step, mean, std, error);

            if(res.max_distance >= max_dist){
                worst_gk_res = res;
            }
        }
    }

    //cout << "GK : " << worst_gk_res << endl;
    save_test(string("./gk_") + to_string(error) + string("_"), worst_gk_res);

    return 0;
}
