/**
 * @file q_digest.h
 * @brief QSBD q_digest class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_Q_DIGEST_FACTORY_H
#define QSBD_Q_DIGEST_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../q_digest/q_digest.h"

namespace qsbd{

	/** @class q_digest_factory
	 *  @brief A factory of q_digest
	 * 
	 * A factory that create new q_digest instances that are able to merge with each other.
	*/
	class q_digest_factory : public sketch_factory<int>{
	private:
		double error;
		int universe;
	public:

		/**
		 * @brief Constructor for the class
		 * @param err The error that will be used in all instances of q_digest from this factory
		 * @param uni The universe that will be used in all instances of q_digest from this factory
		*/
		q_digest_factory(double err, int uni){
			this->error = err;
			this->universe = uni;
		}

		
		/**
		 * @brief Creates a new instance of a q_digest and returns it.
		 * @return A pointer to a new instance of q_digest in the heap
		 * @warning
		 * 	It's up to a user the function of delete this memory. 
		 * @warning 
		 * 	It's return as a quantile_sketch\<int\> you should cast it to the correct type (q_digest *).
		*/
		quantile_sketch<int> * instance() override {
			return new q_digest(this->error, this->universe);
		}
	};
}

#endif