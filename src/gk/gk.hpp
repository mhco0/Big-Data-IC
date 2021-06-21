#ifndef QSBD_GK_H
#define QSBD_GK_H
#include "../global_generator/global_generator.h"
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../qsbd_debug/qsbd_debug.h"
#include "../utils/utils.h"

namespace qsbd {
	template <class T>
	class gk : public quantile_sketch<T> {
	private:
		std::vector<std::pair<T, std::pair<int, int>>> tuple_list;
		int N;
		double epsilon;

		int find_smallest_i(const std::vector<std::pair<T, std::pair<int, int>>>& vec, const T& data){
			auto up_it = std::upper_bound(vec.begin(), vec.end(), data, [](const T& data, std::pair<T, std::pair<int, int>> value){
				return (data < value.first);
			});

			return std::distance(vec.begin(), up_it);
		}

	public:
		gk(double eps){
			N = 0;
			epsilon = eps;

			insert_sorted(tuple_list, std::make_pair(std::numeric_limits<T>::max(), std::make_pair(1, 0)));
		}

		~gk(){}

		void update(T value) override {
			N += 1;

			//find smallest i such x[i] > value
			int index = find_smallest_i(tuple_list, value);

			int weight = tuple_list[index].second.first;
			int uncertainty = tuple_list[index].second.second;
			int capacity = (int) ceil(2.0 * epsilon * N);

			if((weight + uncertainty + 1) < capacity){
				tuple_list[index].second.first += 1;
			}else{

				insert_sorted(tuple_list, {value, {1, weight + uncertainty - 1}});

				for(int i = 0, j = 1; j < tuple_list.size(); i++, j++){
					if ((tuple_list[i].second.first + tuple_list[j].second.first + tuple_list[j].second.second) < capacity){
						tuple_list[j].second.first += tuple_list[i].second.first;

						tuple_list.erase(tuple_list.begin() + i);

						break;
					}
				} 
			}
			
			return;
		}

		quantile_sketch<T>* merge(quantile_sketch<T>& rhs) override {
			gk<T>& rhs_cv = dynamic_cast<gk<T>&>(rhs); 

			ASSERT(rhs_cv.epsilon - this->epsilon < 1e-6);

			int left_index = 0;
			int right_index = 0; 
			gk<T>* merged_summary = new gk<T>(epsilon);

			while(left_index < tuple_list.size() and right_index < rhs_cv.tuple_list.size()){
				merged_summary->N += 1;

				if(tuple_list[left_index].first < rhs_cv.tuple_list[right_index].first){
					T value = tuple_list[left_index].first;
					int weight = tuple_list[left_index].second.first;
					int uncertainty = tuple_list[left_index].second.second + rhs_cv.tuple_list[right_index].second.first + rhs_cv.tuple_list[right_index].second.second - 1; 
					
					insert_sorted(merged_summary->tuple_list, {value, {weight, uncertainty}});

					left_index++;
				}else{
					T value = rhs_cv.tuple_list[right_index].first;
					int weight = rhs_cv.tuple_list[right_index].second.first;
					int uncertainty = rhs_cv.tuple_list[right_index].second.second + rhs_cv.tuple_list[right_index].second.first + tuple_list[left_index].second.second - 1; 

					insert_sorted(merged_summary->tuple_list, {value, {weight, uncertainty}});

					right_index++;
				}
			}

			while(left_index < tuple_list.size()){
				merged_summary->N += 1;
				insert_sorted(merged_summary->tuple_list, tuple_list[left_index]);

				left_index++;
			}

			while(right_index < rhs_cv.tuple_list.size()){
				merged_summary->N += 1;
				insert_sorted(merged_summary->tuple_list, rhs_cv.tuple_list[right_index]);

				right_index++;
			}

			int capacity = (int) ceil(2.0 * epsilon * merged_summary->get_N());
			for(int i = 0, j = 1; j < merged_summary->tuple_list.size(); i++, j++){
				if ((merged_summary->tuple_list[i].second.first + merged_summary->tuple_list[j].second.first + merged_summary->tuple_list[j].second.second) < capacity){
					merged_summary->tuple_list[j].second.first += merged_summary->tuple_list[i].second.first;

					merged_summary->tuple_list.erase(merged_summary->tuple_list.begin() + i);
				}
			} 


			return merged_summary;
		}

		void inner_merge(quantile_sketch<T>& rhs) override {
			gk<T>& rhs_cv = dynamic_cast<gk<T>&>(rhs); 

			ASSERT(rhs_cv.epsilon - this->epsilon < 1e-6);

			int left_index = 0;
			int right_index = 0; 
			int merged_N = 0; 

			std::vector<std::pair<T, std::pair<int, int>>> merged_tuple;

			while(left_index < tuple_list.size() and right_index < rhs_cv.tuple_list.size()){
				merged_N += 1;

				if(tuple_list[left_index].first < rhs_cv.tuple_list[right_index].first){
					T value = tuple_list[left_index].first;
					int weight = tuple_list[left_index].second.first;
					int uncertainty = tuple_list[left_index].second.second + rhs_cv.tuple_list[right_index].second.first + rhs_cv.tuple_list[right_index].second.second - 1; 
					
					insert_sorted(merged_tuple, {value, {weight, uncertainty}});

					left_index++;
				}else{
					T value = rhs_cv.tuple_list[right_index].first;
					int weight = rhs_cv.tuple_list[right_index].second.first;
					int uncertainty = rhs_cv.tuple_list[right_index].second.second + rhs_cv.tuple_list[right_index].second.first + tuple_list[left_index].second.second - 1; 

					insert_sorted(merged_tuple, {value, {weight, uncertainty}});

					right_index++;
				}
			}

			while(left_index < tuple_list.size()){
				merged_N += 1;
				insert_sorted(merged_tuple, tuple_list[left_index]);

				left_index++;
			}

			while(right_index < rhs_cv.tuple_list.size()){
				merged_N += 1;
				insert_sorted(merged_tuple, rhs_cv.tuple_list[right_index]);

				right_index++;
			}

			int capacity = (int) ceil(2.0 * epsilon * merged_N);
			for(int i = 0, j = 1; j < merged_tuple.size(); i++, j++){
				if ((merged_tuple[i].second.first + merged_tuple[j].second.first + merged_tuple[j].second.second) < capacity){
					merged_tuple[j].second.first += merged_tuple[i].second.first;

					merged_tuple.erase(merged_tuple.begin() + i);
				}
			}

			this->tuple_list.clear();
			this->tuple_list = std::move(merged_tuple);
			this->N = merged_N;
		}

		int query(T value) override {
			int weight_sum = 0;
			int index = 0;

			while(value >= tuple_list[index].first){
				weight_sum += tuple_list[index].second.first;
				index++;
			}
			
			return (weight_sum - 1 + ((tuple_list[index].second.first + tuple_list[index].second.second) / 2));
		}

		//DEBUG
		void print(){
			for (auto &it : tuple_list){
				std::cout << "(" << it.first << "," << it.second.first << "," << it.second.second << ") ";
			}
			std::cout << std::endl;
		}

		//DEBUG
		int get_N(){
			return N;
		}

	};
}
#endif