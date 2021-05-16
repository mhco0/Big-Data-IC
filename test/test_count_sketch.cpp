#include <gtest/gtest.h>
#include <count_sketch/count_sketch.h>
#include <stream_maker/stream_maker.h>
#include <utils/utils.h>
using namespace std;
using namespace qsbd;
using namespace qsbd::stream_maker;

deque<string> g_args;
// error delta stream_size attempts stream_merge_size1 stream_merge_size2

TEST(CountScketchTest, TestAuxVariables){
    count_sketch cs(0.1, 0.3);

    EXPECT_DOUBLE_EQ(cs.get_error(), 0.1);
    EXPECT_DOUBLE_EQ(cs.get_delta(), 0.3);
    EXPECT_EQ(cs.get_t(), 30);
    EXPECT_EQ(cs.get_d(), 58);
    auto estimators = cs.get_estimators();

    EXPECT_EQ(estimators.size(), 58);
    EXPECT_EQ(estimators[0].size(), 30);

    for(int i = 0; i < estimators.size(); i++){
        for(int j = 0; j < estimators[i].size(); j++){
            EXPECT_EQ(estimators[i][j], 0);
        }
    }

    auto hash_functions = cs.get_hash_functions();

    EXPECT_EQ(hash_functions.size(), 58);
    EXPECT_EQ(hash_functions.size(), estimators.size());
}

TEST(CountScketchTest, TestUpdate){
    count_sketch cs(0.1, 0.3);

    cs.update(1, 1);
    auto hash_functions = cs.get_hash_functions();
    auto estimators = cs.get_estimators();

    for(int i = 0; i < estimators.size(); i++){
        int hx = hash_functions[i](1) % (2 * cs.get_t());
        int index = hx >> 1;
        bool condition = (estimators[i][index] == 1) or (estimators[i][index] == -1);

        EXPECT_TRUE(condition) << estimators[i][index];
    }
}

TEST(CountScketchTest, TestQueryAndBounds){
    int stream_size = stoi(g_args[2]);
    int attempts = stoi(g_args[3]);
    double error = stod(g_args[0]);
    double delta = stod(g_args[1]);
    vector<pair<int, int>> stream = random_int_stream_with_weight(stream_size, 0, 100, -20, 20);
    map<int, int> frequency = frequency_counter(stream);
    map<int, int> fails;
    int total_weight = weight_from_stream(stream);


    for(int i = 0; i < attempts; i++){
        count_sketch cs(error, delta);

        for(auto& it : stream){
            cs.update(it.first, it.second);
        }

        for(auto& it : frequency){
            int approx_f = cs.query(it.first);
            int real_f = it.second;
            
            if(fabs(approx_f - real_f) > (error * total_weight)) fails[it.first]++;
        }
    }

    for(auto& it : fails){
        EXPECT_LE((it.second / (double) attempts), delta) << it.second;
    }
}

TEST(CountScketchTest, TestMerge){
    int S1 = stoi(g_args[4]);
    int S2 = stoi(g_args[5]);
    int attempts = stoi(g_args[3]);
    double error = stod(g_args[0]);
    double delta = stod(g_args[1]);
    int d = ceil(48 * log(1.0 / delta));
    int t = 3.0 / error;
    vector<pair<int, int>> stream1 = random_int_stream_with_weight(S1, 0, 100, -50, 50);
    vector<pair<int, int>> stream2 = random_int_stream_with_weight(S2, 0, 100, 0, 60);
    vector<pair<int, int>> merged_stream = merge_stream(stream1, stream2);
    int total_weight = weight_from_stream(merged_stream);
    map<int, int> frequency = frequency_counter(merged_stream);
    map<int, int> fails;
    vector<k_wise_family> hash_functions;

    for(int i = 0; i < d; i++){
        k_wise_family h(2, 2 * t);

        hash_functions.push_back(h);
    }

    for(int i = 0; i < attempts; i++){
        count_sketch cs1 (error, delta, hash_functions);
        count_sketch cs2 (error, delta, hash_functions);

        for(auto& it : stream1){   
            cs1.update(it.first, it.second);
        }

        for(auto& it : stream2){
            cs2.update(it.first, it.second);
        }

        count_sketch* merged_cs = cs1.merge(cs2);

        for(auto & it : frequency){
            int approx_f = merged_cs->query(it.first);
            int real_f = it.second;
            
            if(fabs(approx_f - real_f) > (error * total_weight)) fails[it.first]++;
        }
    }

    for(auto& it : fails){
        EXPECT_LE((it.second / (double) attempts), delta) << it.second;
    }
}


int main(int argc, char* argv[]){
    testing::InitGoogleTest(&argc, argv);

    g_args = process_args(argc, argv);

    cout << fixed;

    return RUN_ALL_TESTS();
}