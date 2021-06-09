#include "k_wise_family.h"

namespace qsbd {

    k_wise_family::k_wise_family(int k, int universe){
        this->big_prim = nextPrime(universe);

        std::uniform_int_distribution<int> distribution(1, this->big_prim - 1);

        this->_k = k;
        this->universe = universe;

        this->constants.reserve(_k);
	    for(int i = 0; i < _k; i++){
            this->constants.push_back(distribution(generator));
        }
    }

    k_wise_family::k_wise_family(int k, int universe, const std::vector<int>& conf){
        this->big_prim = nextPrime(universe);
        this->_k = k;
        this->universe = universe;

        ASSERT(conf.size() == this->_k);
	
	    this->constants.reserve(this->_k);
        for(int i = 0; i < conf.size(); i++){
            this->constants.push_back(conf[i]);
        }
    }

    k_wise_family::k_wise_family(const k_wise_family& rhs){
        this->big_prim = rhs.big_prim;
        this->_k = rhs._k;
        this->universe = rhs.universe;

	    this->constants.reserve(rhs.constants.size());
        for(int i = 0; i < rhs.constants.size(); i++){
            this->constants.push_back(rhs.constants[i]);
        }
    }

    k_wise_family::~k_wise_family(){
    }

    int k_wise_family::operator() (int elem) {
        int parse_sum = 0LL;

        for(int i = 0; i < this->_k; i++){
            int exp_x = 1LL;

            for(int j = 0; j < i; j++){
                exp_x = exp_x * elem;
            }

            parse_sum += this->constants[i] * exp_x;
        }

        return (parse_sum % this->big_prim);
    }

    k_wise_family& k_wise_family::operator=(const k_wise_family& other){
        if(this != &other){
            this->big_prim = other.big_prim;
            this->_k = other._k;
            this->universe = other.universe;
            this->constants.resize(other.constants.size());

            for(int i = 0; i < other.constants.size(); i++){
                this->constants[i] = other.constants[i];
            }
        }

        return *this;
    }

    std::vector<int> k_wise_family::get_constants() const {
        return this->constants;
    }

    int k_wise_family::get_universe() const {
        return this->universe;
    }

    int k_wise_family::get_k() const {
        return this->_k;
    }
}
