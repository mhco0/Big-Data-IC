#include <gtest/gtest.h>
#include <stream_maker/stream_maker.h>
using namespace std;
using namespace qsbd::stream_maker;

TEST(StreamTest, TestSortAndMergeWeights){
	vector<pair<int,int>> stream = random_int_stream_with_weight(3, 1, 1, 0, 5);
	cout << "Before sort and merge:" << endl;
	show_stream(stream);
	stream = sort_and_merge_weight(stream);
	cout << "After :" << endl;
	show_stream(stream);

	for(int i = 0, j = 1; j < stream.size(); i++, j++){
		EXPECT_LT(stream[i].first, stream[j].first);
	}
}

TEST(StreamTest, TestNormalStream){
	vector<pair<int, int>> stream = normal_int_stream_with_weight(10000, 30.0, 5.0, 1, 1);
	stream = sort_and_merge_weight(stream);
	vector<double> xs, ys;

	for(auto& it : stream){
		xs.push_back((double) it.first);
		ys.push_back((double) it.second);
	}
}

TEST(StreamTest, TestRanksFromNormalStream){
	vector<pair<int, int>> stream = normal_int_stream_with_weight(10000, 30.0, 5.0, 1, 1);
	stream = sort_and_merge_weight(stream);
	vector<int> ranks = real_ranks_from_stream_with_weight(stream);
	vector<double> xs, ys;

	for(auto& it : stream){
		xs.push_back((double) it.first);
		ys.push_back((double) ranks[it.first]);
	}
}

TEST(StreamTest, TestRandomStream){
	vector<int> stream = random_int_stream(100, 0, 10);

	ASSERT_TRUE(stream.size() == 100);

	for(auto& it : stream){	
		EXPECT_LE(it, 10);
		EXPECT_GE(it, 0);
	}
}

TEST(StreamTest, TestRandomStreamWithWeights){
    vector<pair<int,int>> stream = random_int_stream_with_weight(100, 0, 10, -50, 50);

    ASSERT_TRUE(stream.size() == 100);

    for(auto& it : stream){
        EXPECT_LE(it.first, 10);
        EXPECT_GE(it.first, 0);
        EXPECT_LE(it.second, 50);
        EXPECT_GE(it.second, -50);
    }
}

TEST(StreamTest, TestMergeStream){
	vector<int> stream1 = random_int_stream(100, 0, 10);
	vector<int> stream2 = random_int_stream(300, 20, 50);
	vector<int> merged = merge_stream<int>(stream1, stream2);

	ASSERT_TRUE(merged.size() == 400);

	for(auto& it : merged){	
		EXPECT_LE(it, 50);
		EXPECT_GE(it, 0);
	}
}

TEST(StreamTest, TestTotalWeight){
	vector<pair<int, int>> stream = {{1, 2}, {5, 10}, {1, -2}, {4, 3}, {2, 7}, {5, -5}};
	int ans = 29;

	ASSERT_TRUE(weight_from_stream(stream) == ans);
}

TEST(StreamTest, TestFrequencyCounter){
	vector<int> stream = {1, 3, 4, 1, 2, 3, 4, 5, 1, 3};
	map<int, int> ans;
	ans[1] = 3;
	ans[2] = 1;
	ans[3] = 3;
	ans[4] = 2;
	ans[5] = 1;

	ASSERT_TRUE(ans == frequency_counter(stream));
}

TEST(StreamTest, TestFrequencyCounterWithWeight){
	vector<pair<int, int>> stream = {{1, 3}, {2, 1}, {3, 5}, {3, -2}, {4, 2}, {5, 1}};
	map<int, int> ans;
	ans[1] = 3;
	ans[2] = 1;
	ans[3] = 3;
	ans[4] = 2;
	ans[5] = 1;

	ASSERT_TRUE(ans == frequency_counter(stream));
}

TEST(StreamTest, TestRandomRegionPoints){
	vector<pair<int, pair<double, double>>> stream = random_stream_in_region(1000, 0.0, 0.0, 100.0, 100.0);

	EXPECT_EQ(stream.size() , 1000);

	for(int i = 0; i < stream.size(); i++){
		EXPECT_EQ(stream[i].first, i);

		EXPECT_LE(stream[i].second.first, 100.0);
		EXPECT_LE(stream[i].second.second, 100.0);
		EXPECT_GE(stream[i].second.first, 0.0);
		EXPECT_GE(stream[i].second.second, 0.0);
	}
}

TEST(StreamTest, TestRandomCitys){
	auto stream = random_stream_city(1000, 0.0, 0.0, 100.0, 100.0, 0, 1000, 10, 5.0);
	//ofstream city_file;

	//city_file.open("test_file.txt", ios::out);

	EXPECT_EQ(stream.size() , 1000);

	for(int i = 0; i < stream.size(); i++){
		//city_file << stream[i].second.first << "\t" << stream[i].second.second << "\t" << stream[i].first << endl;
		
		EXPECT_LE(stream[i].first, 1000);
		EXPECT_GE(stream[i].first, 0);
		EXPECT_LE(stream[i].second.first, 100.0);
		EXPECT_LE(stream[i].second.second, 100.0);
		EXPECT_GE(stream[i].second.first, 0.0);
		EXPECT_GE(stream[i].second.second, 0.0);
	}

	//city_file.close();
}

TEST(StreamTest, TestRandomRegion){
	auto rect = random_rectangle_in_region(0.0, 0.0, 100.0, 100.0);

	
	EXPECT_LE(rect.second.first, 100.0);
	EXPECT_LE(rect.second.second, 100.0);
	EXPECT_GE(rect.first.first, 0.0);
	EXPECT_GE(rect.first.second, 0.0);
}

TEST(RanksTest, TestRealRank){
	vector<int> stream = {1, 3, 4, 3, 4, 5, 10, 2, 5, 2, 9, 10};
	vector<int> ranks = {0, 0, 1, 3, 5, 7, 9, 9, 9, 9, 10};

	vector<int> real_ranks = real_ranks_from_stream(stream);
	
	ASSERT_EQ(ranks.size(), real_ranks.size());

	for(int i = 0; i < real_ranks.size(); i++){
		EXPECT_EQ(ranks[i], real_ranks[i]);
	}
}

TEST(RanksTest, TestRealRanksWithWeights){
    vector<pair<int, int>> stream = {{1, 2}, {2, 4}, {2, -2}, {3, 5}, {1, 6}, {3, 3}, {2, -1}};
    vector<int> ranks = {0, 0, 8, 9};

    vector<int> real_ranks = real_ranks_from_stream_with_weight(stream);

    ASSERT_TRUE(ranks.size() ==  real_ranks.size());

    for(int i = 0; i < real_ranks.size(); i++){
        EXPECT_EQ(ranks[i], real_ranks[i]);
    }
}