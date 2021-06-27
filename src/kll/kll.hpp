#ifndef QSBD_KLL_H
#define QSBD_KLL_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd {
	template<class T>
	class kll : public quantile_sketch<T>{
	protected:
		
		std::uniform_int_distribution<int> coin;
		std::vector<std::vector<T>> buffers_array;
		int height;
		double buffer_diff;
		double capacity_max;
		double error;
		
	public:
		kll(double err);

		static void insert_sorted(std::vector<T> &buffer, const T &elem);
		void compress();
		void update(T elem) override;
		int query(T elem) override;
		quantile_sketch<T>* merge(quantile_sketch<T>& rhs) override;
		void inner_merge(quantile_sketch<T>& rhs) override;

		//DEBUG
		void print();
		void set_buffers(std::vector<std::vector<T>>& buffers);
		std::vector<std::vector<T>> get_buffers();
		double get_error();
		double get_capacity();
		uint64_t get_heap_size() override;
	};
}
#endif