#include <gtest/gtest.h>
//#include <memory_tracker/memory_tracker.h>
#include <qsbd_debug/qsbd_debug.h>
#include <timer/timer.h>
#include <stream_maker/stream_maker.h>
#include <quantile_quadtree/quantile_quadtree.hpp>
#include <kll_factory/kll_factory.hpp>
#include <gk_factory/gk_factory.hpp>
#include <q_digest_factory/q_digest_factory.h>
#include <dcs_factory/dcs_factory.h>
#include <utils/utils.h>
using namespace std;
using namespace qsbd;
using namespace qsbd::stream_maker;

deque<string> g_args;
// aabb[0-3] deep error stream_size attempts 

vector<int> real_ranks_from_stream_gk(const vector<int>& stream, int min_v,int max_v){
    vector<int> weights(max_v + 1, 0);
    vector<int> ranks(max_v + 1, 0);

    for(int i = 0; i < stream.size(); i++){
        weights[stream[i]]++;    
	}

    for(int i = 0, j = 1; j <= max_v; i++, j++){
        ranks[j] = weights[i] + ranks[i];
    }

    for(int i = 0; i < ranks.size(); i++){
        ranks[i] += weights[i] - 1;
    }

	return ranks;
}


vector<int> brute_force_search(const vector<pair<int, pair<double, double>>>& stream, const aabb<int>& region){
    vector<int> ans;

    for(auto&it : stream){
        point<int> p(it.second.first, it.second.second);
        if (region.contains(p)){
            ans.push_back(it.first);
        }
    }

    return ans;
}

vector<pair<int, int>> brute_force_search(const vector<pair<pair<int, int>, pair<double, double>>>& stream, const aabb<int>& region){
    vector<pair<int, int>> ans;

    for(auto&it : stream){
        point<int> p(it.second.first, it.second.second);
        if (region.contains(p)){
            ans.push_back(it.first);
        }
    }

    return ans;
}

aabb<int> construct_aabb_from_random_region(double minx, double miny, double maxx, double maxy){
    auto bounds = random_rectangle_in_region(minx, miny, maxx, maxy);

    aabb<int> region(bounds.first.first, bounds.first.second, bounds.second.first, bounds.second.second);

    return region;
}

TEST(QuantileQuadtreeTest, TestConstructWithKll){
    double error = stod(g_args[5]);
    aabb<int> resolution(stod(g_args[0]), stod(g_args[1]), stod(g_args[2]), stod(g_args[3]));
    kll_factory<int> factory(error);
    int deep = stoi(g_args[4]);

    timer counter;
    counter.start();
    quantile_quadtree<int> test(resolution, deep, &factory);
    counter.end();
}

TEST(QuantileQuadtreeTest, TestUpdateAndQueryWithKll){
    int N = stoi(g_args[6]);
    int points_guarantee = 100;
    int attempts = stoi(g_args[7]);
    int deep = stoi(g_args[4]);
    double error = stod(g_args[5]);
    aabb<int> resolution(stod(g_args[0]), stod(g_args[1]), stod(g_args[2]), stod(g_args[3]));
    aabb<int> search_region = construct_aabb_from_random_region(stod(g_args[0]), stod(g_args[1]), stod(g_args[2]), stod(g_args[3]));

	vector<pair<int, pair<double, double>>> stream =  random_stream_city(N, stod(g_args[0]), stod(g_args[1]), stod(g_args[2]), stod(g_args[3]), 0, N, 15, 5.0);
    vector<pair<int, pair<double, double>>> guarantee_stream = random_stream_in_region(points_guarantee, search_region.bounds().first.x(), search_region.bounds().first.y(), search_region.bounds().second.x(), search_region.bounds().second.y());
    for(auto& it : guarantee_stream){
        stream.push_back(it);
    }
    vector<int> stream_in_region = brute_force_search(stream, search_region);
	vector<int> real_ranks = real_ranks_from_stream(stream_in_region);
	vector<int> fails(real_ranks.size(), 0);
    
	for(int i = 0; i < attempts; i++){
		kll_factory<int> factory(error);
        quantile_quadtree<int> test(resolution, deep, &factory);

        timer counter;

        counter.start();
		for(auto& it : stream){
            test.update(point<int>(it.second.first, it.second.second), it.first);
        }
        counter.end();

        counter.start();
		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = test.query(search_region, j);
			int real_rank = real_ranks[j];

			if(abs(approximated_rank - real_rank) > (error * (N + points_guarantee))){
				fails[j]++;
			}
		}
        counter.end();
	}

	for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), error);
	}
}

TEST(QuantileQuadtreeTest, TestConstructWithGK){
    double error = 0.3;
    aabb<int> resolution(0.0, 0.0, 1280.0, 720.0);
    gk_factory<int> factory(error);
    int deep = 8;
    quantile_quadtree<int> test(resolution, deep, &factory);
}

TEST(QuantileQuadtreeTest, TestUpdateAndQueryWithGK){
    int N = 10000;
    int attempts = 1;
    int deep = 8;
    double error = 0.3;
    int points_guarantee = 100;
    aabb<int> resolution(0.0, 0.0, 1280.0, 720.0);
    aabb<int> search_region = construct_aabb_from_random_region(0.0, 0.0, 1280.0, 720.0);
 
	vector<pair<int, pair<double, double>>> stream =  random_stream_city(N, 0.0, 0.0, 1280.0, 720.0, 0, N, 15, 5.0);
    vector<pair<int, pair<double, double>>> guarantee_stream = random_stream_in_region(points_guarantee, search_region.bounds().first.x(), search_region.bounds().first.y(), search_region.bounds().second.x(), search_region.bounds().second.y());
    for(auto& it : guarantee_stream){
        stream.push_back(it);
    }
    vector<int> stream_in_region = brute_force_search(stream, search_region);
	vector<int> real_ranks = real_ranks_from_stream_gk(stream_in_region, 0, N);
    vector<int> fails(real_ranks.size(), 0);

	for(int i = 0; i < attempts; i++){
		gk_factory<int> factory(error);
        quantile_quadtree<int> test(resolution, deep, &factory);

		for(auto& it : stream){
            test.update(point<int>(it.second.first, it.second.second), it.first);
        }

		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = test.query(search_region, j);
			int real_rank = real_ranks[j];

            if(abs(approximated_rank - real_rank) > (error * (N + points_guarantee))){
				fails[j]++;
			}
		}
	}

    for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), error);
	}
}

TEST(QuantileQuadtreeTest, TestConstructWithQDigest){
    double error = 0.3;
    aabb<int> resolution(0.0, 0.0, 1280.0, 720.0);
    q_digest_factory factory(error, 1000);
    int deep = 8;
    quantile_quadtree<int> test(resolution, deep, &factory);
}


TEST(QuantileQuadtreeTest, TestUpdateAndQueryWithQDigest){
    int N = 10000;
    int points_guarantee = 100;
    int universe = 16384;
    int attempts = 1;
    int deep = 8;
    double error = 0.3;
    aabb<int> resolution(0.0, 0.0, 1280.0, 720.0);
    aabb<int> search_region = construct_aabb_from_random_region(0.0, 0.0, 1280.0, 720.0);
    
	vector<pair<pair<int, int>, pair<double, double>>> stream =  random_stream_in_region_with_weight(N, 0.0, 0.0, 1280.0, 720.0, 0, N, 1, 50);
    vector<pair<pair<int, int>, pair<double, double>>> guarantee_stream = random_stream_in_region_with_weight(points_guarantee, search_region.bounds().first.x(), search_region.bounds().first.y(), search_region.bounds().second.x(), search_region.bounds().second.y(), 0, N, 1, 50);
    for(auto& it : guarantee_stream){
        stream.push_back(it);
    }
    vector<pair<int, int>> stream_in_region = brute_force_search(stream, search_region);
	vector<int> real_ranks = real_ranks_from_stream_with_weight(stream_in_region);

    vector<pair<int, int>> stream_without_location;
    for(auto& it : stream){
        stream_without_location.push_back(it.first);
    }

    int total_weight = weight_from_stream(stream_without_location);

	for(int i = 0; i < attempts; i++){
		q_digest_factory factory(error, universe);
        quantile_quadtree<int> test(resolution, deep, &factory);

		for(auto& it : stream){
            test.update(point<int>(it.second.first, it.second.second), it.first.first, it.first.second);
        }

		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = test.query(search_region, j);
			int real_rank = real_ranks[j];

            EXPECT_GE(real_rank, approximated_rank);
            EXPECT_LE(real_rank, approximated_rank + total_weight * error);
		}
	}
}

TEST(QuantileQuadtreeTest, TestConstructWithDcs){
    double error = stod(g_args[5]);
    int universe = 1024;
    int deep = stoi(g_args[4]);
    aabb<int> resolution(stod(g_args[0]), stod(g_args[1]), stod(g_args[2]), stod(g_args[3]));
    dcs_factory factory(error, universe);

    timer counter;
    counter.start();
    quantile_quadtree<int> test(resolution, deep, &factory);
    counter.end();
}

TEST(QuantileQuadtreeTest, TestUpdateAndQueryWithDcs){
    int N = stoi(g_args[6]);
    int points_guarantee = 100;
    int attempts = stoi(g_args[7]);
    int deep = stoi(g_args[4]);
    int universe = 131072;
    double error = stod(g_args[5]);
    aabb<int> resolution(stod(g_args[0]), stod(g_args[1]), stod(g_args[2]), stod(g_args[3]));
    aabb<int> search_region = construct_aabb_from_random_region(stod(g_args[0]), stod(g_args[1]), stod(g_args[2]), stod(g_args[3]));

	vector<pair<pair<int, int>, pair<double, double>>> stream = random_stream_in_region_with_weight(N, stod(g_args[0]), stod(g_args[1]), stod(g_args[2]), stod(g_args[3]), 0, N, 1, 50);
    vector<pair<pair<int, int>, pair<double, double>>> guarantee_stream = random_stream_in_region_with_weight(points_guarantee, search_region.bounds().first.x(), search_region.bounds().first.y(), search_region.bounds().second.x(), search_region.bounds().second.y(), 0, N, 1, 50);
    for(auto& it : guarantee_stream){
        stream.push_back(it);
    }
    vector<pair<int, int>> stream_in_region = brute_force_search(stream, search_region);
	vector<int> real_ranks = real_ranks_from_stream_with_weight(stream_in_region);
	vector<int> fails(real_ranks.size(), 0);
    int total_weight = weight_from_stream(stream_in_region, false);
    
	for(int i = 0; i < attempts; i++){
		dcs_factory factory(error, universe);
        quantile_quadtree<int> test(resolution, deep, &factory);

        timer counter;

        counter.start();
		for(auto& it : stream){
            test.update(point<int>(it.second.first, it.second.second), it.first.first, it.first.second);
        }
        counter.end();

        counter.start();
		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = test.query(search_region, j);
			int real_rank = real_ranks[j];

			if(abs(approximated_rank - real_rank) > (error * total_weight)){
				fails[j]++;
			}
		}
        counter.end();
	}

	for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), error);
	}
}


int main(int argc, char* argv[]){
    testing::InitGoogleTest(&argc, argv);

    g_args = process_args(argc, argv);

    cout << fixed;

    return RUN_ALL_TESTS();
}