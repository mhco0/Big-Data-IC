#include "k_wise_family.h"

// Function that returns true if n  
// is prime else returns false  
bool isPrime(int n){  
    // Corner cases  
    if (n <= 1)  return false;  
    if (n <= 3)  return true;  
    
    // This is checked so that we can skip   
    // middle five numbers in below loop  
    if (n%2 == 0 || n%3 == 0) return false;  
    
    for (int i = 5; i*i <= n; i = i + 6)  
        if (n%i == 0 || n%(i+2) == 0)  
           return false;  
    
    return true;  
}  

// Function to return the smallest 
// prime number greater than N 
int nextPrime(int N){ 
  
    // Base case 
    if (N <= 1) 
        return 2; 
  
    int prime = N; 
    bool found = false; 
  
    // Loop continuously until isPrime returns 
    // true for a number greater than n 
    while (!found) { 
        prime++; 
  
        if (isPrime(prime)) 
            found = true; 
    } 
  
    return prime; 
}

k_wise_family::k_wise_family(int k, unsigned long long int universe){
    this->big_prim = nextPrime(universe);

    std::uniform_int_distribution<unsigned long long int> distribution(1LL, this->big_prim - 1);

    this->k_indepent = k;
    this->universe = universe;


    this->constants.assign(k_indepent, 0);

    for(auto& it : constants){
        it = distribution(generator);
    }
}

k_wise_family::k_wise_family(int k, unsigned long long int universe, std::vector<ll> conf){
    this->big_prim = nextPrime(universe);
    this->k_indepent = k;
    this->universe = universe;
    this->constants.assign(k_indepent, 0);

    assert(conf.size() == constants.size());

    for(int i = 0; i < conf.size(); i++){
        constants[i] = conf[i];
    }

}

unsigned long long int k_wise_family::operator() (unsigned long long int elem){
    unsigned long long int parse_sum = 0LL;

    for(int i = 0; i < this->k_indepent; i++){
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