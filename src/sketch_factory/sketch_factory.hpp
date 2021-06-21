/**
 * @file sketch_factory.hpp
 * @brief QSBD sketch_factory class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_SKETCH_FACTORY
#define QSBD_SKETCH_FACTORY
#include "../quantile_sketch/quantile_sketch.hpp"

namespace qsbd{
	template<class Type>

	/** @class sketch_factory
	 *  @brief A interface for factorys of sketchs
	 * 
	 * A interface for a factory that creates new quantile sketchs
	 * @note
	 * The idea of ​​this class is to always make quantile_sketchs that are able to merge together
	*/
	class sketch_factory{
	private:
	public:

		/**
		 * @brief A pure virtual method for create new instances of a quantile_sketch\<Type\>.
		 * @return A pointer to a new allocated quantile_sketch\<Type\>
		 * @warning 
		 * It's up to a user the function of delete this memory. 
		 * @warning
		 * All quantile sketchs returned by this method should be capable of merging together.
		*/
		virtual quantile_sketch<Type> * instance() = 0;    
	};  
}
#endif