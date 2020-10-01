#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <map>
#define ARGS_NUMBER 2
using ll = unsigned long long int;


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);

ll lsb(ll y){
	
	if(y == 0) throw std::invalid_argument("0 don't has a bit set");

	ll index = 0;

	while(!(y & index)){
		index++;
	}

	return index;
}

ll nextp2(ll x){
	return (1ULL << (int)std::ceil(std::log2(x)));
}

ll h_map(ll x,ll && m){
	std::uniform_int_distribution<int> distribution(1,(1ULL << m));
	static ll a = distribution(generator);
	static ll b = distribution(generator);
	// a can't be zero;
	// a and b must be random numbers in [1..2^m] choose properly after

	/*
	std::cout << "1^m : " <<(1ULL<<m) << std::endl;
	std::cout << "prim : " << ((1ULL)<<61 - 1) << std::endl;
	std::cout << "a : " << a << std::endl;
	std::cout << "b : " << b << std::endl;
	std::cout << "x : " << x << std::endl;

	std::cout << "a*x : " << a*x << std::endl;
	std::cout << "a*x + b : " << a*x + b << std::endl;

	std::cout << "(a*x + b) mod prim : " << (a*x + b) % ((1ULL<<61) - 1) << std::endl;
	std::cout << "((a*x + b) mod prim ) mod 1^m : " << ((a*x + b) % ((1ULL<<61) - 1)) % (1ULL<<m) << std::endl;

	std::cout << "------------" << std::endl;
	*/

	return ((a*x + b) % ((1ULL<<61) - 1)) % (1ULL<<m) + 1;
}

class non_idealized_fm{
	/*
	- P[|n-ñ| > e*n] < delta
	*/
private:
	ll counter;
	ll universe;
public:
	
	non_idealized_fm(ll n){
		universe = nextp2(n);
		counter = 0LL;

		//after you need to upper the n to the next power of 2 (round up)
	}

	~non_idealized_fm(){
	}

	void update(ll stream){
		//std::cout <<"My universe : " << universe << std::endl;
		//std::cout <<"log2(universe) : " << std::log2(universe) << std::endl;

		counter = std::max(counter, lsb(h_map(stream,(ll)std::log2(universe))));
		// this 32 may be log2(universe) try this later
	}
 
	ll query(void){
		return (1ULL << counter);
	} 
};

class trivial_solution_dep{
private:
	double error;
	double constant;
	int store;
	std::map<ll,int> many_saw;
	/* 
		Use map is a bad approximation because we use O(N*logN), and we can solve with
		O(logN) change later;
	*/

public:
	trivial_solution_dep(){
	}

	trivial_solution_dep(double cst, double err){
		constant = cst;
		error = err;
		store = (int) std::ceil(constant/(error*error));
	}

	~trivial_solution_dep(){
	}

	void update(ll elem){
		if(many_saw.size() < store){
			if(!many_saw[elem]){
				many_saw[elem] = 1;
			}
		}
	}	

	int saw(){
		return many_saw.size();
	}
};


class refined_ts_dep{
private:
	/*
		to asnwer the cases where we cannot find such a j that t/(2^(j+1)) ~= 1/(error*error)
		we can also run in parallel a copy of TS that asnwer the question for small values,

		t < 10/e^2
	
		for values greater than this we use the j that we find in non_idealized_fm

		trivial_solution_dep ts_for_small_values(cst,err);

	*/
	std::vector<trivial_solution_dep> ts;
	ll universe;
	double error;
	double constant;
	int j;
public:
	refined_ts_dep(ll n,double cst,double err,int jax){
		//this jax here is only for tests, the non_idealized_fm must run in parallel here 
		//to pick the real j used for the stream.
		universe = nextp2(n);
		constant = cst;
		error = err;

		ts.resize((ll)std::log2(universe),trivial_solution_dep(constant,error));
	}
	~refined_ts_dep(){
	}

	void update(ll stream){
		ts[lsb(h_map(stream,(ll)std::log2(universe)))].update(stream);
	}


	int query(){
		/*
			remember that j must be find where 

			t/(2^(j+1)) ~= 1/(error)^2
		*/

		return (1<<(j+1))*ts[j].saw();
	}
};

void help(void){
	std::cout << "./morris.exe [--help] n_events n_distinct_events delta_morris_p error_rate_morris_pp delta_morris_pp morris_relative" << std::endl;
	std::cout << ".Doc:" << std::endl;
	std::cout << "--help (optional): Show help guide (this guide)." << std::endl;
	std::cout << "n_events (required): Number of events to stream computes." << std::endl;
	std::cout << "n_distinct_events (required): Number of distinct numbers on the stream (0 <= n_distinct_events <= n_events)." << std::endl;
	std::cout << "delta_morris_p (required): Fail probability used in morris_p algorithm test." << std::endl;
	std::cout << "error_rate_morris_pp (required): Error rate for morris_pp algorithm." << std::endl;
	std::cout << "delta_morris_pp (required): Fail propability used in morris_pp algorithm." << std::endl;
	std::cout << "morris_relative (required): E-risk expected for morris algorithm.(>~ 70%)" << std::endl;
}

void print_summary(int n_distinct_events,non_idealized_fm& fm){
	std::cout << "--------------------BENCHMARK------------------" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "True value := " << n_distinct_events << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Using Non Idealized Flajolet-Martin Algorithm: " << std::endl;
	std::cout << "Value predicted := " << fm.query() << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
}


int main(int argc,char * argv[]){
	std::deque<std::string> args;
	std::vector<ll> numbers_on_stream;
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);

	for(int i=1;i<argc;i++){
		std::string s(argv[i]);

		args.push_back(s);
	}

	if(args[0] == "--help"){
		help();
		return 0;
	}

	if(args.size() != ARGS_NUMBER){
		std::cout << "Invalid parameters, try --help" << std::endl;
		return 0;
	}

	ll distinct = std::stoi(args[1]);
	std::uniform_int_distribution<ll> values_to_copy(1ULL,distinct);
	numbers_on_stream.resize(std::stoi(args[0]),0ULL);
	non_idealized_fm fm(numbers_on_stream.size());

	
	for(ll i=0;i<distinct;i++){
		numbers_on_stream[i] = i + 1;
	}

	for(int i=distinct;i<numbers_on_stream.size();i++){
		numbers_on_stream[i] = values_to_copy(generator);
	}

	for(int i=0;i<numbers_on_stream.size();i++){
		std::cout << numbers_on_stream[i] << " ";
	}
	std::cout << std::endl;

	std::random_shuffle(numbers_on_stream.begin(),numbers_on_stream.end());

	for(int i=0;i<numbers_on_stream.size();i++){
		std::cout << numbers_on_stream[i] << " ";
	}
	std::cout << std::endl;

	for(int i=0;i<numbers_on_stream.size();i++){
		fm.update(numbers_on_stream[i]);
	}

	print_summary(distinct,fm);

	return 0;
}		