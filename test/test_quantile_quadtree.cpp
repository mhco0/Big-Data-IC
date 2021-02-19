#include <gtest/gtest.h>
#include <stream_maker/stream_maker.h>
#include <quantile_quadtree/quantile_quadtree.hpp>
#include <count_sketch/count_sketch.h>
using namespace std;

vector<pair<int, int>> brute_force_search(const vector<pair<pair<int, int>, pair<double, double>>>& stream, const aabb& region){
    vector<pair<int, int>> ans;

    for(auto&it : stream){
        point<double> p(it.second.first, it.second.second);
        if (region.contains(p)){
            ans.push_back(it.first);
        }
    }

    return ans;
}

TEST(QuantileQuadtreeTest, TestConstructWithCountSketch){
    aabb resolution(0.0, 0.0, 1280.0, 720.0);
    aabb leaf_sz(0.0, 0.0, 10.0, 10.0);
    quantile_quadtree<count_sketch, int> quad(resolution, leaf_sz, 0.1, 0.3);

    point<double> p(0.0, 3.4);
}


TEST(QuantileQuadtreeTest, TestUpdateAndQueryWithCountSketch){
    aabb resolution(0.0, 0.0, 400.0, 400.0);
    aabb leaf_sz(0.0, 0.0, 100.0, 100.0);
    int attempts = 100;
    double error = 0.1;
    double delta = 0.3;
    int d = ceil(48 * log(1.0 / delta));
    int t = 3.0 / error;
    auto stream = random_stream_in_region_with_weight(1000, 0.0, 0.0, 400.0, 400.0, 0, 100, 0, 100);
    aabb search_region(0.0, 0.0, 400.0, 400.0);
    auto points = brute_force_search(stream, search_region);
    int total_weight = weight_from_stream(points);
    map<int, int> frequency = frequency_counter(points);
    map<int, int> fails;
    vector<k_wise_family> hash_functions;   

    for(int i = 0; i < d; i++){
        k_wise_family h(2, 2 * t);

        hash_functions.push_back(h);
    }

    for(int i = 0; i < attempts; i++){
        quantile_quadtree<count_sketch, int> quad(resolution, leaf_sz, 0.1, 0.3, hash_functions);

        for(auto& it : stream){
            quad.update(point<double>(it.second.first, it.second.second), it.first.first, it.first.second);
        }

        for(auto & it : frequency){
            int approx_f = quad.query(search_region, it.first);
            int real_f = it.second;

            if(fabs(approx_f - real_f) > (error * total_weight)) fails[it.first]++;
        }
    }

    for(auto& it : fails){
        EXPECT_LE((it.second / (double) attempts), delta) << it.second;
    }
    
}