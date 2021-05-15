#include "k_wise_family.h"

namespace qsbd {
    k_wise_family::k_wise_family(int k, unsigned long long int universe){
        this->big_prim = nextPrime(universe);

        std::uniform_int_distribution<unsigned long long int> distribution(1LL, this->big_prim - 1);

        this->_k = k;
        this->universe = universe;

        for(int i = 0; i < _k; i++){
            constants.push_back(distribution(generator));
        }
    }

    k_wise_family::k_wise_family(int k, unsigned long long int universe, const std::vector<ll>& conf){
        this->big_prim = nextPrime(universe);
        this->_k = k;
        this->universe = universe;

        assert(conf.size() == _k);

        for(int i = 0; i < conf.size(); i++){
            constants.push_back(conf[i]);
        }
    }

    unsigned long long int k_wise_family::operator() (unsigned long long int elem){
        unsigned long long int parse_sum = 0LL;

        for(int i = 0; i < this->_k; i++){
            unsigned long long int exp_x = 1LL;

            for(int j = 0; j < i; j++){
                exp_x = exp_x * elem;
            }

            parse_sum += this->constants[i] * exp_x;
        }

        return (parse_sum % this->big_prim);
    }

    std::vector<unsigned long long int> k_wise_family::get_constants() const {
        return this->constants;
    }

    unsigned long long int k_wise_family::get_universe() const {
        return this->universe;
    }

    int k_wise_family::get_k() const {
        return this->_k;
    }
}