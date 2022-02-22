#include <gtest/gtest.h>
#include <kolmogorov_smirnov/kolmogorov_smirnov.hpp>
#include <kll/kll.hpp>
#include <gk/gk.hpp>
#include <q_digest/q_digest.h>
#include <dcs/dcs.h>
using namespace std;
using namespace qsbd;
using namespace stream_maker;

deque<string> g_args;

TEST(KSTest, TestNormalDistribution){
    int samples = 10000;
    int start = 0;
    int end = 1000;
    int step = 1;
    double mean = 700.0;
    double std = 200.0;

    vector<int> lhs = normal_int_stream(samples, mean, std);
    vector<int> rhs = normal_int_stream(samples, mean, std);

    auto dks = distributions_ks(lhs, rhs, start, end, step);
    
    cout << dks.first << endl;

    cout << endl;

    for(auto& it : dks.second){
        cout << it << " ";
    }

    cout << endl;
}

TEST(KSTest, TestDistincNormalDistribution){
    int samples = 10000;
    int start = 0;
    int end = 1000;
    int step = 1;
    double mean_lhs = 700.0;
    double std_lhs = 200.0;
    double mean_rhs = 300.0;
    double std_rhs = 100.0;

    vector<int> lhs = normal_int_stream(samples, mean_lhs, std_lhs);
    vector<int> rhs = normal_int_stream(samples, mean_rhs, std_rhs);

    auto dks = distributions_ks(lhs, rhs, start, end, step);
    
    cout << dks.first << endl;

    cout << endl;

    for(auto& it : dks.second){
        cout << it << " ";
    }

    cout << endl;
}

TEST(KSTest, TestKSWithKLL){
    int samples = 10000;
    int start = 0;
    int end = 1000;
    int step = 1;
    double mean = 700.0;
    double std = 200.0;
    vector<double> distances;
    double max_dist = numeric_limits<double>::min();
    kll<int> sketch(0.003);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        sketch.update(distribution[i]);
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end; i += step, j++){
        double distance = fabs(cdf_samples[j] - sketch.cdf(i));
        distances.push_back(distance);
        max_dist = max(max_dist, distance);
    }
    
    cout << max_dist << endl;

    cout << endl;

    for(auto& it : distances){
        cout << it << " ";
    }

    cout << endl;
}

TEST(KSTest, TestKSWithDCS){
    int samples = 10000;
    int start = 0;
    int end = 1000;
    int step = 1;
    double mean = 700.0;
    double std = 200.0;
    vector<double> distances;
    double max_dist = numeric_limits<double>::min();
    dcs sketch(0.003, 16384);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        sketch.update(distribution[i], 1);
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
        double distance = fabs(cdf_samples[j] - sketch.cdf(i));
        distances.push_back(distance);
        max_dist = max(max_dist, distance);
    }
    
    cout << max_dist << endl;

    cout << endl;

    for(auto& it : distances){
        cout << it << " ";
    }

    cout << endl;
}

TEST(KSTest, TestKSWithQDigest){
    int samples = 10000;
    int start = 0;
    int end = 1000;
    int step = 1;
    double mean = 700.0;
    double std = 200.0;
    vector<double> distances;
    double max_dist = numeric_limits<double>::min();
    q_digest sketch(0.003, 16384);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        sketch.update(distribution[i], 1);
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
        double distance = fabs(cdf_samples[j] - sketch.cdf(i));
        distances.push_back(distance);
        max_dist = max(max_dist, distance);
    }
    
    cout << max_dist << endl;

    cout << endl;

    for(auto& it : distances){
        cout << it << " ";
    }

    cout << endl;
}

TEST(KSTest, TestKSWithGK){
    int samples = 10000;
    int start = 0;
    int end = 1000;
    int step = 1;
    double mean = 700.0;
    double std = 200.0;
    vector<double> distances;
    double max_dist = numeric_limits<double>::min();
    gk<int> sketch(0.003);

    vector<int> distribution = normal_int_stream(samples, mean, std);

    for(size_t i = 0; i < distribution.size(); i++){
        sketch.update(distribution[i]);
    }

    auto cdf_samples = cdf_from_samples(distribution, start, end, step);

    for(int i = start, j = 0; i < end and j < cdf_samples.size(); i += step, j++){
        double distance = fabs(cdf_samples[j] - sketch.cdf(i));
        distances.push_back(distance);
        max_dist = max(max_dist, distance);
    }
    
    cout << max_dist << endl;

    cout << endl;

    for(auto& it : distances){
        cout << it << " ";
    }

    cout << endl;
}

int main(int argc, char* argv[]){
    testing::InitGoogleTest(&argc, argv);

    g_args = process_args(argc, argv);

    cout << fixed;

    return RUN_ALL_TESTS();
}