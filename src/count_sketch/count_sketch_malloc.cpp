#include "count_sketch.h"

namespace qsbd {
	void count_sketch::set_param_double(double err, double delt){
		this->error = err;
		this->delta = delt;

		this->d = (int) ceil(48 * log(1.0 / delta));
		this->t = (int) ceil(3.0 / error);
	}

	void count_sketch::set_param_int(int fixd, int fixt){
		this->d = fixd;
		this->t = fixt;

		this->error = 3.0 / this->t;
		this->delta = exp(-this->d / 48.0);
	}

	void count_sketch::set_param_double2(double err, double delt){
		this->error = err;
		this->delta = delt;

		this->d = (int) ceil(16 * log(1.0 / delta));
		this->t = (int) ceil(4.0 / (error * error));
	}

	void count_sketch::set_param_int2(int fixd, int fixt){
		this->d = fixd;
		this->t = fixt;

		this->error = 2.0 / sqrt(this->t);
		this->delta = exp(-this->d / 16.0);
	}

	int count_sketch::g(int random_bit){
		if(random_bit & 1) return 1;
		else return -1;
	}

	count_sketch::count_sketch(int fixd, int fixt){
		this->set_param_int(fixd, fixt);

		this->estimators = (int **) malloc(this->d * sizeof(int *));

		for(int i = 0; i < this->d; i++){
			this->estimators[i] = (int *) calloc(this->t, sizeof(int));
		}
	
		this->hash_functions = (k_wise_family **) malloc(this->d * sizeof(k_wise_family *));
		for(int i = 0; i < this->d; i++){
			this->hash_functions[i] = new k_wise_family(2, 2 * this->t);
		}
	}

	count_sketch::count_sketch(int fixd, int fixt, const std::vector<std::vector<int>>& hashs_consts){
		this->set_param_int(fixd, fixt);

		this->estimators = (int **) malloc(this->d * sizeof(int *));

		for(int i = 0; i < this->d; i++){
			this->estimators[i] = (int *) calloc(this->t, sizeof(int));
		}

		ASSERT(this->d == hashs_consts.size());
	
		this->hash_functions = (k_wise_family **) malloc(this->d * sizeof(k_wise_family *));
		for(int i = 0; i < this->d; i++){
			this->hash_functions[i] = new k_wise_family(2, 2 * this->t, hashs_consts[i]);
		}
	}

	count_sketch::count_sketch(double err, double delt){
		this->set_param_double(err, delt);

		this->estimators = (int **) malloc(this->d * sizeof(int *));

		for(int i = 0; i < this->d; i++){
			this->estimators[i] = (int *) calloc(this->t, sizeof(int));
		}
		
		this->hash_functions = (k_wise_family **) malloc(this->d * sizeof(k_wise_family *));
		for(int i = 0; i < this->d; i++){
			this->hash_functions[i] = new k_wise_family(2, 2 * this->t);
		}
	}

	count_sketch::count_sketch(double err, double delt, const std::vector<std::vector<int>>& hashs_consts){
		this->set_param_double(err, delt);
	
		this->estimators = (int **) malloc(this->d * sizeof(int *));

		for(int i = 0; i < this->d; i++){
			this->estimators[i] = (int *) calloc(this->t, sizeof(int));
		}

		// the hash should map to 2 * t
		ASSERT(this->d == hashs_consts.size());

		this->hash_functions = (k_wise_family **) malloc(this->d * sizeof(k_wise_family *));
		for(int i = 0; i < this->d; i++){
			this->hash_functions[i] = new k_wise_family(2, 2 * this->t, hashs_consts[i]);
		}
	}

	count_sketch::count_sketch(const count_sketch& other){
		this->error = other.error;
		this->delta = other.delta;
		this->d = other.d;
		this->t = other.t;

		this->estimators = (int **) malloc(other.d * sizeof(int *));
		for(int i = 0; i < other.d; i++){
			this->estimators[i] = (int *) malloc(other.t * sizeof(int));
			for(int j = 0; j < other.t; j++){
				this->estimators[i][j] = other.estimators[i][j];
			}
		}
	
		this->hash_functions = (k_wise_family **) malloc(other.d * sizeof(k_wise_family *));
		for(int i = 0; i < other.d; i++){
			this->hash_functions[i] = new k_wise_family(*other.hash_functions[i]);
		}
	}

	count_sketch::~count_sketch(){
		for(int i = 0; i < this->d; i++){
			free(this->estimators[i]);
			delete this->hash_functions[i];
		}
	
		free(this->estimators);
		free(this->hash_functions);
	}

	void count_sketch::update(int elem, int weight){
		for(int i = 0; i < this->d; i++){
			int hx = (*hash_functions[i])(elem) % (2 * this->t);
			int hi = hx >> 1;

			estimators[i][hi] += g(hx) * weight;
		}
	}

	int count_sketch::query(int elem){
		std::vector<int> ranks;

		ranks.reserve(this->d);

		for(int i = 0; i < this->d; i++){
			int hx = (*hash_functions[i])(elem) % (2 * this->t);
			int hi = hx >> 1;

			ranks.emplace_back(g(hx) * estimators[i][hi]);
		}

		//std::sort(ranks.begin(), ranks.end());
		int size = ranks.size();

		if (size == 0) return -1;

		// picking the median in linear time;
		if ((size & 1)){

			std::nth_element(ranks.begin(), ranks.begin() + size / 2, ranks.end());

			return ranks[size / 2];
		}else{
			nth_element(ranks.begin(), ranks.begin() + size / 2, ranks.end());
  
			nth_element(ranks.begin(), ranks.begin() + (size - 1) / 2, ranks.end());

			return (ranks[size / 2 - 1] + ranks[size / 2]) / 2;
		}
	}

	count_sketch* count_sketch::merge(count_sketch& rhs){
		ASSERT(this->t == rhs.t);
		ASSERT(this->d == rhs.d);

		for(int i = 0; i < this->d; i++){
			ASSERT(this->hash_functions[i]->get_constants() == rhs.hash_functions[i]->get_constants());        
		}

		count_sketch* merged_cs = new count_sketch(this->error, this->delta, this->get_hash_functions_consts());

		for(int i = 0; i < merged_cs->d; i++){
			for(int j = 0; j < merged_cs->t; j++){
				merged_cs->estimators[i][j] = this->estimators[i][j] + rhs.estimators[i][j];
			}
		}

		return merged_cs;
	}

	void count_sketch::inner_merge(count_sketch& rhs){
		ASSERT(this->t == rhs.t);
		ASSERT(this->d == rhs.d);

		for(int i = 0; i < this->d; i++){
			ASSERT(this->hash_functions[i]->get_constants() == rhs.hash_functions[i]->get_constants());
		}

		for(int i = 0; i < this->d; i++){
			for(int j = 0; j < this->t; j++){
				this->estimators[i][j] += rhs.estimators[i][j];
			}
		}
	}

	count_sketch& count_sketch::operator=(const count_sketch& other){
		if(this != &other){
			this->error = other.error;
			this->delta = other.delta;
			this->d = other.d;
			this->t = other.t;

			for(int i = 0; i < this->d; i++){
				free(this->estimators[i]);
			}

			free(this->estimators);

			this->estimators = (int **) malloc(other.d * sizeof(int *));
			for(int i = 0; i < other.d; i++){
				this->estimators[i] = (int *) malloc(other.t * sizeof(int));
				for(int j = 0; j < other.t; j++){
					this->estimators[i][j] = other.estimators[i][j];
				}
			}

			for(int i = 0; i < this->d; i++){
				delete this->hash_functions[i];
			}

			free(this->hash_functions);
	
			this->hash_functions = (k_wise_family **) malloc(other.d * sizeof(k_wise_family *));
			for(int i = 0; i < other.d; i++){
				this->hash_functions[i] = new k_wise_family(*other.hash_functions[i]);
			}
		}

		return *this;
	}

	int count_sketch::get_d() const {
		return this->d;
	}

	int count_sketch::get_t() const {
		return this->t;
	}

	double count_sketch::get_error() const {
		return this->error;
	}

	double count_sketch::get_delta() const {
		return this->delta;
	}

	std::vector<std::vector<int>> count_sketch::get_estimators() const { 
		std::vector<std::vector<int>> copy_estimators;

		copy_estimators.reserve(this->d);

		for(int i = 0; i < this->d; i++){
			copy_estimators.emplace_back(this->estimators[i], this->estimators[i] + this->t);
		}

		return copy_estimators;
	}

	std::vector<k_wise_family> count_sketch::get_hash_functions() const {
		std::vector<k_wise_family> copy_functions;

		copy_functions.reserve(this->d);
		for(int i = 0; i < this->d; i++){
			copy_functions.emplace_back(*this->hash_functions[i]);
		}
		return copy_functions;
	}

	std::vector<std::vector<int>> count_sketch::get_hash_functions_consts() const {
		std::vector<std::vector<int>> consts;

		consts.reserve(this->d);
		for(int i = 0; i < this->d; i++){
			consts.emplace_back(hash_functions[i]->get_constants());
		}

		return consts;
	}
}
