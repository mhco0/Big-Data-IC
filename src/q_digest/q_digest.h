#ifndef QSBD_Q_DIGEST_H
#define QSBD_Q_DIGEST_H
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../qsbd_debug/qsbd_debug.h"
#include "../utils/utils.h"
#include <cmath>
#include <iomanip>

namespace qsbd{
	class q_digest : public quantile_sketch<int> {
	public:
		class node{
		public:
			int weight;
			int left_child;

			node(int w, int lc);
		};

		class merge_error: public std::exception {
		public:
			const char* what() const throw();
		};

		std::vector<std::pair<int, int>> small_buffer;
		std::vector<q_digest::node> tree;
		bool transfered_buffer;
		int total_weight;
		int ceil_weight;
		int capacity;
		int universe;
		double error;

		int alloc_childs();
		void private_print(int cur_node, int indent = 0) const;
		void private_print_subtree_weights(int cur_node, int indent = 0);
		int subtree_weight_from(int cur_node);
		void set_subtree_weight_to_zero(int cur_node);
		bool in_range(const int& x, const int& l, const int& r);
		bool is_leaf(const int& x, const int& l, const int& r);
		void private_compress(int cur_node, int debt, int left_range, int right_range);
		void private_merge(const std::vector<q_digest::node>& left, const std::vector<q_digest::node>& right_tree, int cur_node, int left_node, int right_node);
		void private_update(int x, int weight);
		int private_query(int x);
		void insert_in_buffer(const int& x, const int& weight);
		void transfer_buffer_to_tree();
		int query_from_buffer(const int& x);

	public:
		q_digest(double error, int universe);
		q_digest(const q_digest& other) = delete;
		~q_digest();

		q_digest& operator=(const q_digest& other) = delete;

		void print(int indent = 0) const;
		void print_subtree_weights();
		void update(int x, int weight) override;
		int query(int x) override;
		void compress();
		quantile_sketch<int>* merge(quantile_sketch<int>& rhs) override;
		void inner_merge(quantile_sketch<int>& rhs) override;

		int get_total_weight() const;
	};
}
#endif