#ifndef STREAM_MAKER_H
#define STREAM_MAKER_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"

std::vector<std::pair<int, int>> random_int_stream_with_weight(int vector_size, int min_v, int max_v, int min_w, int max_w);
std::vector<std::pair<int, int>> normal_int_stream_with_weight(int vector_size, double mean, double stddev, int min_w, int max_w);
std::vector<std::pair<int, int>> sort_and_merge_weight(std::vector<std::pair<int, int>>& stream);
std::vector<int> random_int_stream(int vector_size, int min_v, int max_v);
std::vector<int> real_ranks_from_stream(std::vector<int>& stream);
std::vector<int> real_ranks_from_stream_with_weight(std::vector<std::pair<int, int>>& stream);
int weight_from_stream(std::vector<std::pair<int, int>>& stream, bool absolute = true);
std::map<int, int> frequency_counter(std::vector<int>& stream);
std::map<int, int> frequency_counter(std::vector<std::pair<int, int>>& stream);
int quantile(std::vector<std::pair<int, int>>& stream, int total_weight, double quant);
std::vector<std::pair<int, std::pair<double, double>>> random_stream_in_region(int vector_size, double minx, double miny, double maxx, double maxy);
std::vector<std::pair<std::pair<int, int>, std::pair<double, double>>> random_stream_in_region_with_weight(int vector_size, double minx, double miny, double maxx, double maxy, int min_v, int max_v, int min_w, int max_w);

template<class T>
std::vector<T> merge_stream(std::vector<T>& stream1, std::vector<T>& stream2){
	std::vector<T> merged_stream;
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

//DEBUG
void show_stream(std::vector<int>& stream);
void show_stream(std::vector<std::pair<int, int>>& stream);

#endif