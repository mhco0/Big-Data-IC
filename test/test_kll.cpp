#include <gtest/gtest.h> 
#include <kll/kll.hpp>
#include <stream_maker/stream_maker.h>
#include <kll_factory/kll_factory.hpp>
#include <utils/utils.h>
using namespace std;
using namespace qsbd;
using namespace qsbd::stream_maker;

deque<string> g_args;
// error stream_size attempts stream_merge_sz1 stream_merge_sz2 stream_weight_min stream_weight_max

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
	int N = stoi(g_args[1]);
	double error = stod(g_args[0]);
	int attempts = stoi(g_args[2]);
	vector<int> stream = random_int_stream(N, stoi(g_args[5]), stoi(g_args[6]));
	vector<int> real_ranks = real_ranks_from_stream(stream);
	vector<int> fails(real_ranks.size(), 0);

	for(int i = 0; i < attempts; i++){
		kll<int> test(error);

		for(auto& it : stream){
			test.update(it);
		}

		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = test.query(j);
			int real_rank = real_ranks[j];

			if(abs(approximated_rank - real_rank) > (error * N)){
				fails[j]++;
			}
		}
	}

	for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), error);
	}
}

TEST(KllTest, TestMerge){
	int N1 = stoi(g_args[3]);
	int N2 = stoi(g_args[4]);
	int N_total = N1 + N2;
	double error = stod(g_args[0]);
	int attempts = stoi(g_args[2]);
	vector<int> stream1 = random_int_stream(N1, 0, 100);
	vector<int> stream2 = random_int_stream(N2, 100, 200);
	vector<int> merged_stream = merge_stream<int>(stream1, stream2);
	vector<int> real_ranks = real_ranks_from_stream(merged_stream);
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

		kll<int>* merged_kll = dynamic_cast<kll<int>*>(k1.merge(k2));

		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = merged_kll->query(j);
			int real_rank = real_ranks[j];

			if(abs(approximated_rank - real_rank) > (error * N_total)){
				fails[j]++;
			}
		}

		delete merged_kll;

	}

	for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), error) << "fails : " << fails[i];
	}
	
}

TEST(KllTest, TestFactory){
	int N = stoi(g_args[1]);
	double error = stod(g_args[0]);
	int attempts = stoi(g_args[2]);
	vector<int> stream = random_int_stream(N, stoi(g_args[5]), stoi(g_args[6]));
	vector<int> real_ranks = real_ranks_from_stream(stream);
	vector<int> fails(real_ranks.size(), 0);

	for(int i = 0; i < attempts; i++){
		kll_factory<int> factory(error);
		kll<int>* test = dynamic_cast<kll<int> *>(factory.instance());

		for(auto& it : stream){
			test->update(it);
		}

		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = test->query(j);
			int real_rank = real_ranks[j];

			if(abs(approximated_rank - real_rank) > (error * N)){
				fails[j]++;
			}
		}

		delete test;
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