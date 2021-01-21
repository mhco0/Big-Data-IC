#ifndef K_WISE_FAMILY_H
#define K_WISE_FAMILY_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
 
bool isPrime(int n);
int nextPrime(int N);

class k_wise_family{
private:
	using ll = unsigned long long int;

 	ll big_prim = 2LL;
	std::vector<ll> constants;
 	ll universe;
 	int k_indepent;
public:
	k_wise_family(int k, ll universe);

	k_wise_family(int k, ll universe, std::vector<ll> conf);

	~k_wise_family(){}
	
	ll operator() (ll elem);

	std::vector<ll> get_constants() const;
};
#endif