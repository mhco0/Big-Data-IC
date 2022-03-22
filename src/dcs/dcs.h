/**
 * @file dcs.h
 * @brief QSBD dcs class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_DCS_H
#define QSBD_DCS_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../count_sketch/count_sketch.h"
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd{

	class dcs_factory;

	/** @class dcs
	 *  @brief A class to represent the Dyadic Count Sketch summary
	 * 	
	 * The DCS summary provides a compact summary of data drawn from an integer domain [U] = [0, 1, . . . , U − 1].
	 *  It takes (x,w) pairs as input where w can be an integer that is either positive or negative.
	 *  But the multiplicity of x for any x in the data set must remain nonnegative when the summary is queried
	 *  for it to be meaningful. It is a randomized algorithm. This summary takes space
	 *  \f$O(\frac{1}{\epsilon}*\log^{1.5}{U}*\log^{1.5}{\frac{\log{U}}{\epsilon}})\f$, and 
	 * returns all ranks within εW error with constant probability,
	 *  where W is the total weight of input integers. The summary works upon a 
	 * dyadic structure over the universe U and maintains a Count Sketch
	 *  structure for frequency estimation in each level of this
	 * 
	*/
	class dcs : public quantile_sketch<int>{
	private: 
		friend class dcs_factory;

		int universe;
		double error;
		// for initialize count sketch
		int total_weight;
		int w, d; //!< The rows and collums used in each Count Sketch
		int s; //!< The levels that we use sketchs
		int lvls;

		std::vector<std::vector<int>> frequency_counters;
		std::vector<count_sketch> estimators;

		/**
		 * @brief A setter for the propertys of the dcs instance 
		 * @param err The error used by the algorithm
		 * @param univ The universe of values choosed
		*/
		void set_params(double err, int univ);
	public:

		/**
		 * @brief The constructor for the dcs 
		 * @param err The error used by the algorithm
		 * @param univ The universe of values choosed
		*/
		dcs(double err, int univ);

		/**
		 * @brief The constructor for dcs that can regule the constants used in the hash functions 
		 * @param err The error used by the algorithm
		 * @param univ The universe of values choosed
		 * @param hashs_consts A vector of constants to be used in each hash function in each Count Sketch
		 * 
		 * @note
		 * This constructor is helpful to construct dcs's that can merge together 
		 * 
		 * @warning 
		 * The vector for @p hashs_consts should have the size as the levels that the algorithm uses the Count Sketch
		*/
		dcs(double err, int univ, const std::vector<std::vector<std::vector<int>>>& hashs_consts);

		/**
		 * @brief A destructor for the dcs 
		*/
		~dcs();

		/**
		 * @brief Inserts a element @p x with weight @p weight
		 * @param x The element to be inserted
		 * @param weight The weight of this element
		*/
		void update(int x, int weight) override;

		/**
		 * @brief Query the rank of the element @p x
		 * @param x The element to be queried
		 * @return The estimated rank for @p x
		*/
		int query(int x) override;

		/**
		 * @brief Query the element for a given quantile @p quant 
		 * @param quant The quantile to be queried
		 * @return The estimated value that represents the quantile @p quant
		 * 
		 * @warning 
		 * @p quant Should be in range [0.0 .. 1.0].
		*/
		int quantile(double quant) override;

		/**
		 * @brief CDF method. Receives a @p elem and returns a probability for some value be less than this value. 
		 * @param elem The element to be queried
		 * @return The probability for this element
		*/
		double cdf(int elem) override;
		
		/**
		 * @brief Merges this instance of dcs with another dcs instance
		 * @param rhs A dcs instance to be merged
		 * @return A pointer to the new instance of a dcs 
		 * @sa dcs::inner_merge(quantile_sketch\<int\>&)
		 * @warning 
		 * The error in both instance needs to be the same. The error will be considered as the same
		 * if \f$|this->error - rhs.error| \leq 1e-6\f$
		 * 
		 * @warning 
		 * The quantile_sketch must be a instance of the dcs class
		 * 
		 * @warning 
		 * Is the user responsabilite of frees the returned pointer
		 * 
		*/
		quantile_sketch<int> * merge(quantile_sketch<int>& rhs) override;

		/**
		 * @brief Merges this instance of dcs with another dcs instance and saves it in this instance
		 * @param rhs A dcs instance to be merged
		 * @sa dcs::merge(quantile_sketch\<int\>&)
		 * @warning 
		 * The error in both instances needs to be the same. The error will be considered as the same
		 * if \f$|this->error - rhs.error| \leq 1e-6\f$
		 * 
		 * @warning 
		 * The quantile_sketch must be a instance of the dcs class
		 * 
		*/
		void inner_merge(quantile_sketch<int>& rhs) override;

		/**
		 * @brief A getter for all estimators in the summary
		 * @return A vector of Count Sketchs that are beening used
		*/
		std::vector<count_sketch> get_estimators() const;
		
		/**
		 * @brief A getter for how many levels we use in the summary
		 * @return The levels usedsin the dcs
		*/
		int get_tree_lvl() const;
		
		/**
		 * @brief A getter for the number of collumns used in each Count Sketch 
		 * @return The number of collumns  in each Count Sketch
		*/
		int get_w() const;
		
		/**
		 * @brief A getter for the number of rows used in each Count Sketch
		 * @return The number of rows in each Count Sketch
		*/
		int get_d() const;
		
		/**
		 * @brief A getter for the level that we stop using Count Sketchs in summary
		 * @return The level that we stop using Count Sketchs 
		*/
		int get_s() const;

		/**
		 * @brief A getter for the error in the algorithm
		 * @return The error
		*/
		double get_error() const;

		/**
		 * @brief A getter for the universe use in the algorithm
		 * @return The universe
		*/
		int get_universe() const;

		/**
		 * @brief A getter for the total weight sean in the summary
		 * @return The total weight
		*/
		int get_total_weight() const;

		/**
		 * @brief A getter for all hash functions
		*/
		std::vector<std::vector<std::vector<int>>> get_count_sketchs_hash_functions_constants() const;

		/**
		 * @brief Getter for the estimated amount of heap memory allocated for this object for a pointer.
		 * @return The estimated memory in bytes.
		*/
		uint64_t get_heap_size() override;
	};
}
#endif