#include "two_wise_family.h"

namespace qsbd {

    two_wise_family::two_wise_family(int universe){
        this->big_prim = nextPrime(universe);

        std::uniform_int_distribution<int> distribution(0, this->big_prim - 1);

        this->universe = universe;
        this->constants[0] = distribution(generator);
        this->constants[1] = distribution(generator);
    }

    two_wise_family::two_wise_family(int universe, const std::vector<int>& conf){
        this->big_prim = nextPrime(universe);
        this->universe = universe;
	
        ASSERT(conf.size() == 2);

        this->constants[0] = conf[0];
        this->constants[1] = conf[1];
    }

    two_wise_family::two_wise_family(const two_wise_family& rhs){
        this->big_prim = rhs.big_prim;
        this->universe = rhs.universe;
        this->constants[0] = rhs.constants[0];
        this->constants[1] = rhs.constants[1];
    }

    two_wise_family::~two_wise_family(){
    }

    int two_wise_family::operator() (int elem) {
        return (this->constants[0] + this->constants[1] * elem) % this->big_prim;
    }

    two_wise_family& two_wise_family::operator=(const two_wise_family& other){
        if(this != &other){
            this->big_prim = other.big_prim;
            this->universe = other.universe;

            this->constants[0] = other.constants[0];
            this->constants[1] = other.constants[1];
        }

        return *this;
    }

    std::vector<int> two_wise_family::get_constants() const {
        return std::vector<int>(this->constants, this->constants + 2);
    }

    int two_wise_family::get_universe() const {
        return this->universe;
    }

    int two_wise_family::get_k() const {
        return 2;
    }
}
