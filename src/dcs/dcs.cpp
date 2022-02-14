#include "dcs.h"

namespace qsbd {
	void dcs::set_params(double err, int univ){
		this->universe = univ;
		this->error = err;
		this->total_weight = 0;
		this->w = (int) ceil(sqrt(log2(universe) * log(log(universe) / err)) / err);
		this->d = (int) ceil(log(log(universe) / err));
		this->s = std::max((int) floor(log2(universe / (double) (this->w * this->d))), 0);
		this->lvls = (int) ceil(log2(universe));
	}

	dcs::dcs(double err, int univ, const std::vector<std::vector<std::vector<int>>>& hashs_consts){
		this->set_params(err, univ);

		this->frequency_counters.reserve(this->lvls - (this->s + 1));

		for(int i = this->s + 1; i < this->lvls; i++){
			this->frequency_counters.emplace_back();
			int dyadic_interval = 1 << i;
			int interval_size = universe / dyadic_interval;

			this->frequency_counters[i - (this->s + 1)].reserve(interval_size);
			for(int j = 0; j < interval_size; j++){
				this->frequency_counters[i - (this->s + 1)].emplace_back(0);
			}
		}

		ASSERT((this->s + 1) == hashs_consts.size());

		this->estimators.reserve(this->s + 1);
		for(int i = 0; i <= this->s; i++){
			this->estimators.emplace_back(this->d, this->w, hashs_consts[i]);
		}
	}

	std::vector<count_sketch> dcs::get_estimators() const {
		return this->estimators;
	}


	dcs::dcs(double err, int univ){
		this->set_params(err, univ);

		this->frequency_counters.reserve(this->lvls - (this->s + 1));
		for(int i = this->s + 1; i < this->lvls; i++){
			this->frequency_counters.emplace_back();
			int dyadic_interval = 1 << i;
			int interval_size = universe / dyadic_interval;

			for(int j = 0; j < interval_size; j++){
				this->frequency_counters[i - (this->s + 1)].emplace_back(0);
			}
		}

		this->estimators.reserve(this->s + 1);
		for(int i = 0; i <= this->s; i++){
			this->estimators.emplace_back(this->d, this->w);
		}
	}

	dcs::~dcs(){
	}

	void dcs::update(int x, int weight){
		if(x >= this->universe){
			DEBUG_ERR("On dcs::update value first parameter 'x' is greater then the universe range");

			return;
		}

		this->total_weight += weight;
		for(int i = 0; i < this->lvls; i++){
			if(i > this->s){
				/*if(((i - (this->s + 1)) < 0) or x >= this->frequency_counters[i - (this->s + 1)].size()){
					VDEBUG(this->d);
					VDEBUG(this->w);
					VDEBUG(this->s);
					VDEBUG(this->lvls);
					VDEBUG(this->frequency_counters[i - (this->s + 1)].size());
					VDEBUG(i);
					VDEBUG(x);
				}*/
				this->frequency_counters[i - (this->s + 1)][x] += weight;
			}else{
				this->estimators[i].update(x, weight);
			}

			x = x / 2;
		}
	}

	int dcs::query(int x){
		if(x >= this->universe){
			DEBUG_ERR("On dcs::query value first parameter 'x' is greater then the universe range");

			return -1;
		}

		int rank = 0;

		for(int i = 0; i < this->lvls; i++){
			if(x & 1){
				if(i > this->s){
					rank += this->frequency_counters[i - (this->s + 1)][x - 1];
				}else{
					rank += this->estimators[i].query(x - 1);
				}
			}

			x = x / 2;
		}

		return rank;
	}

	int dcs::quantile(double quant){
		// quant must be in [0, 1]

		int weight = (int) total_weight * quant;
		int x = 0;
		int parse_quant = 0;

		for(int i = this->lvls - 1; i >= 0; i--){
			int M = 0;
			x = 2 * x;

			if(i > this->s){
				M = this->frequency_counters[i - (this->s + 1)][x];
			}else{
				M = this->estimators[i].query(x);
			}

			if(parse_quant + M < weight){
				x += 1;
				parse_quant += M;
			}
		}

		return x;
	}

	double dcs::cdf(int elem) override {
		if (not this->total_weight) return 0.0;

		return this->query(elem) / this->total_weight;
	}

	quantile_sketch<int> * dcs::merge(quantile_sketch<int>& rhs){
		dcs& rhs_cv = dynamic_cast<dcs&> (rhs);

		if(this->error - rhs_cv.error > 1e-6){
			DEBUG_ERR("dcs's error need to match");
			return nullptr;
		}

		if(this->universe != rhs_cv.universe){
			DEBUG_ERR("dcs's universe need to match");
			return nullptr;
		}

		dcs* merged = new dcs(this->error, this->universe, this->get_count_sketchs_hash_functions_constants());

		merged->total_weight = this->total_weight + rhs_cv.total_weight;

		for(int i = 0; i < merged->lvls; i++){
			if(i > merged->s){
				for(int j = 0; j < merged->frequency_counters[i - (merged->s + 1)].size(); j++){
					merged->frequency_counters[i - (merged->s + 1)][j] = this->frequency_counters[i - (this->s + 1)][j] + rhs_cv.frequency_counters[i - (rhs_cv.s + 1)][j];
				}
			}else{
				count_sketch * ref = this->estimators[i].merge(rhs_cv.estimators[i]);

				if(ref == nullptr){
					DEBUG_ERR("in dcs merge, count_sketch merge fail.");
					return nullptr;
				}

				merged->estimators[i] = *ref;
				delete ref;
			}
		}

		return merged;
	}

	void dcs::inner_merge(quantile_sketch<int>& rhs){
		dcs& rhs_cv = dynamic_cast<dcs&> (rhs);

		if(this->error - rhs_cv.error > 1e-6){
			DEBUG_ERR("dcs's error need to match");
			return;
		}

		if(this->universe != rhs_cv.universe){
			DEBUG_ERR("dcs's universe need to match");
			return;
		}

		this->total_weight += rhs_cv.total_weight;

		for(int i = 0; i < this->lvls; i++){
			if(i > this->s){
				for(int j = 0; j < this->frequency_counters[i - (this->s + 1)].size(); j++){
					this->frequency_counters[i - (this->s + 1)][j] += rhs_cv.frequency_counters[i - (rhs_cv.s + 1)][j];
				}
			}else{
				this->estimators[i].inner_merge(rhs_cv.estimators[i]);
			}
		}
	}

	int dcs::get_tree_lvl() const {
		return this->lvls;
	}

	int dcs::get_w() const {
		return this->w;
	}

	int dcs::get_d() const {
		return this->d;
	}

	int dcs::get_s() const {
		return this->s;
	}

	double dcs::get_error() const {
		return this->error;
	}

	int dcs::get_universe() const {
		return this->universe;
	}

	int dcs::get_total_weight() const {
		return this->total_weight;
	}

	std::vector<std::vector<std::vector<int>>> dcs::get_count_sketchs_hash_functions_constants() const {
		std::vector<std::vector<std::vector<int>>> consts;
	
		consts.reserve(this->estimators.size());
		for(auto& it : this->estimators){
			consts.emplace_back(it.get_hash_functions_consts());
		}

		return consts;
	}

	uint64_t dcs::get_heap_size(){
		uint64_t dcs_hs = sizeof(dcs);
		uint64_t fc_hs = 0;

		for(int i = 0; i < frequency_counters.size(); i++){
			fc_hs += sizeof(std::vector<int>) + sizeof(int) * frequency_counters[i].capacity();
		}

		uint64_t cs_hs = 0;

		for(int i = 0; i < estimators.size(); i++){
			cs_hs += estimators[i].get_heap_size();
		}

		return dcs_hs + fc_hs + cs_hs;
	}
}
