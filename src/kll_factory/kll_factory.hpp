/**
 * @file kll_factory.h
 * @brief QSBD kll_factory class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_KLL_FACTORY_H
#define QSBD_KLL_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../kll/kll.hpp"

namespace qsbd {

	/** @class kll_factory_factory
	 *  @brief A factory of kll_factory
	 *  @tparam Type The object type for the quantile_sketch that the factory will provide
	 * 
	 * A factory that create new kll\<Type\> instances that are able to merge with each other.
	*/
	template<class Type>
	class kll_factory : public sketch_factory<Type>{
	private:
		double error;
	public:

		/**
		 * @brief Constructor for the class
		 * @param err The error that will be used in all instances of kll\<Type\> from this factory
		*/
		kll_factory(double err){
			this->error = err;
		}

		/**
		 * @brief Creates a new instance of a kll and returns it.
		 * @return A pointer to a new instance of kll in the heap
		 * @warning
		 * 	It's up to a user the function of delete this memory. 
		 * @warning 
		 * 	It's return as a quantile_sketch\<int\> you should cast it to the correct type (kll\<Type\> *).
		*/
		quantile_sketch<Type> * instance() override {
			return new kll<Type>(this->error);
		}
	};
}

#endif  