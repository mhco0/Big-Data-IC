/**
 * @file dcs_factory.h
 * @brief QSBD dcs_factory class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_DCS_FACTORY_H
#define QSBD_DCS_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../dcs/dcs.h"

namespace qsbd{

	/** @class dcs_factory
	 *  @brief A factory of dcs
	 * 
	 * A factory that create new dcs instances that are able to merge with each other.
	*/
	class dcs_factory : public sketch_factory<int>{
	private:
		double error;
		int universe;
		std::vector<std::vector<std::vector<int>>> hashs_consts;
	public:

		/**
		 * @brief Constructor for the class
		 * @param err The error that will be used in all instances of dcs from this factory
		 * @param uni The universe that will be used in all instances of dcs from this factory
		*/
		dcs_factory(double err, int univ){
			this->error = err;
			this->universe = univ;
		
			dcs dummy(err, univ);

			hashs_consts = std::move(dummy.get_count_sketchs_hash_functions_constants());
		}

		/**
		 * @brief Creates a new instance of a dcs and returns it.
		 * @return A pointer to a new instance of dcs in the heap
		 * @warning
		 * 	It's up to a user the function of delete this memory. 
		 * @warning 
		 * 	It's return as a quantile_sketch\<int\> you should cast it to the correct type (dcs *).
		*/
		quantile_sketch<int> * instance() override {
			return new dcs(this->error, this->universe, this->hashs_consts);
		}

	};
}

#endif
