/**
 * @file gk_factory.h
 * @brief QSBD gk_factory class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_GK_FACTORY_H
#define QSBD_GK_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../gk/gk.hpp"

namespace qsbd {

	/** @class gk_factory_factory
	 *  @brief A factory of gk_factory
	 *  @tparam Type The object type for the quantile_sketch that the factory will provide
	 * 
	 * A factory that create new gk\<Type\> instances that are able to merge with each other.
	*/
	template<class Type>
	class gk_factory : public sketch_factory<Type>{
	private:
		double error;
	public:

		/**
		 * @brief Constructor for the class
		 * @param err The error that will be used in all instances of gk\<Type\> from this factory
		*/
		gk_factory(double err){
			this->error = err;
		}

		/**
		 * @brief Creates a new instance of a gk and returns it.
		 * @return A pointer to a new instance of gk in the heap
		 * @warning
		 * 	It's up to a user the function of delete this memory. 
		 * @warning 
		 * 	It's return as a quantile_sketch\<int\> you should cast it to the correct type (gk\<Type\> *).
		*/
		quantile_sketch<Type> * instance() override {
			return new gk<Type>(this->error);
		}
	};
}

#endif