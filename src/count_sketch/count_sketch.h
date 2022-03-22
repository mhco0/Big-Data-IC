/**
 * @file count_sketch.h
 * @brief QSBD count_sketch class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_COUNT_SKETCH_H
#define QSBD_COUNT_SKETCH_H
#include "../qsbd_debug/qsbd_debug.h"
#include "../global_generator/global_generator.h"
#include "../commum_header/commum_header.h"
//#include "../k_wise_family/k_wise_family.h"
#include "../two_wise_family/two_wise_family.h"


namespace qsbd {

	/** @class count_sketch
	 *  @brief A class to estimate a frequency of a element in a stream.
	 * 	
	 *  It also summarizes a multiset v and answers point queries 
	 *  approximately. It has asymptotically the same guarantees as the CountMin Sketch,
	 *  but with slightly worse hidden constant factors: Given parameters \f$\epsilon\f$ and \f$\delta\f$,
	 *  the summary uses space \f$O(\frac{\log(\frac{1}{\delta})}{\epsilon})\f$ , and guarantees with probability at least 1−δ
	 *  that any point query is answered with additive error at most \f$\epsilon||v||_1\f$. Alternatively,
	 *  with space \f$O(\frac{\log(\frac{1}{\delta})}{\epsilon^2})\f$, it has error at most \f$\epsilon||v||_2\f$ with probability at least \f$1 − \delta\f$.
	 *  The benefit of the Count Sketch is that it produces an unbiased estimator.
	 * 
	 *  This Count Sketch uses the L1 bound garantee.
	 * 
	*/

	class count_sketch {
	private:  

		double error, delta;
		// d is the number of rows and t is the number of columns for the estimator matrix
		int d, t; //<! The row and the collumns size.
	   	std::vector<two_wise_family> hash_functions; //<! The two_wise_family functions for mapping the values.
		std::vector<std::vector<int>> estimators; //<! The matrix of estimators.
		//L1 bound
		void set_param_double(double err, double delt);
		void set_param_int(int fixd, int fixt);
		// L2 bound
		void set_param_double2(double err, double delt);
		void set_param_int2(int fixd, int fixt);

		int g(int random_bit);
	public:
		/**
		 * @brief Constructs a count_sketch with @p fixd rows and @p fixt collumns
		 * @param fixd The number of rows for the estimation matrix
		 * @param fixt The number of collumns for the estimation matrix
		 * @warning 
		 * Constructor bounds are not guaranteed unless you know the exact calculations,
		 *  if that is not the case use @see count_sketch(double, double) instead.
		 * @sa 
		 * count_sketch(double, double)
		*/
		count_sketch(int fixd, int fixt);

		/**
		 * @brief Constructs a count_sketch with @p fixd rows, @p fixt collumns and @p hashs_consts 
		 * @param fixd The number of rows for the estimation matrix
		 * @param fixt The number of collumns for the estimation matrix
		 * @param hashs_consts A vector of vectors of constants used by each hash function in the count_sketch
		 * @warning 
		 * Constructor bounds are not guaranteed unless you know the exact calculations,
		 *  if that is not the case use @see count_sketch(double, double) instead.
		 * @warning 
		 * The vector should have the same size as @p fixd , it also need that each vector inside it 
		 * contains with two values from a interval [0..nextPrim(2 * @p fixt ) - 1]. 
		 * @sa 
		 * count_sketch(double, double)
		*/
		count_sketch(int fixd, int fixt, const std::vector<std::vector<int>>& hashs_consts);

		/**
		 * @brief Constructs a count_sketch with error @p err and delta @p delt 
		 * @param err The \f$\epsilon\f$ from the algorithm
		 * @param delt The \f$\delta\f$ from the algorithm
		*/
		count_sketch(double err, double delt);

		/**
		 * @brief Constructs a count_sketch with error @p err and delta @p delt 
		 * @param err The \f$\epsilon\f$ from the algorithm
		 * @param delt The \f$\delta\f$ from the algorithm
		 * @param hashs_consts The constants to be used by each hash function
		 * @warning 
		 * The vector should have the same size as the rows of estimators from the sketch, it also need that each vector inside it 
		 * contains with two values from a interval [0..nextPrim(2 * collumns in the sketch) - 1]. 
		 *  
		*/
		count_sketch(double err, double delt, const std::vector<std::vector<int>>& hashs_consts);

		/**
		 * @brief Constructs a count_sketch with @p other count_sketch instance
		 * @param other Another count_sketch instance
		*/
		count_sketch(const count_sketch& other);

		/**
		 * @brief Destructs the count_sketch instance
		*/
		~count_sketch();

		/**
		 * @brief Updates the count_sketch with @p elem element and @p weight
		 * @param elem The element to update the sketch
		 * @param weight The weight associeted with @p elem
		*/
		void update(int elem, int weight);

		/**
		 * @brief Query the count_sketch about the frequency of @p elem 
		 * @param elem The element to be queried
		 * @return The estimated frequency for that @p elem
		*/
		int query(int elem);

		/**
		 * @brief Merges this sketch with @p rhs sketch and saves the result in a new instance
		 * @param rhs Another instance of a count_sketch
		 * @return A new allocated instance of a count_sketch
		 *  @warning
		 * The @p rhs count_sketch should have the same estimator matrix size (d and t), in other words
		 * should have the same \f$\epsilon\f$ and \f$\delta\f$.
		 * @warning
		 * It's up to a user the function of delete this memory.
		 * @sa 
		 * void count_sketch::inner_merge(count_sketch&)
		*/
		count_sketch* merge(count_sketch& rhs);

		/**
		 * @brief Merges this sketch with @p rhs sketch and saves the result in this sketch
		 * @param rhs Another instance of a count_sketch
		 * @warning 
		 * All hash functions from @p rhs should have the same constants as this sketch.
		 * @warning
		 * The @p rhs count_sketch should have the same estimator matrix size (d and t), in other words
		 * should have the same \f$\epsilon\f$ and \f$\delta\f$.
		 * @sa 
		 * count_sketch* count_sketch::merge(count_sketch&)
		*/
		void inner_merge(count_sketch& rhs);

		/**
		 * @brief Assign this count_sketch with @p other
		 * @param other Another instance of a count_sketch
		 * @return A reference to this
		*/
		count_sketch& operator=(const count_sketch& other);

		/**
		 * @brief A getter for the row of the estimation matrix
		 * @return The number of rows on the estimation matrix
		*/
		int get_d() const;

		/**
		 * @brief A getter for the collumns on the estimation matrix
		 * @return The number of collumns on the estimation matrix
		*/
		int get_t() const;

		/**
		 * @brief A getter for the \f$\epsilon\f$
		 * @return \f$\epsilon\f$
		*/
		double get_error() const;

		/**
		 * @brief A getter for the \f$\delta\f$
		 * @return \f$\delta\f$
		*/
		double get_delta() const;

		/**
		 * @brief A getter for the estimation matrix
		 * @return The estimation matrix
		*/
		std::vector<std::vector<int>> get_estimators() const;

		/**
		 * @brief A getter for the hash functions
		 * @return All hash functions used by this count_sketch
		 * @sa 
		 * qsbd::two_wise_family
		*/
		std::vector<two_wise_family> get_hash_functions() const;

		/**
		 * @brief A getter for all the hash functions constants
		 * @return A vector with all hash functions constants each row on vector has size equal two.
		 * @sa 
		 * qsbd::two_wise_family
		*/
		std::vector<std::vector<int>> get_hash_functions_consts() const;

		/**
		 * @brief Getter for the estimated amount of heap memory allocated for this object for a pointer.
		 * @return The estimated memory in bytes.
		*/
		uint64_t get_heap_size();
	};
}
#endif
