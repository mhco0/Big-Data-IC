#ifndef QSBD_Q_DIGEST_H
#define QSBD_Q_DIGEST_H
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../qsbd_debug/qsbd_debug.h"
#include "../utils/utils.h"
#include <cmath>
#include <iomanip>

namespace qsbd{
	class q_digest : public quantile_sketch<int> {
	private:
		/*class node{
		private:
			friend class q_digest;
			int weight;
			q_digest::node * left;
			q_digest::node * right;

			void private_print(q_digest::node* rt, int indent = 0);
			void private_sub_tree_weights_print(q_digest::node* rt, int indent);
			int sub_tree_weight_from(q_digest::node * rt);
			void delete_tree(q_digest::node * rt);
			void set_subtree_weight_to_zero(q_digest::node * rt);

		public:
			node(int weight);
			~node();

			bool is_leaf();
			void print(int indent = 0);
			void print_subtree_weights();

			//DEBUG
			/*friend q_digest tree_on_paper_ex1();
			friend q_digest tree_on_paper_ex2();
			*/
		/*};
		*/

		class merge_error: public std::exception {
		public:
			const char* what() const throw();
		};

		std::vector<std::pair<int, int>> small_buffer;
		std::vector<int> tree;
		int total_weight;
		int ceil_weight;
		int capacity;
		int universe;
		double error;

		void private_print(int cur_node, int indent = 0);
		void private_print_subtree_weights(int cur_node, int indent = 0);
		int subtree_weight_from(int cur_node);
		void set_subtree_weight_to_zero(int cur_node);
		bool in_range(const int& x, const int& l, const int& r);
		bool is_leaf(const int& x, const int& l, const int& r);
		void private_compress(int cur_node, int debt, int left_range, int right_range);
		void private_merge(const std::vector<int>& left, const std::vector<int>& right_tree);
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

		//DEBUG
		//q_digest(q_digest::node* root, double error, int universe);

		void print(int indent = 0);
		void print_subtree_weights();
		void update(int x, int weight) override;
		int query(int x) override;
		void compress();
		quantile_sketch<int>* merge(const quantile_sketch<int>& rhs) override;
		int get_total_weight();

	};
}
#endif