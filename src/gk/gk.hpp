/**
 * @file gk.hpp
 * @brief QSBD gk class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.5
 *
 */

#ifndef QSBD_GK_H
#define QSBD_GK_H
#include "../global_generator/global_generator.h"
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../qsbd_debug/qsbd_debug.h"
#include "../utils/utils.h"

namespace qsbd {

	/** @class gk\<T\>
	 * @brief A class to represent the Greenwald-Khanna sketch
	 *  
	 * The GK summary provides a compact summary of data drawn from an ordered input domain U. It's a
	 * comparison-based algorithm, it works on any ordered domain U where comparison operator is defined.
	 * But it only accepts insertions of unweighted elements. So here the total number of elements inserted
	 * into the summary, N, is the same as the total weight W. This summary can be merged but its size grows
	 * after each MERGE.
	 * 
	 * @tparam T The object type for the gk
	*/
	template <class T>
	class gk : public quantile_sketch<T> {
	private:
		std::vector<std::pair<T, std::pair<int, int>>> tuple_list; //!< A list of tuples for the elements (element, number of itens represented, uncertainty)
		int N;
		double epsilon;

		/**
		 * @brief A private method function that returns the smallest index for a element in @p vec such this element is greater than @p data
		 * @param vec Some vector organized as the tuple list for the GK summary
		 * @param data A element to be queried
		 * @return The smallest index on the list such that the element \f$x_{index}\f$ > @p data 
		*/
		int find_smallest_i(const std::vector<std::pair<T, std::pair<int, int>>>& vec, const T& data){
			auto up_it = std::upper_bound(vec.begin(), vec.end(), data, [](const T& data, std::pair<T, std::pair<int, int>> value){
				return (data < value.first);
			});

			return std::distance(vec.begin(), up_it);
		}

	public:

		/**
		 * @brief A constructor for the GK summary
		 * @param eps The error used in the sketch
		 * 
		 * @note
		 * The object type T should define std\::numeric_limits\<T\>::max()
		*/
		gk(double eps){
			N = 0;
			epsilon = eps;

			tuple_list.insert(tuple_list.begin(), std::make_pair(std::numeric_limits<T>::max(), std::make_pair(1, 0)));
		}

		/**
		 * @brief The destructor for the GK summary
		*/
		~gk(){}

		/**
		 * @brief A method to update the summary
		 * @param value A object of type \<T\>
		*/
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
				// see if this inserts in the i position
				tuple_list.insert(tuple_list.begin() + index, {value, {1, weight + uncertainty - 1}});

				for(int i = 0, j = 1; j < tuple_list.size() - 1; i++, j++){
					if ((tuple_list[i].second.first + tuple_list[j].second.first + tuple_list[j].second.second) < capacity){
						tuple_list[j].second.first += tuple_list[i].second.first;

						tuple_list.erase(tuple_list.begin() + i);
					}
				} 
			}
			
			return;
		}

		/**
		 * @brief A method that returns the rank for some element @p value
		 * @param value The element to be queried
		 * @return The rank for such element
		*/
		int query(T value) override {
			int weight_sum = 0;
			int index = 0;

			while(value >= tuple_list[index].first){
				weight_sum += tuple_list[index].second.first;
				index++;
			}
			
			return (weight_sum - 1 + ((tuple_list[index].second.first + tuple_list[index].second.second) / 2));
		}

		/**
		 * @brief Quantile method. Receives a @p quant and returns a element associate with this quantile.
		 * @param quant The quantile to be queried
		 * @return The element associete with the quantile @p quant
		 * @warning 
		 * The quantile should be in range [0..1]
		*/
		T quantile(double quant) override {
			int rank = (int) this->N * quant;

			if((rank + 1) <= (N - (epsilon * N))){
				int weight_sum = 0;
				int index = 0;

				while((weight_sum + tuple_list[index].second.first + tuple_list[index].second.second) <= (rank + (epsilon * N) + 1) and index < tuple_list.size()){
					weight_sum += tuple_list[index].second.first;
					index++;
				}

				if(index) return tuple_list[index - 1].first;
				else return tuple_list[index].first; 
			}else{
				if(tuple_list.size() > 1) return tuple_list[tuple_list.size() - 2].first;
				else return T();
			}

			return T();
		}

		/**
		 * @brief CDF method. Receives a @p elem and returns a probability for some value be less than this value. 
		 * @param elem The element to be queried
		 * @return The probability for this element
		*/
		double cdf(T elem) override {
			if (not this->N) return 0.0;

			return this->query(elem) / (double) this->N;
		}

		/**
		 * @brief Merges this instance of gk with another gk instance
		 * @param rhs A gk\<T\> instance to be merged
		 * @return A pointer to the new instance of a gk 
		 * @sa gk\<T\>::inner_merge(quantile_sketch\<T\>&)
		 * @warning 
		 * The error in both instance needs to be the same. The error will be considered as the same
		 * if \f$|this->error - rhs.error| \leq 1e-6\f$
		 * 
		 * @warning 
		 * The quantile_sketch must be a instance of the gk class
		 * 
		 * @warning 
		 * Is the user responsabilite of frees the returned pointer
		 * 
		*/
		quantile_sketch<T>* merge(quantile_sketch<T>& rhs) override {
			gk<T>& rhs_cv = dynamic_cast<gk<T>&>(rhs); 

			ASSERT(rhs_cv.epsilon - this->epsilon < 1e-6);

			int left_index = 0;
			int right_index = 0; 
			gk<T>* merged_summary = new gk<T>(epsilon);
			merged_summary->tuple_list.clear();

			while(left_index < tuple_list.size() - 1 and right_index < rhs_cv.tuple_list.size() - 1){

				if(tuple_list[left_index].first < rhs_cv.tuple_list[right_index].first){
					T value = tuple_list[left_index].first;
					int weight = tuple_list[left_index].second.first;
					int uncertainty = tuple_list[left_index].second.second + rhs_cv.tuple_list[right_index].second.first + rhs_cv.tuple_list[right_index].second.second - 1; 
					
					merged_summary->tuple_list.push_back({value, {weight, uncertainty}});

					left_index++;
				}else{
					T value = rhs_cv.tuple_list[right_index].first;
					int weight = rhs_cv.tuple_list[right_index].second.first;
					int uncertainty = rhs_cv.tuple_list[right_index].second.second + rhs_cv.tuple_list[right_index].second.first + tuple_list[left_index].second.second - 1; 

					merged_summary->tuple_list.push_back({value, {weight, uncertainty}});

					right_index++;
				}
			}

			while(left_index < tuple_list.size() - 1){
				merged_summary->tuple_list.push_back(tuple_list[left_index]);

				left_index++;
			}

			while(right_index < rhs_cv.tuple_list.size() - 1){
				merged_summary->tuple_list.push_back(rhs_cv.tuple_list[right_index]);

				right_index++;
			}

			merged_summary->N = this->N + rhs_cv.N;
			merged_summary->tuple_list.push_back(std::make_pair(std::numeric_limits<T>::max(), std::make_pair(1, 0)));

			int capacity = (int) ceil(2.0 * epsilon * merged_summary->get_N());
			for(int i = 0, j = 1; j < merged_summary->tuple_list.size() - 1; i++, j++){
				if ((merged_summary->tuple_list[i].second.first + merged_summary->tuple_list[j].second.first + merged_summary->tuple_list[j].second.second) < capacity){
					merged_summary->tuple_list[j].second.first += merged_summary->tuple_list[i].second.first;

					merged_summary->tuple_list.erase(merged_summary->tuple_list.begin() + i);
				}
			} 


			return merged_summary;
		}

		/**
		 * @brief Merges this instance of gk with another gk instance and saves it in this instance
		 * @param rhs A gk\<T\> instance to be merged
		 * @sa gk\<T\>\::merge(quantile_sketch\<T\>&)
		 * @warning 
		 * The error in both instances needs to be the same. The error will be considered as the same
		 * if \f$|this->error - rhs.error| \leq 1e-6\f$
		 * 
		 * @warning 
		 * The quantile_sketch must be a instance of the gk class
		 * 
		*/
		void inner_merge(quantile_sketch<T>& rhs) override {
			gk<T>& rhs_cv = dynamic_cast<gk<T>&>(rhs); 

			ASSERT(rhs_cv.epsilon - this->epsilon < 1e-6);

			int left_index = 0;
			int right_index = 0; 
			int merged_N = 0; 

			std::vector<std::pair<T, std::pair<int, int>>> merged_tuple;

			while(left_index < tuple_list.size() - 1 and right_index < rhs_cv.tuple_list.size() - 1){

				if(tuple_list[left_index].first < rhs_cv.tuple_list[right_index].first){
					T value = tuple_list[left_index].first;
					int weight = tuple_list[left_index].second.first;
					int uncertainty = tuple_list[left_index].second.second + rhs_cv.tuple_list[right_index].second.first + rhs_cv.tuple_list[right_index].second.second - 1; 
					
					merged_tuple.push_back({value, {weight, uncertainty}});

					left_index++;
				}else{
					T value = rhs_cv.tuple_list[right_index].first;
					int weight = rhs_cv.tuple_list[right_index].second.first;
					int uncertainty = rhs_cv.tuple_list[right_index].second.second + rhs_cv.tuple_list[right_index].second.first + tuple_list[left_index].second.second - 1; 

					merged_tuple.push_back({value, {weight, uncertainty}});

					right_index++;
				}
			}

			while(left_index < tuple_list.size() - 1){
				merged_tuple.push_back(tuple_list[left_index]);

				left_index++;
			}

			while(right_index < rhs_cv.tuple_list.size() - 1){
				merged_tuple.push_back(rhs_cv.tuple_list[right_index]);

				right_index++;
			}

			merged_N = this->N + rhs_cv.N;
			merged_tuple.push_back(std::make_pair(std::numeric_limits<T>::max(), std::make_pair(1, 0)));

			int capacity = (int) ceil(2.0 * epsilon * merged_N);
			for(int i = 0, j = 1; j < merged_tuple.size() - 1; i++, j++){
				if ((merged_tuple[i].second.first + merged_tuple[j].second.first + merged_tuple[j].second.second) < capacity){
					merged_tuple[j].second.first += merged_tuple[i].second.first;

					merged_tuple.erase(merged_tuple.begin() + i);
				}
			}

			this->tuple_list = std::move(merged_tuple);
			this->N = merged_N;
		}

		/**
		 * @brief A debug method to print the tuple list in the summary on the standard ouput
		*/
		void print(){
			for (auto &it : tuple_list){
				std::cout << "(" << it.first << "," << it.second.first << "," << it.second.second << ") ";
			}
			std::cout << std::endl;
		}

		/**
		 * @brief Getter method to pick the total of elements inserted
		 * @return The total of elements inserted.
		*/
		int get_N(){
			return N;
		}

		/**
		 * @brief Getter for the estimated amount of heap memory allocated for this object for a pointer.
		 * @return The estimated memory in bytes.
		*/
		uint64_t get_heap_size() override {
			uint64_t gk_hs = sizeof(gk<T>);
			uint64_t tl_hs = sizeof(std::pair<T, std::pair<int, int>>) * tuple_list.capacity();

			return gk_hs + tl_hs;
		}
	};
}
#endif