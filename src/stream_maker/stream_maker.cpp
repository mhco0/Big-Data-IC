#include "stream_maker.h"

using namespace std;

vector<pair<int, int>> normal_int_stream_with_weight(int vector_size, double mean, double stddev, int min_w, int max_w){
	normal_distribution<double> who_pick(mean, stddev);
	uniform_int_distribution<int> weight(min_w, max_w);
	vector<pair<int, int>> normal_stream;
	map<int, int> weight_got;

	for(int i = 0; i < vector_size; i++){
		int element = (int) who_pick(generator);
		int new_weight = weight(generator);

		if(new_weight > 0){
			weight_got[element] += new_weight;
		}else{
			if((weight_got[element] + new_weight) < 0){
				new_weight = -new_weight;
			}
		}

		normal_stream.push_back({element, new_weight});
	}

	return normal_stream;
}

vector<pair<int, int>> random_int_stream_with_weight(int vector_size, int min_v, int max_v, int min_w, int max_w){
    uniform_int_distribution<int> who_pick(min_v, max_v);
    uniform_int_distribution<int> weight(min_w, max_w);
	vector<pair<int, int>> random_stream;
	map<int, int> weight_got;

	for(int i = 0; i < vector_size; i++){
		int element = who_pick(generator);
		int new_weight = weight(generator);

		if(new_weight > 0){
			weight_got[element] += new_weight;
		}else{
			if((weight_got[element] + new_weight) < 0){
				new_weight = -new_weight;
			}
		}

		random_stream.push_back({element, new_weight});
	}

	return random_stream;
}

vector<pair<int, int>> sort_and_merge_weight(vector<pair<int, int>>& stream){
	sort(stream.begin(), stream.end());
	vector<pair<int, int>>::iterator i = stream.begin();
	vector<pair<int, int>>::iterator j = next(stream.begin());

	for(;;){
		while(j != stream.end() and (*j).first == (*i).first){
			(*i).second += (*j).second;
			j = next(j);
		}

		if(next(i) != j) j = stream.erase(next(i), j);

		if(j == stream.end()) break;
		else{
			i = j;
			j = next(j);
		}
	}

	return stream;
}

vector<int> random_int_stream(int vector_size, int min_v, int max_v){
	uniform_int_distribution<int> who_pick(min_v, max_v);
	vector<int> random_stream;

	for(int i = 0; i < vector_size; i++){
		random_stream.push_back(who_pick(generator));
	}

	return random_stream;
}

vector<int> real_ranks_from_stream(vector<int>& stream){
	sort(stream.begin(), stream.end());

	int max_elem = stream[stream.size() - 1];
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

vector<int> real_ranks_from_stream_with_weight(vector<pair<int, int>>& stream){
	sort(stream.begin(), stream.end());

	int max_elem = stream[stream.size() - 1].first;
	vector<int> ranks(max_elem + 1, 0);

	for(int i = 0; i < stream.size(); i++){
		ranks[stream[i].first] += stream[i].second;
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

int weight_from_stream(vector<pair<int, int>>& stream, bool absolute){
	int total_w = 0;

	for(auto& it : stream){
		if(absolute) total_w += abs(it.second);
		else total_w += it.second;
	}

	return total_w;
}

map<int, int> frequency_counter(vector<int>& stream){
	map<int, int> mp;

	for(auto& it : stream){
		mp[it]++;
	}

	return mp;
}

map<int, int> frequency_counter(vector<pair<int, int>>& stream){
	map<int, int> mp;

	for(auto& it : stream){
		mp[it.first] += it.second;
	}

	return mp;
}

int quantile(vector<pair<int, int>>& stream, int total_weight, double quant){
	vector<pair<int, int>> stream_copy = stream;

	if(stream.size() == 0) return 0;

	sort(stream_copy.begin(), stream_copy.end());
	int index = (int) total_weight * quant;
	int acc = 0;

	for(auto& it : stream_copy){
		if(acc + it.second >= index) return it.first;
		else{
			acc += it.second;
		}
	}

	return -0x3f3f3f;
}

void show_stream(vector<int>& stream){
	for(auto& it : stream){
		cout <<  it << " ";
	}
	cout << endl;
}

void show_stream(vector<pair<int, int>>& stream){
	for(auto& it : stream){
		cout << "(" << it.first << ", " << it.second << ") "; 
	}
	cout << endl;
}