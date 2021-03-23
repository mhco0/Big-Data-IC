#ifndef QSBD_Q_DIGEST_H
#define QSBD_Q_DIGEST_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../quantile_sketch/quantile_sketch.hpp"

namespace qsbd{
	class q_digest : public quantile_sketch<int> {
	private:
		class node{
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
		};

		class merge_error: public std::exception {
		public:
			const char* what() const throw();
		};

		std::vector<std::pair<int, int>> small_buffer;
		int total_weight;
		int ceil_weight;
		int capacity;
		int universe;
		double error;
		q_digest::node* root;

		void check_adress(q_digest::node* t);
		bool in_range(int x, int l, int r);
		bool is_leaf(int x, int l, int r);
		void private_compress(q_digest::node * rt, int debt);
		void private_merge(q_digest::node*& new_root, q_digest::node * tree_to_merge1, q_digest::node* tree_to_merge2);
		void private_update(int x, int weight);
		int private_query(int x);
		void insert_in_buffer(int x, int weight);
		void transfer_buffer_to_tree();
		int query_from_buffer(int x);

	public:
		q_digest(double error, int universe);
		q_digest(const q_digest& other) = delete;
		~q_digest();

		q_digest& operator=(const q_digest& other) = delete;

		//DEBUG
		q_digest(q_digest::node* root, double error, int universe);

		void print(int indent = 0);
		void print_subtree_weights();
		void update(int x, int weight) override;
		int query(int x) override;
		void compress();
		quantile_sketch<int>* merge(const quantile_sketch<int>& rhs) override;
		int weight_total();

		//DEBUG
		/*friend q_digest tree_on_paper_ex1();
		friend q_digest tree_on_paper_ex2();
		friend void compress_tree_from_ex1();*/
	};
	/*
	q_digest tree_on_paper_ex1();
	q_digest tree_on_paper_ex2();
	void compress_tree_from_ex1();
	*/
}
#endif