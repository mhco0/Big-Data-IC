/**
 * @file k_wise_family.h
 * @brief QSBD k_wise_family class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_K_WISE_FAMILY_H
#define QSBD_K_WISE_FAMILY_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../utils/utils.h"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd{

	/** @class k_wise_family
	 *  @brief A function class to represent functions from a k-wise family.
	 * 	
	 * From Wikipédia :
	 * In computer science, a family of hash functions is said to be k-independent, k-wise independent or k-universal if 
	 * selecting a function at random from the family guarantees that the hash codes of any designated k keys are independent random variables (see precise mathematical definitions below).
	 * Such families allow good average case performance in randomized algorithms or data structures, even if the input data is chosen by an adversary.
	 *  The trade-offs between the degree of independence and the efficiency of evaluating the hash function are well studied,
	 *  and many k-independent families have been proposed.
	 * 
	*/

	class k_wise_family{
	private:
		int big_prim;   //!< A big prim to map the constant values
		std::vector<int> constants;
		int universe;
		int _k;
	public:
		/**
		 * @brief Constructs the k_wise_family class using the default values given by the compilator
		 * @warning 
		 * The instance won't be initialize properly by just calling in this way.
		*/
		k_wise_family() = default;
		
		/**
		 * @brief Constructs the k_wise_family class passing @p k and @p universe of values 
		 * @param k The k type of the family. Ex: k = 2 -> 2-wise family
		 * @param universe The universe of values that the function will map
		 */
		k_wise_family(int k, int universe);

		/**
		 * @brief Constructs the k_wise_family class passing @p k , @p universe and @p config as the constant values
		 * @param k The k type of the family. Ex: k = 2 -> 2-wise family
		 * @param universe The universe of values that the function will map
		 * @param conf A vector of constant values to be used by the hash formula
		 * @warning
		 * The constant values need to be in the range [0, nextPrim(universe)),
		 *  otherwise the hash properties won't be satisfied.
		 * @warning
		 * The vector of constants sould have the size as the given @p k .
		*/
		k_wise_family(int k, int universe, const std::vector<int>& conf);
		
		/**
		 * @brief Constructs the k_wise_family class by copy constructor. Copys all information given by @p rhs
		 * @param rhs A instance of another k_wise_family
		*/
		k_wise_family(const k_wise_family& rhs);

		/**
		 * @brief Destructs the k_wise_family instance
		*/
		~k_wise_family();
		
		/**
		 * @brief Calculates the value hashed of the @p elem
		 * @param elem A value in range [0, universe)
		 * @return The hash value of the @p elem
		 * @note
		 * By the way the hash maps the values ​​in the range of [0..nextPrim(universe) - 1],
		 *  the return can be greater than the universe.
		*/
		int operator() (int elem);

		/**
		 * @brief Assigns a k_wise_family to another k_wise_family instance @p other
		 * @param other Another instace of a k_wise_family class
		 * @return A reference to this instance
		*/
		k_wise_family& operator=(const k_wise_family& other);

		/**
		 * @brief Gets the constant values choosen by the user or by the algorithm.
		 * @return A vector of constant values
		*/
		std::vector<int> get_constants() const;
		
		/**
		 * @brief Gets the universe of values choosen by the user on initialization.
		 * @return The universe of values to map
		*/
		int get_universe() const;
		
		/**
		 * @brief Gets the K type of the family. Ex: if we have a 2-wise family, this should return 2
		 * @return The k type of the family
		*/
		int get_k() const;
	};
}
#endif