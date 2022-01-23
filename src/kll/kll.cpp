#include "kll.hpp"

namespace qsbd {
	template<class T>
	kll<T>::kll(double err, double delt, double multiplier) : coin(0, 1){
		ASSERT(multiplier > 0.5 and multiplier < 1.0);
		error = err;
		delta = delt;
		buffer_diff = multiplier;
		N = 0;
		double capacity_const = (std::pow(buffer_diff, 2) * (2 * buffer_diff - 1));
		capacity_const = 1.0 / sqrt(capacity_const);
		capacity_max = (capacity_const * sqrt(log(2.0 / delta))) / error;
		height = 0;
		buffers_array.push_back({});   
	}

	template<class T>
	void kll<T>::insert_sorted(std::vector<T>& buffer, const T& elem){
		buffer.insert(upper_bound(buffer.begin(), buffer.end(), elem), elem);
	}

	template<class T>
	void kll<T>::compress(){
		for(int l = 0; l <= height; l++){
			if(buffers_array[l].size() >= std::max(2.0, std::pow(buffer_diff, height - l) * capacity_max)){
				if(l == height){
					buffers_array.push_back({});
				}

				for(auto i = buffers_array[l].begin() + coin(generator); i < buffers_array[l].end(); i = next(i, 2)){
					kll<T>::insert_sorted(buffers_array[l + 1], (*i));
				}
				
				buffers_array[l].clear();
			}
		}

		if((buffers_array.size() - 1) != height){
			height++;
		}
	}

	template<class T>
	void kll<T>::update(T elem){
		this->N++;
		kll<T>::insert_sorted(buffers_array[0], elem);
		this->compress();
	}

	template<class T>
	int kll<T>::query(T elem){
		int rank = 0;

		for(int i = 0; i <= height; i++){
			auto it = upper_bound(buffers_array[i].begin(), buffers_array[i].end(), elem);

			rank += (it - buffers_array[i].begin()) * (1 << i);
		}

		return rank;
	}

	template<class T>
	T kll<T>::quantile(double quant){
		int64_t rank = (int64_t) this->N * quant;
		int64_t total_weight = 0;
		std::map<T, int64_t> weights;

		for(int64_t i = 0; i <= height; i++){
			for(int64_t j = 0; j < buffers_array[i].size(); j++){
				weights[buffers_array[i][j]] += (int64_t)(1LL << i);
			}
		}

		for(auto& it : weights){
			if(total_weight + it.second > rank) return it.first;
			else total_weight += it.second;
		}

		return T();
	}

	template<class T>
	quantile_sketch<T>* kll<T>::merge(quantile_sketch<T>& rhs){
		kll<T>& rhs_cv = dynamic_cast<kll<T>&> (rhs);

		if((rhs_cv.error - this->error) > 1e-6){
			DEBUG_ERR("in kll::merge kll's error need to match");
			return nullptr;
		}

		if((rhs_cv.delta - this->delta) > 1e-6){
			DEBUG_ERR("in kll::merge kll's error prob need to match");
			return nullptr;
		}

		if((rhs_cv.buffer_diff - this->buffer_diff) > 1e-6){
			DEBUG_ERR("in kll::merge kll's C constant need to match");
			return nullptr;
		}

		kll<T>* merged_summary = new kll<T>(rhs_cv.error);
		merged_summary->height = std::max(rhs_cv.height, this->height);

		for(int l = 0; l <= merged_summary->height; l++){
			if(l < merged_summary->height){
				merged_summary->buffers_array.push_back({});
			}
			
			int i = 0;
			int j = 0;

			bool have_in_rhs = l < rhs_cv.buffers_array.size();
			bool have_in_this = l < this->buffers_array.size();
			bool have_in_both_arrays = have_in_rhs && have_in_this;

			while (have_in_both_arrays &&  i < rhs_cv.buffers_array[l].size() && j < this->buffers_array[l].size()){
				if(rhs_cv.buffers_array[l][i] <= this->buffers_array[l][j]){
					merged_summary->buffers_array[l].push_back(rhs_cv.buffers_array[l][i]);
					i++;
				}else{
					merged_summary->buffers_array[l].push_back(this->buffers_array[l][j]);
					j++;
				}
			}

			while(have_in_rhs && i < rhs_cv.buffers_array[l].size()){
				merged_summary->buffers_array[l].push_back(rhs_cv.buffers_array[l][i]);
				i++;
			}

			while(have_in_this && j < this->buffers_array[l].size()){
				merged_summary->buffers_array[l].push_back(this->buffers_array[l][j]);
				j++;
			}
		}

		merged_summary->N = this->N + rhs_cv.N;
		merged_summary->compress();

		return merged_summary;
	}

	template<class T>
	void kll<T>::inner_merge(quantile_sketch<T>& rhs){
		kll<T>& rhs_cv = dynamic_cast<kll<T>&> (rhs);

		if((rhs_cv.error - this->error) > 1e-6){
			DEBUG_ERR("in kll::inner_merge kll's error need to match");
		}

		if((rhs_cv.delta - this->delta) > 1e-6){
			DEBUG_ERR("in kll::merge kll's error prob need to match");
		}

		if((rhs_cv.buffer_diff - this->buffer_diff) > 1e-6){
			DEBUG_ERR("in kll::merge kll's C constant need to match");
		}

		int merged_height = std::max(rhs_cv.height, this->height);
		std::vector<std::vector<T>> merged_buffer;
		merged_buffer.push_back({});

		for(int l = 0; l <= merged_height; l++){
			if(l < merged_height){
				merged_buffer.push_back({});
			}
			
			int i = 0;
			int j = 0;

			bool have_in_rhs = l < rhs_cv.buffers_array.size();
			bool have_in_this = l < this->buffers_array.size();
			bool have_in_both_arrays = have_in_rhs && have_in_this;

			while (have_in_both_arrays &&  i < rhs_cv.buffers_array[l].size() && j < this->buffers_array[l].size()){
				if(rhs_cv.buffers_array[l][i] <= this->buffers_array[l][j]){
					merged_buffer[l].push_back(rhs_cv.buffers_array[l][i]);
					i++;
				}else{
					merged_buffer[l].push_back(this->buffers_array[l][j]);
					j++;
				}
			}

			while(have_in_rhs && i < rhs_cv.buffers_array[l].size()){
				merged_buffer[l].push_back(rhs_cv.buffers_array[l][i]);
				i++;
			}

			while(have_in_this && j < this->buffers_array[l].size()){
				merged_buffer[l].push_back(this->buffers_array[l][j]);
				j++;
			}
		}

		this->height = merged_height;
		this->N += rhs_cv.N;
		this->buffers_array.clear();
		this->buffers_array = std::move(merged_buffer);

		this->compress();
	}

	//DEBUG

	template<class T>
	void kll<T>::print(){
		for(int l = 0; l <= height; l++){
			std::cout << "height [" << l << "]: ";
			for(auto& i : buffers_array[l]){
				std::cout << i << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	template<class T>
	void kll<T>::set_buffers(std::vector<std::vector<T>>& buffers){
		buffers_array = buffers;
		height = buffers.size() - 1;
	}

	template<class T>
	std::vector<std::vector<T>> kll<T>::get_buffers(){
		return buffers_array;
	}

	template<class T>
	double kll<T>::get_error(){
		return error;
	}

	template<class T>
	double kll<T>::get_delta(){
		return delta;
	}

	template<class T>
	int kll<T>::get_N(){
		return N;
	}

	template<class T>
	double kll<T>::get_capacity(){
		return capacity_max;
	}

	template<class T>
	uint64_t kll<T>::get_heap_size(){
		uint64_t kll_hs = sizeof(kll<T>);
		uint64_t ba_hs = 0;

		for(int i = 0; i < buffers_array.size(); i++){
			ba_hs += sizeof(std::vector<T>) + sizeof(T) * buffers_array[i].capacity();
		}

		return kll_hs + ba_hs; 
	}

	template class kll<int>;
	template class kll<double>;
}