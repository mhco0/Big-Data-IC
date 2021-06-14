/**
 * @file q_digest.h
 * @brief QSBD q_digest class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_Q_DIGEST_H
#define QSBD_Q_DIGEST_H
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../qsbd_debug/qsbd_debug.h"
#include "../utils/utils.h"
#include <cmath>
#include <iomanip>

namespace qsbd{
	
	/** @class q_digest
	 *  @brief A quantile sketch derived class.
	 * 	
	 * The Q-Digest summary is based on maintaining a subset 
	 * of simple counters of ranges, when the input is drawn from a fixed integer domain,
	 * and can be weighted. Each input is an \f$(x,w)\f$ pair where w must be positive. 
	 * The summary takes space at most \f$O(\frac{\log{U}}{\epsilon})\f$,
	 * which is fixed and independent of the input. Summaries can be easily merged together. The 
	 * summary works by maintaining a tree-structure over the domain U: 
	 * nodes in the tree correspond to subranges of U, and simple statistics
	 * are maintained for a small set of nodes.
	 * 
	*/

	class q_digest : public quantile_sketch<int> {
	private:
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
		/**
		 * @brief Constructs the q_digest class using @p error and @p universe
		 * @param error The error that the q_digest will suport
		 * @param universe The range for the elements on q_digest [0..universe - 1]
		*/
		q_digest(double error, int universe);
		q_digest(const q_digest& other) = delete;
		
		/**
		 * @brief Destructs the q_digest instance
		*/
		~q_digest();
		q_digest& operator=(const q_digest& other) = delete;

		/**
		 * @brief Prints the Tree structure of the class on the standard output (std::cout)
		 * @param indent The indentation on print
		*/
		void print(int indent = 0) const;

		/**
		 * @brief Prints the Tree structure of the class on the standard output (std::cout), but the weights
		 * on the tree in each node will be the sum of the weight plus the weight of their subtree
		*/
		void print_subtree_weights();

		/**
		 * @brief Inserts a element @p x with weigth @p weight
		 * @param x The element to be inserted
		 * @param weight The weight of this element
		 * @note The insertion will be in \f$O(log(U))\f$
		*/
		void update(int x, int weight) override;
		
		/**
		 * @brief Query the rank of the element @p x
		 * @param x The element to be queried
		 * @return The estimated rank for @p x
		 * @note All ranks will be int values, and \f$query(x) \leq rank(x) \leq query(x) + \epsilon * W\f$ 
		*/
		int query(int x) override;
		
		/**
		 * @brief Trys to compress the q_digeste tree if possible
		*/
		void compress();
		
		/**
		 * @brief Merges this instance of quadtree with another q_digest instance
		 * @param rhs A q_digest instance to be merged
		 * @return A pointer to the new instance of a q_digest 
		 * @sa q_digest::inner_merge(quantile_sketch<int>&)
		 * @warning 
		 * The universe and the error in both instance needs to be the same. The error will be considered as the same
		 * if \f$|this->error - rhs.error| \leq 1e-6\f$
		 * 
		 * @warning 
		 * The quantile_sketch must be a instance of the q_digest class
		 * 
		 * @warning 
		 * Is the user responsabilite of frees the returned pointer
		 * 
		*/
		quantile_sketch<int>* merge(quantile_sketch<int>& rhs) override;
		
		/**
		 * @brief Merges this instance of quadtree with another q_digest instance and saves it in this instance
		 * @param rhs A q_digest instance to be merged
		 * @sa q_digest::merge(quantile_sketch<int>&)
		 * @warning 
		 * The universe and the error in both instance needs to be the same. The error will be considered as the same
		 * if \f$|this->error - rhs.error| \leq 1e-6\f$
		 * 
		 * @warning 
		 * The quantile_sketch must be a instance of the q_digest class
		 * 
		*/
		void inner_merge(quantile_sketch<int>& rhs) override;
		
		/**
		 * @brief Getter for the total weight saw the the instance
		 * @return The total weight 
		*/
		int get_total_weight() const;
	};
}
#endif