#include <gtest/gtest.h>
#include <stream_maker/stream_maker.h>
#include <dcs/dcs.h>
#include <dcs_factory/dcs_factory.h>

using namespace std;

TEST(DcsTest, TestInit){
    int universe = 64;
    dcs test(0.3, universe);

    EXPECT_TRUE(test.get_tree_lvl() == 6);
    EXPECT_DOUBLE_EQ(test.get_error(), 0.3);
    EXPECT_TRUE(test.get_universe() == 64);
    EXPECT_TRUE(test.get_d() == 2);
    EXPECT_TRUE(test.get_w() == 14);
    EXPECT_TRUE(test.get_s() == 1);
	EXPECT_TRUE(test.get_total_weight() == 0);
}

TEST(DcsTest, TestUpdate){
	int N = 1000;
	int universe = 16384;
	vector<pair<int, int>> stream = random_int_stream_with_weight(N, 0, universe - 1, 0, 50);
	int total_weight = weight_from_stream(stream, false);
	dcs test(0.3, universe);

	for(auto& it : stream){
		test.update(it.first, it.second);
	}

	ASSERT_TRUE(total_weight == test.get_total_weight());
}

TEST(DcsTest, TestQueryAndBounds){
    int N = 1000;
	double error = 0.1;
	int attempts = 100;
    int universe = 16384;
	// fazer funções mais distribuidas da entrada e verificar como fica a saida do algortimo
	// se tem haver com a função da entrada
	// testar tambem se as saidas do algoritmo são parecidas pro que ta sendo mostrado
	vector<pair<int, int>> stream = random_int_stream_with_weight(N, 0, universe - 1, -10, 30);
	vector<int> real_ranks = real_ranks_from_stream_with_weight(stream);
	vector<int> fails(real_ranks.size(), 0);
    int total_weight = weight_from_stream(stream, false);

	for(int i = 0; i < attempts; i++){
		dcs test(error, universe);

		for(auto& it : stream){
			test.update(it.first, it.second);
		}

		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = test.query(j);
			int real_rank = real_ranks[j];

			// pegar o valor do erro aqui , e montar um grafico
			// para ver o quanto esse erro se relaciona a um erro 
			// relativo de forma epsilon * real_rank
			if(fabs(approximated_rank - real_rank) > (error * total_weight)){
				fails[j]++;
			}
		}
	}

	for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), error / log2(universe));
		cout << setprecision(7);
		cout << (fails[i]/(double) attempts) << endl;
	}
}

TEST(DcsTest, TestQuantile){
	int N = 1000;
	double error = 0.1;
	int attempts = 100;
    int universe = 16384;
	vector<pair<int, int>> stream = random_int_stream_with_weight(N, 0, universe - 1, -10, 30);
	vector<int> fails(10, 0);// [0.0, 1.0] adding 0.1
    int total_weight = weight_from_stream(stream, false);

	for(int i = 0; i < attempts; i++){
		dcs test(error, universe);

		for(auto& it : stream){
			test.update(it.first, it.second);
		}

		for(double j = 0.0; j < 1.0; j += 0.1){
			int approximated_quantile = test.quantile(j);
			int real_quantile = quantile(stream, total_weight, j);

			if(fabs(approximated_quantile - real_quantile) > (error * total_weight)){
				fails[j]++;
			}
		}
	}

	for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), error / log2(universe));
	}
}


TEST(DcsTest, TestErrorFromRandomStream){
	int N = 1000;
	double error = 0.1;
	int attempts = 100;
    int universe = 16384;
	double max_diff = -1;
	double most_fail = -1;
	double max_mean_error = 0;
	int item_with_most_fail = -1;
	vector<pair<int, int>> stream = random_int_stream_with_weight(N, 0, universe - 1, -10, 50);
	stream = sort_and_merge_weight(stream);
	vector<int> real_ranks = real_ranks_from_stream_with_weight(stream);
	vector<int> fails(real_ranks.size(), 0);
    int total_weight = weight_from_stream(stream, false);

	vector<double> values;
	vector<double> sample_real_ranks;
	vector<double> mean_dcs_ans(real_ranks.size(), 0.0);
	vector<double> mean_dcs_error(real_ranks.size(), 0.0);

	for(int j = 0; j < real_ranks.size(); j++){
		values.push_back((double) j);
		sample_real_ranks.push_back((double) real_ranks[j]);
	}

	EXPECT_EQ(values.size(), real_ranks.size());
	EXPECT_EQ(sample_real_ranks.size(), real_ranks.size());
	EXPECT_EQ(mean_dcs_error.size(), real_ranks.size());


	for(int i = 0; i < attempts; i++){
		dcs test(error, universe);

		for(auto& it : stream){
			test.update(it.first, it.second);
		}

		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = test.query(j);
			int real_rank = real_ranks[j];
			double diff = fabs(approximated_rank - real_rank);

			max_diff = max(diff, max_diff);
			mean_dcs_ans[j] += approximated_rank;
			mean_dcs_error[j] += diff;

			if(diff > (error * total_weight)){
				fails[j]++;
			}
		}
	}

	for(int i = 0; i < mean_dcs_error.size(); i++){
		mean_dcs_error[i] = mean_dcs_error[i] / (double) attempts;
		mean_dcs_ans[i] = mean_dcs_ans[i] / (double) attempts;
	}

	for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), (error / log2(universe)));
		if((fails[i]/(double) attempts) > most_fail){
			most_fail = (fails[i]/(double) attempts);
			item_with_most_fail = i;
		}
	}

	for(auto& it: mean_dcs_error){
		max_mean_error = max(max_mean_error, it);
	}

	cout << "Testing dcs using uniform int distribution : " << endl;
	cout << "Max diff on stream tested := " << max_diff << endl;
	cout << "Max mean diff on stream tested := " << max_mean_error << endl;
	cout << "error * W := " << error * total_weight << endl;
	cout << "error / log2 (U) := " << (error / log2(universe)) << endl;
	cout << "Most fail on stream := " << item_with_most_fail << " with failure of " << most_fail * 100.0 << "%" << endl;

}

TEST(DcsTest, TestErrorSimilarities){
	int N = 1000;
	vector<double> errors = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
	int attempts = 100;
    int universe = 4096;

	for(auto& error : errors){
		vector<pair<int, int>> stream = random_int_stream_with_weight(N, 0, universe - 1, -10, 30);
		stream = sort_and_merge_weight(stream);
		vector<int> real_ranks = real_ranks_from_stream_with_weight(stream);
		vector<int> fails(real_ranks.size(), 0);
		int total_weight = weight_from_stream(stream, false);
		vector<double> values;
		vector<double> sample_real_ranks;
		vector<double> mean_dcs_error(real_ranks.size(), 0.0);
		vector<double> errors_on_rank(real_ranks.size(), 0.0);
		
		for(int j = 0; j < real_ranks.size(); j++){
			values.push_back((double) j);
			sample_real_ranks.push_back((double) real_ranks[j]);
		}

		EXPECT_EQ(values.size(), real_ranks.size());
		EXPECT_EQ(sample_real_ranks.size(), real_ranks.size());
		EXPECT_EQ(mean_dcs_error.size(), real_ranks.size());
		EXPECT_EQ(errors_on_rank.size(), real_ranks.size());
		
		for(int i = 0; i < attempts; i++){
			dcs test(error, universe);

			for(auto& it : stream){
				test.update(it.first, it.second);
			}

			for(int j = 0; j < real_ranks.size(); j++){
				int approximated_rank = test.query(j);
				int real_rank = real_ranks[j];

				double diff = fabs(approximated_rank - real_rank);

				mean_dcs_error[j] += diff;
				errors_on_rank[j] = real_rank * error;

				if(diff > (error * total_weight)){
					fails[j]++;
				}
			}
		}

		for(int i = 0; i < mean_dcs_error.size(); i++){
			mean_dcs_error[i] = mean_dcs_error[i] / (double) attempts;
		}

		for(int i = 0; i < fails.size(); i++){
			EXPECT_LT((fails[i]/(double) attempts), error / log2(universe));
		}

	}
}

TEST(DcsTest, TestErrorFromNormalStream){
	int N = 1000;
	double error = 0.3;
	int attempts = 100;
    int universe = 16384;
	double max_diff = -1;
	double most_fail = -1;
	double max_mean_error = 0;
	int item_with_most_fail = -1;
	vector<pair<int, int>> stream = normal_int_stream_with_weight(N, universe / 2, 100, -10, 50);
	stream = sort_and_merge_weight(stream);
	vector<int> real_ranks = real_ranks_from_stream_with_weight(stream);
	vector<int> fails(real_ranks.size(), 0);
    int total_weight = weight_from_stream(stream, false);

	vector<double> values;
	vector<double> sample_real_ranks;
	vector<double> mean_dcs_ans(real_ranks.size(), 0.0);
	vector<double> mean_dcs_error(real_ranks.size(), 0.0);

	for(int j = 0; j < real_ranks.size(); j++){
		values.push_back((double) j);
		sample_real_ranks.push_back((double) real_ranks[j]);
	}

	EXPECT_EQ(values.size(), real_ranks.size());
	EXPECT_EQ(sample_real_ranks.size(), real_ranks.size());
	EXPECT_EQ(mean_dcs_error.size(), real_ranks.size());


	for(int i = 0; i < attempts; i++){
		dcs test(error, universe);

		for(auto& it : stream){
			test.update(it.first, it.second);
		}

		for(int j = 0; j < real_ranks.size(); j++){
			int approximated_rank = test.query(j);
			int real_rank = real_ranks[j];
			double diff = fabs(approximated_rank - real_rank);

			max_diff = max(diff, max_diff);
			mean_dcs_ans[j] += approximated_rank;
			mean_dcs_error[j] += diff;

			if(diff > (error * total_weight)){
				fails[j]++;
			}
		}
	}

	for(int i = 0; i < mean_dcs_error.size(); i++){
		mean_dcs_error[i] = mean_dcs_error[i] / (double) attempts;
		mean_dcs_ans[i] = mean_dcs_ans[i] / (double) attempts;
	}

	for(int i = 0; i < fails.size(); i++){
		EXPECT_LT((fails[i]/(double) attempts), (error / log2(universe)));
		if((fails[i]/(double) attempts) > most_fail){
			most_fail = (fails[i]/(double) attempts);
			item_with_most_fail = i;
		}
	}

	for(auto& it: mean_dcs_error){
		max_mean_error = max(max_mean_error, it);
	}

	cout << "Testing dcs using normal int distribution : " << endl;
	cout << "Max diff on stream tested := " << max_diff << endl;
	cout << "Max mean diff on stream tested := " << max_mean_error << endl;
	cout << "error * W := " << error * total_weight << endl;
	cout << "error / log2 (U) := " << (error / log2(universe)) << endl;
	cout << "Most fail on stream := " << item_with_most_fail << " with failure of " << most_fail * 100.0 << "%" << endl;

}

TEST(DcsTest, TestFailPlot){
	int N = 1000;
	vector<double> errors = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
	int attempts = 100;
    int universe = 16384;

	for(auto& error : errors){
		vector<pair<int, int>> stream = random_int_stream_with_weight(N, 0, universe - 1, -100, 100);
		stream = sort_and_merge_weight(stream);
		vector<int> real_ranks = real_ranks_from_stream_with_weight(stream);
		vector<int> fails(real_ranks.size(), 0);
		int total_weight = weight_from_stream(stream, false);

		vector<double> values;
		vector<double> bounds(real_ranks.size(), 0.0);
		vector<double> failure(real_ranks.size(), 0.0);
		
		for(int j = 0; j < real_ranks.size(); j++){
			values.push_back((double) j);
			bounds[j] = (error / log2(universe));
		}

		EXPECT_EQ(values.size(), real_ranks.size());
		EXPECT_EQ(bounds.size(), real_ranks.size());
		EXPECT_EQ(failure.size(), real_ranks.size());

		for(int i = 0; i < attempts; i++){
			dcs test(error, universe);

			for(auto& it : stream){
				test.update(it.first, it.second);
			}

			for(int j = 0; j < real_ranks.size(); j++){
				int approximated_rank = test.query(j);
				int real_rank = real_ranks[j];

				double diff = fabs(approximated_rank - real_rank);

				if(diff > (error * total_weight)){
					fails[j]++;
				}
			}
		}

		double big_fail = -1;
		for(int i = 0; i < fails.size(); i++){
			EXPECT_LT((fails[i]/(double) attempts), error / log2(universe));
			failure[i] = (fails[i]/(double) attempts);
			
			big_fail = max(big_fail, failure[i]);
		}

		cout << big_fail << endl;	
	}
}

TEST(DcsTest, TestFailCondition){
	int N = 1000;
	vector<double> errors = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
	int attempts = 100;
    int universe = 16384;

	for(auto& error : errors){
		vector<pair<int, int>> stream = random_int_stream_with_weight(N, 0, universe - 1, -100, 100);
		stream = sort_and_merge_weight(stream);
		vector<int> real_ranks = real_ranks_from_stream_with_weight(stream);
		vector<int> fails(real_ranks.size(), 0);
		int total_weight = weight_from_stream(stream, false);

		vector<double> values;
		vector<double> error_to_fail(real_ranks.size(), 0.0);
		vector<double> dcs_mean_error(real_ranks.size(), 0.0);
		vector<double> dcs_max_error(real_ranks.size(), 0.0);
		
		for(int j = 0; j < real_ranks.size(); j++){
			values.push_back((double) j);
			error_to_fail[j] = (error * total_weight);
		}

		EXPECT_EQ(values.size(), real_ranks.size());
		EXPECT_EQ(error_to_fail.size(), real_ranks.size());
		EXPECT_EQ(dcs_mean_error.size(), real_ranks.size());
		EXPECT_EQ(dcs_max_error.size(), real_ranks.size());

		for(int i = 0; i < attempts; i++){
			dcs test(error, universe);

			for(auto& it : stream){
				test.update(it.first, it.second);
			}

			for(int j = 0; j < real_ranks.size(); j++){
				int approximated_rank = test.query(j);
				int real_rank = real_ranks[j];

				double diff = fabs(approximated_rank - real_rank);

				dcs_mean_error[j] += diff;
				dcs_max_error[j] = max(dcs_max_error[j], diff);

				if(diff > (error * total_weight)){
					fails[j]++;
				}
			}
		}

		for(int i = 0; i < dcs_mean_error.size(); i++){
			dcs_mean_error[i] = dcs_mean_error[i] / (double) attempts;
		}

		for(int i = 0; i < fails.size(); i++){
			EXPECT_LT((fails[i]/(double) attempts), error / log2(universe));
		}

	}
}

TEST(DcsTest, MemoryTest){
	int first_universe = 4096;
	vector<double> errors = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
	vector<int> universes;
	vector<double> values;

	for(int i = 0; i < 10; i++){
		universes.push_back(first_universe);
		values.push_back((double) first_universe);
		first_universe = first_universe << 1;
	}

	for(auto& error : errors){
		vector<double> memory_dcs;
		vector<double> memory_brute;

		for(auto& universe : universes){
			dcs test(error, universe);
			double memory_use = test.get_w() * test.get_d() * log2(universe);
			memory_dcs.push_back(memory_use);
			memory_brute.push_back((double) universe / 100);
		}

		EXPECT_EQ(memory_dcs.size(), values.size());
		EXPECT_EQ(memory_brute.size(), values.size());
	}
	
}