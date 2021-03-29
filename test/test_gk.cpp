#include <gtest/gtest.h>
#include <gk/gk.hpp>
#include <gk_factory/gk_factory.hpp>
#include <utils/utils.h>
using namespace std;
using namespace qsbd;

deque<string> g_args;
/*
	Makes a random stream with length vector_size using elements from [min_v..max_v]
*/
vector<int> make_random_int_stream(int vector_size, int min_v, int max_v){
	uniform_int_distribution<int> who_pick(min_v, max_v);
	vector<int> random_stream;

	for(int i=0;i<vector_size;i++){
		random_stream.push_back(who_pick(generator));
	}

	return random_stream;
}

/*
	Calculates the rank(x), for a given positive stream
*/
vector<int> ranks_from_stream(vector<int> stream, int min_v,int max_v){
    vector<int> weights(max_v + 1, 0);
    vector<int> ranks(max_v + 1, 0);

    for(int i = 0; i < stream.size(); i++){
        weights[stream[i]]++;    
	}

    for(int i = 0, j = 1;j <= max_v; i++, j++){
        ranks[j] = weights[i] + ranks[i];
    }

    for(int i = 0; i < ranks.size(); i++){
        ranks[i] += weights[i] - 1;
    }

    cout << "Weights : " << endl;
    for(auto& it : weights){
        cout << it << " ";
    }
    cout << endl;

    cout << "Ranks : " << endl;
    for(auto& it : ranks){
        cout << it << " ";
    } 
    cout << endl;

	return ranks;
}

/*
    Prints a simple star histogram using the samples in the arg. The stars are bounded by the scale arg
*/
void histogram_print(const vector<double>& samples, int scale = 1){
    for(int i = 0; i < samples.size(); i++){
        cout << i << ":\t" << string(floor((samples[i] + 1) / scale), '*') << endl;
    }
}

/*
    Test if the ranks_from_stream is working   
*/
void test_real_ranks(){
    vector<int> input = {1, 4, 2, 8, 5, 7, 6, 7, 6, 7, 2, 1};
    vector<int> querys = {1, 2, 4, 5, 6, 7, 8};
    vector<int> real_rank = {0, 2, 4, 5, 6, 8, 11};
    vector<int> ranks = ranks_from_stream(input, 1, 8);

    cout << "From function : " << endl;

    for(int i = 0; i < ranks.size(); i++){
        cout << "rank " << i << ":\t" << ranks[i] << endl;
    }

    cout << "-------------------------" << endl;
    cout << "From paper : " << endl;

    for(int i = 0; i < real_rank.size(); i++){
        cout <<"rank " << querys[i] << ":\t" << real_rank[i] << endl;
    }
}


/*
    Second test to see if the ranks_from_stream is working   
*/
void test_real_ranks2(int vector_size, int min_v, int max_v){
    vector<int> stream = make_random_int_stream(vector_size, min_v, max_v);  
    vector<int> real_ranks = ranks_from_stream(stream, min_v, max_v);

    sort(stream.begin(),stream.end());

    for(auto& it: stream){
        cout << it << " ";
    }
    cout << endl;
    cout << endl;

    for(int i = 0; i < real_ranks.size(); i++){
        cout << i << " : " << real_ranks[i] << endl;
    }
    cout << endl;
}

/*
    Tests the make_random_int_stream function
*/
void test_stream_creation(int vector_size, int min_v, int max_v){
    vector<int> stream = make_random_int_stream(vector_size, min_v, max_v);

    for(auto& it : stream){
        cout << it << " ";
    }
    cout << endl;
}

/*
    Tests tuple insertion on multimap
*/
TEST(GkTest, TestTupleInsert){
    vector<pair<int, pair<int, int>>> tuple_list;
    vector<int> input = {1, 4, 2, 8, 5, 7, 6, 7, 6, 7, 2, 1};

    for(auto& it : input){
        insert_sorted(tuple_list, {it, {1, 10}});
    }

    for(auto& it : tuple_list){
        cout << "(" << it.first << "," << it.second.first << "," << it.second.second << ") ";
    }
    cout << endl;

    auto up_it = upper_bound(tuple_list.begin(), tuple_list.end(), 7, [](const int& data, std::pair<int, std::pair<int, int>> value){
        return (data < value.first);
    });

    int index = distance(tuple_list.begin(), up_it);

    cout << "(" << tuple_list[index].first << "," << tuple_list[index].second.first << "," << tuple_list[index].second.second << ") ";
    cout << endl;

    insert_sorted(tuple_list, std::make_pair(8, std::make_pair(0, 10)));

    for(auto& it : tuple_list){
        cout << "(" << it.first << "," << it.second.first << "," << it.second.second << ") ";
    }
    cout << endl;
}


/*
    Tests the merge from two summarys
*/
TEST(GKTest, TestMerge){
    vector<int> input1 = {1, 4, 2, 8, 5, 7, 6, 7, 6, 7, 2, 1, 3, 5, 6, 9, 9, 10};
    vector<int> input2 = {10, 11, 14, 12, 18, 15, 17, 16, 17, 16, 17, 12, 11, 13, 15, 16, 19, 19};
    vector<double> samples(20, 0);
    gk<int> summary1(0.2);
    gk<int> summary2(0.2);

    for(auto& it : input1){
        summary1.update(it);
    }

    for(int i = 0; i < 20; i++){
        samples[i] = (double) summary1.query(i);
    }

    cout << "Histogram ranks 1:" << endl;
    histogram_print(samples);

    for(auto& it : input2){
        summary2.update(it);
    }

    for(int i = 0; i < 20; i++){
        samples[i] = (double) summary2.query(i);
    }

    cout << "Histogram ranks 2:" << endl;
    histogram_print(samples);

    gk<int>* merged = dynamic_cast<gk<int>*>(summary1.merge(summary2));

    for(int i = 0; i < 20; i++){
        samples[i] = merged->query(i);
    }

    cout << "Histogram ranks merged:" << endl;
    histogram_print(samples);
}

/*
    Testing updates and querys from the paper
*/
TEST(GkTest, TestUpdateAndQuery){
    vector<int> input = {1, 4, 2, 8, 5, 7, 6, 7, 6, 7, 2, 1};
    vector<int> querys = {1, 2, 4, 5, 6, 7, 8};
    vector<int> real_ranks = {0, 2, 4, 5, 6, 8, 11};
    gk<int> summary(0.2);

    cout << "None" << " " << summary.get_N() << " " << (2 * 0.2 * summary.get_N()) << ": ";
    summary.print();
    cout << endl;

    for(int i = 0; i < input.size(); i++){
        summary.update(input[i]);
        cout << input[i] << " " << summary.get_N() << " " << (2 * 0.2 * summary.get_N()) << ": ";
        summary.print();
        cout << endl;
    }

    assert(querys.size() == real_ranks.size());

    cout << "x\trank(x)\t~r(x)\terror" << endl;
    for(int i = 0; i < querys.size(); i++){
        double app_rank = summary.query(querys[i]);
        cout << querys[i] << "\t" << real_ranks[i] << "\t" << app_rank << "\t" << abs(app_rank - real_ranks[i]) << endl; 
    }
}

/*
    Testing the gk summary for int numbers
*/
void test_gk_summary(double epsilon, int vector_size, int min_v, int max_v, bool print){

    double epsilon, int vector_size, int min_v, int max_v, bool print
    vector<int> stream = make_random_int_stream(vector_size, min_v, max_v);
    vector<int> real_ranks = ranks_from_stream(stream, min_v, max_v);
    vector<double> samples(real_ranks.size(),0);
    gk<int> summary(epsilon);

    for(auto &it : stream){
        summary.update(it);
    }

    for(int i = min_v; i <= max_v; i++){
        double app_rank = summary.query(i);
        samples[i] = app_rank;

        cout << i << "\t" << real_ranks[i] << "\t" << app_rank << "\t" << abs(app_rank - real_ranks[i]) << "\t";
        cout << "[" << (app_rank - summary.get_N() * epsilon) << " <= " << real_ranks[i] << " <= " << (app_rank + summary.get_N() * epsilon) << "]" << endl;
        assert((app_rank - summary.get_N() * epsilon) <= real_ranks[i] and real_ranks[i] <= (app_rank + summary.get_N() * epsilon));
    }
}


TEST(GkTest, TestFactory){
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