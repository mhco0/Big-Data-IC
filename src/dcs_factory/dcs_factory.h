#ifndef QSBD_DCS_FACTORY_H
#define QSBD_DCS_FACTORY_H
#include "../sketch_factory/sketch_factory.hpp"
#include "../dcs/dcs.h"

namespace qsbd{
	class dcs_factory : public sketch_factory<int>{
	private:
		double error;
		int universe;
		int count;
		std::vector<std::vector<std::vector<int>>> hashs_consts;
	public:
		dcs_factory(double err, int univ){
			this->error = err;
			this->universe = univ;
			this->count = 0;
		
			dcs dummy(err, univ);

			hashs_consts = std::move(dummy.get_count_sketchs_hash_functions_constants());
		}

		quantile_sketch<int> * instance() override {
			count++;
			return new dcs(this->error, this->universe, this->hashs_consts);
		}

		int allocations(){
			return this->count;
		}
	};
}

#endif
