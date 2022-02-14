/**
 * @file kll.hpp
 * @brief QSBD kll class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_KLL_H
#define QSBD_KLL_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd {

	/** @class kll\<T\>
	 * @brief A class to represent the Karnin-Lang-Liberty sketch
	 *  
	 * The KLL summary provides a summary of data drawn from an ordered domain U.
	 *  It is a comparison-based summary. The size of the summary is \f$O(\frac{1}{\epsilon}*\sqrt{\log{\frac{1}{\epsilon}}})\f$
	 * after any number of UPDATE and MERGE operations. It is more space- and time-efficient than 
	 * the GK summary, but it is a randomized algorithm that may exceed the 
	 * \f$\epsilon\f$ error bound with a small probability. The version of KLL described 
	 * below assumes unweighted items.
	 * 
	 * @tparam T The object type for the kll
	*/
	template<class T>
	class kll : public quantile_sketch<T>{
	protected:
		
		std::uniform_int_distribution<int> coin; //!< A coin to flip and choose the elements to change their resolution
		std::vector<std::vector<T>> buffers_array;
		int height;
		int N;
		double buffer_diff;
		double capacity_max;
		double error;
		double delta;
		
	public:
		/**
		 * @brief Constructor for the kll algorithm
		 * @param err The error for the bound in the algorithm
		 * @param delt The delta probability for error
		 * @param multiplier The decay factor for the buffer capacity on the algorithm (the 'c' constant)
		*/
		kll(double err, double delt = 0.3, double multiplier = 0.7);

		/**
		 * @brief Inserts the element @p elem in a sorted vector @p buffer
		 * @param buffer A vector with sorted elements
		 * @param elem A new element to be inserted
		*/
		static void insert_sorted(std::vector<T> &buffer, const T &elem);

		/**
		 * @brief A method that tries to compress the buffer array in the algorithm
		*/
		void compress();

		/**
		 * @brief Updates the kll summary with the element @p elem
		 * @param elem The new element to be inserted in the summary
		*/
		void update(T elem) override;

		/**
		 * @brief A method to query the rank of a certain element @p elem
		 * @param elem The element to be queried
		 * @return The estimated rank for the element @p elem in the summary
		*/
		int query(T elem) override;

		/**
		 * @brief Quantile method. Receives a @p quant and returns a element associate with this quantile.
		 * @param quant The quantile to be queried
		 * @return The element associete with the quantile @p quant
		 * @warning 
		 * The quantile should be in range [0..1]
		*/
		T quantile(double quant) override;

		/**
		 * @brief CDF method. Receives a @p elem and returns a probability for some value be less than this value. 
		 * @param elem The element to be queried
		 * @return The probability for this element
		*/
		double cdf(T elem) override;

		/**
		 * @brief Merges this instance of kll with another kll instance
		 * @param rhs A kll\<T\> instance to be merged
		 * @return A pointer to the new instance of a kll 
		 * @sa kll\<T\>::inner_merge(quantile_sketch\<T\>&)
		 * @warning 
		 * The error in both instance needs to be the same. The error will be considered as the same
		 * if \f$|this->error - rhs.error| \leq 1e-6\f$
		 * 
		 * @warning 
		 * The quantile_sketch must be a instance of the kll class
		 * 
		 * @warning 
		 * Is the user responsabilite of frees the returned pointer
		 * 
		*/
		quantile_sketch<T>* merge(quantile_sketch<T>& rhs) override;

		/**
		 * @brief Merges this instance of kll with another kll instance and saves it in this instance
		 * @param rhs A kll\<T\> instance to be merged
		 * @sa kll\<T\>\::merge(quantile_sketch\<T\>&)
		 * @warning 
		 * The error in both instances needs to be the same. The error will be considered as the same
		 * if \f$|this->error - rhs.error| \leq 1e-6\f$
		 * 
		 * @warning 
		 * The quantile_sketch must be a instance of the kll class
		 * 
		*/
		void inner_merge(quantile_sketch<T>& rhs) override;

		/**
		 * @brief A debug method to print the array of buffers in the summary on the standard ouput
		*/
		void print();

		/**
		 * @brief A setter for the buffer of elements in the sketch
		 * 
		 * @note 
		 * This can become depreciated in the newer versions of this algoritm
		*/
		void set_buffers(std::vector<std::vector<T>>& buffers);
		
		/**
		 * @brief A getter for the array of buffers
		*/
		std::vector<std::vector<T>> get_buffers();
		
		/**
		 * @brief A getter for the error in the algorithm
		*/
		double get_error();

		/**
		 * @brief A getter for the error in the algorithm
		*/
		double get_delta();
		
		/**
		 * @brief A getter for the capacity on each level of the buffers
		*/
		double get_capacity();

		/**
		 * @brief A getter for the number of elements saw by this sketch
		*/
		int get_N();

		/**
		 * @brief Getter for the estimated amount of heap memory allocated for this object for a pointer.
		 * @return The estimated memory in bytes.
		*/
		uint64_t get_heap_size() override;
	};
}
#endif