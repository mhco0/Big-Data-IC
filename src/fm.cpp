#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <random>
using ll = long long int;

ll lsb(ll y){
	int index = 0;

	while((~y) & 1){
		y >>= 1;
		index++;
	}

	return index;
}

ll h_map(ll x,ll && m){
	std::default_random_engine gen;
	std::uniform_int_distribution<int> distribution(0,(1 << m));
	static int a = (distribution(gen) + 1) % (1 << m),b = distribution(gen);
	// a can't be zero;
	// a and b here aren't 1 , they must be random numbers in [1..2^m] choose properly after

	return ((a*x + b) % ((1LL<<61) - 1)) % (1<<m);
}

class non_idealized_fm{
private:
	ll counter;
	int universe;
public:
	
	non_idealized_fm(ll n){
		universe = n;
		counter = 0;

		//after you need to upper the n to the next power of 2 (round up)
	}

	void update(ll stream){
		counter = std::max(counter, lsb(h_map(stream,64LL)));
	}

	ll query(void){
		return (1LL << counter) + 1;
	} 
};



int main(int argc,char * argv[]){
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);

	return 0;
}		