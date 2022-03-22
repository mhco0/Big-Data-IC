/**
 * @file two_wise_family.h
 * @brief QSBD two_wise_family class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_TWO_WISE_FAMILY_H
#define QSBD_TWO_WISE_FAMILY_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../utils/utils.h"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd{

	
	/** @class two_wise_family
	 *  @brief A function class to represent functions from a 2-wise family.
	 * 	@see k_wise_family
	*/

	class two_wise_family{
	private:
		int big_prim; //!< A big prim to map the constant values
		int constants[2]; //!< Only 2 constants to work with
		int universe;
	public:
		/**
		 * @brief Constructs the two_wise_family class using the default values given by the compilator
		 * @warning 
		 * The instance won't be initialize properly by just calling in this way.
		*/
		two_wise_family() = default;

		/**
		 * @brief Constructs the two_wise_family class passing the @p universe of values 
		 * @param universe The universe of values that the function will map
		 */
		two_wise_family(int universe);

		/**
		 * @brief Constructs the two_wise_family class passing the @p universe and @p config as the constant values
		 * @param universe The universe of values that the function will map
		 * @param conf A vector of constant values to be used by the hash formula
		 * @warning
		 * None of the constant values should be 0, they need to be in the range [1, nextPrim(universe)),
		 *  otherwise the hash properties won't be satisfied.
		 * @warning
		 * The vector of constants sould have size equal 2
		*/
		two_wise_family(int universe, const std::vector<int>& conf);

		/**
		 * @brief Constructs the two_wise_family class by copy constructor. Copys all information given by @p rhs
		 * @param rhs A instance of another two_wise_family
		*/
		two_wise_family(const two_wise_family& rhs);

		/**
		 * @brief Destructs the two_wise_family instance
		*/
		~two_wise_family();
		
		/**
		 * @brief Calculates the value hashed of the @p elem
		 * @param elem A value in range [0, universe)
		 * @return The hash value of the @p elem
		 * @note
		 * By the way the hash maps the values ​​in the range of [0..nextPrim(universe)],
		 *  the return can be greater than the universe.
		*/
		int operator() (int elem);

		/**
		 * @brief Assigns a two_wise_family to another two_wise_family instance @p other
		 * @param other Another instace of a two_wise_family class
		 * @return A reference to this instance
		*/
		two_wise_family& operator=(const two_wise_family& other);

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
		 * @return The return always should be 2
		*/
		int get_k() const;
	};
}
#endif