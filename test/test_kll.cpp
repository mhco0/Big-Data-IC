#include <gtest/gtest.h> 
#include <kll/kll.hpp>
#include <commum_header/commum_header.h>
#include <global_generator/global_generator.h>

using namespace std;

vector<int> random_int_stream(int vector_size, int min_v, int max_v){
	uniform_int_distribution<int> who_pick(min_v,max_v);
	vector<int> random_stream;

	for(int i = 0; i < vector_size; i++){
		random_stream.push_back(who_pick(generator));
	}

	return random_stream;
}

vector<int> real_ranks_from(vector<int>& stream){
	sort(stream.begin(),stream.end());

	int max_elem = stream[stream.size() - 1];
	bool find_nz = false;
	vector<int> ranks(max_elem + 1, 0);

	for(int i = 0; i < stream.size(); i++){
		ranks[stream[i]]++;
	}

	int accumulate = 0;

	for(int i = 0; i < ranks.size(); i++){
		if(ranks[i] != 0){
			accumulate += ranks[i];
			ranks[i] = accumulate - ranks[i];
		}else{
			ranks[i] = accumulate;
		}
	}

	return ranks;
}

vector<int> merge_stream(vector<int>& stream1, vector<int>& stream2){
	vector<int> merged_stream;
	int i = 0;
	int j = 0;

	while(i < stream1.size() && j < stream2.size()){
		merged_stream.push_back(stream1[i]);
		merged_stream.push_back(stream2[j]);
		i++;
		j++;
	}

	while(i < stream1.size()){
		merged_stream.push_back(stream1[i]);
		i++;
	}

	while(j < stream2.size()){
		merged_stream.push_back(stream2[j]);
		j++;
	}

	return merged_stream;
}

TEST(RanksTest, TestRandomStream){
	vector<int> stream = random_int_stream(100, 0, 10);

	ASSERT_TRUE(stream.size() == 100);

	for(auto& it : stream){	
		EXPECT_LE(it, 10);
		EXPECT_GE(it, 0);
	}
}

TEST(RanksTest, TestRealRank){
	vector<int> stream = {1, 3, 4, 3, 4, 5, 10, 2, 5, 2, 9, 10};
	vector<int> ranks = {0, 0, 1, 3, 5, 7, 9, 9, 9, 9, 10};

	vector<int> real_ranks = real_ranks_from(stream);
	
	ASSERT_EQ(ranks.size(), real_ranks.size());

	for(int i = 0; i < real_ranks.size(); i++){
		EXPECT_EQ(ranks[i], real_ranks[i]);
	}
}

TEST(RanksTest, TestMergeStream){
	vector<int> stream1 = random_int_stream(100, 0, 10);
	vector<int> stream2 = random_int_stream(300, 20, 50);
	vector<int> merged = merge_stream(stream1, stream2);

	ASSERT_TRUE(merged.size() == 400);

	for(auto& it : merged){	
		EXPECT_LE(it, 50);
		EXPECT_GE(it, 0);
	}
}

TEST(KllTest, TestInsertSorted){
	vector<int> random_input;
	uniform_int_distribution<int> random_small_number(0, 1000);

	for(int i = 0; i < 100; i++){
		kll<int>::insert_sorted(random_input,random_small_number(generator));
	} 

	ASSERT_TRUE(random_input.size() == 100);

	for(int i = 0, j = 1; j < 100; i++, j++){
		ASSERT_LE(random_input[i], random_input[j]);
	}
}

TEST(KllTest, TestError){
	kll<int> test(0.3);

	ASSERT_DOUBLE_EQ(test.get_error(), 0.3);
}

TEST(KllTest, TestCapacity){
	vector<double> capacity_ans = {
		66.08295292273436, //0.1
		28.967818507934243, //0.2
		17.52928035389943, //0.3
		12.109189191169586, //0.4
		8.990744710217754, //0.5
		6.982244141345214, //0.6
		5.588532265354038, //0.7
		4.568405040545396, //0.8
		3.7908359091082557 //0.9
	};

	for(int i = 0; i < 9; i++){
		double err = ((i + 1)/10.0);
		kll<int> test(err);
		EXPECT_NEAR(test.get_capacity(), capacity_ans[i], 1e-6);
	}
}

TEST(KllTest, TestUpdateAndCompression){    
	vector<int> input = {2, 3, 4, 5, 6, 12, 32, 13, 45};
	kll<int> test(0.3);

	for(auto&it : input){
		test.update(it);
		test.print();
	}
}

TEST(KllTest, TestQuery){
	vector<vector<int>> paper_buffers = {
		{84},
		{16},
		{92, 142},
		{23, 81, 134},
		{61, 112, 123, 175},
		{33, 71, 105, 152, 165, 184}
	};

	kll<int> test(0.25); // k = 8

	test.set_buffers(paper_buffers);
	
	EXPECT_EQ(test.query(100), 103);
	if(HasNonfatalFailure()){
		test.print();
	}
}

TEST(KllTest, TestBounds){
	int N = 20000;
	double error = 0.01;
	int attempts = 100;
	vector<int> stream = random_int_stream(N, 20, 1000);
	vector<int> real_ranks = real_ranks_from(stream);
	vector<int> fails(real_ranks.size(), 0);

	for(int i = 0; i < attempts; i++){
		kll<int> test(error);

		for(auto& it : stream){
			test.update(it);
		}

		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = test.query(j);
			int real_rank = real_ranks[j];

			if(abs(approximated_rank - real_rank) > (error*N)){
				fails[j]++;
			}
		}
	}

	for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), error);
	}
}

TEST(KllTest, TestMerge){
	int N1 = 100;
	int N2 = 300;
	int N_total = N1 + N2;
	double error = 0.3;
	int attempts = 1000;
	vector<int> stream1 = random_int_stream(N1, 0, 100);
	vector<int> stream2 = random_int_stream(N2, 100, 200);
	vector<int> merged_stream = merge_stream(stream1, stream2);
	vector<int> real_ranks = real_ranks_from(merged_stream);
	vector<int> fails(real_ranks.size(), 0);

	for(int i = 0; i < attempts; i++){
		kll<int> k1(error);
		kll<int> k2(error);

		for(auto& it : stream1){
			k1.update(it);
		}

		for(auto& it : stream2){
			k2.update(it);
		}

		kll<int> merged_kll = k1.merge(k2);

		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = merged_kll.query(j);
			int real_rank = real_ranks[j];

			if(abs(approximated_rank - real_rank) > (error*N_total)){
				fails[j]++;
			}
		}
	}

	for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), error) << "fails : " << fails[i];
	}
	
}
