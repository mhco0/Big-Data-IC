#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <map>
#include <set>
#include "bob_jenkins_hash.h"
#define ARGS_NUMBER 4
#define SAME_STREAM 200
#define USE_BJH false
using ll = unsigned long long int;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);

ll lsb(ll y){
	
	if(y == 0ULL) throw std::invalid_argument("0 don't has a bit set");

	ll index = 0ULL;

	while(!(y & (1ULL << index))){
		index++;
	}

	return index;
}

ll nextp2(ll x){
	return (1ULL << (int)std::ceil(std::log2(x)));
}

class two_wise_family{
/*
	The class for two-wise family functions.

	//f:[n] -> [n] m-> log2(n) 
*/
private:
	ll a,b;
	ll universe;
public:

	two_wise_family(){
	}

	two_wise_family(ll&& m){
		// This can be changed later to not use only log2(m) and just m.
		std::uniform_int_distribution<int> distribution(1,(1ULL << m));
		this->a = distribution(generator);
		this->b = distribution(generator);
		this->universe = (1ULL << m);
		// a can't be zero;
		// a and b must be random numbers in [1..2^(log2(n))] choose properly after
		// where log2(n) = m
	}


	ll hash(ll x){

		//See later the reason for the prim commented here
		return ((this->a * x + this->b)  /*% ((1ULL<<61) - 1)*/ ) % (this->universe) + 1;
	}
};

class non_idealized_fm{
	/*
	- save the max value for distinct elements you saw
	*/
private:
	two_wise_family h;
	ll counter;
	ll universe;
public:

	non_idealized_fm(){
	}

	non_idealized_fm(ll n){
		universe = nextp2(n);
		counter = 0ULL;
		h = two_wise_family(std::log2(universe));
		//after you need to upper the n to the next power of 2 (round up)
	}

	~non_idealized_fm(){
	}

	void update(ll stream){
		//std::cout <<"My universe : " << universe << std::endl;
		//std::cout <<"log2(universe) : " << std::log2(universe) << std::endl;

		if(!USE_BJH) counter = std::max(counter, lsb(h.hash(stream)));
		else counter = std::max(counter,lsb(bob_jenkins_hash::hash(ull_to_bytes(stream))));
	}
 
	ll query(void){
		return (1ULL << counter);
	} 
};

class trivial_solution_dep{
private:
	std::set<ll> many_seen;
	double error;
	double constant;
	int store;
	int logic;
	bool busted;
	/* 
		c/e^2
	*/
public:
	trivial_solution_dep(){
	}


	trivial_solution_dep(ll n,double cst, double err){
		constant = cst;
		error = err;
		logic = 0;
		busted = false;
		store = (int) std::ceil(constant/(error*error));

		//std::cout << store << std::endl;
		// save the elements in the set
	}

	~trivial_solution_dep(){
	}

	void update(ll elem){
		if(logic < store){
			if(many_seen.find(elem) == many_seen.end()){
				many_seen.insert(elem);
				logic++;
			}
		}else{
			busted = true;
		}
	}	

	int seen(){
		return logic;
	}

	bool bust(){
		return busted;
	}
};


class refined_fm{
private:
	/*
		to answer the cases where we cannot find such a j that t/(2^(j+1)) ~= 1/(error*error)

		this happens for small values that we find

		we can also run in parallel a copy of TS that asnwer the question for small values,

		t < 10/e^2
	
		for values greater than this we use the j that we find in non_idealized_fm

		trivial_solution_dep ts_for_small_values(n,cst,err);

		for small values of j , jmax can be < 0 so, we must run a extra Ts for this cases

		we also must run a parallel copy of non_idealized_fm to pick the correct value for ~t 
	*/
	std::vector<trivial_solution_dep> ts;
	trivial_solution_dep ts_for_small_values;
	non_idealized_fm fm_to_find_t;
	two_wise_family g;
	ll universe;
	double error;
	double constant;

	ll jmax(void){
		return ((ll)std::log2(fm_to_find_t.query()*error*error) - 1ULL);
	}

public:
	refined_fm(ll n,double cst,double err){
		universe = nextp2(n);
		constant = cst;
		error = err;

		ts_for_small_values = trivial_solution_dep(n,constant,error);
		fm_to_find_t = non_idealized_fm(n);
		g = two_wise_family(std::log2(universe));
		ts.resize((ll)std::log2(universe) + 1ULL,trivial_solution_dep(n,constant,error));
	}

	~refined_fm(){
	}

	void update(ll stream){

		/*
			std::cout << "size := ";
			std::cout << ts.size() << std::endl;
			std::cout << "hash_value := ";
			std::cout << lsb(h_map(stream,(ll)std::log2(universe))) << std::endl;
	
			search for c++ assert lib
			this is because assert lib can change with DEBUG mode
		*/

		ts_for_small_values.update(stream);
		fm_to_find_t.update(stream);
		if (!USE_BJH){
			ts[lsb(g.hash(stream))].update(stream);
		}else{
			ts[lsb(bob_jenkins_hash::hash(ull_to_bytes(stream)))].update(stream);
		}
	}

	ll query(){
		/*
			remember that j must be find where 

			t/(2^(j+1)) ~= 1/(error)^2
		*/

		ll j_max = this->jmax();

		if(j_max >= 0ULL and j_max < ts.size()) return (1ULL << (j_max + 1ULL)) * ts[j_max].seen();
		else return ts_for_small_values.seen();
	}

	double error_rate(){
		return error;
	}

	double constant_used(){
		return constant;
	}
};

void help(void){
	std::cout << "./fm.exe [--help/--hard] n_events n_distinct_events constant_refined error_rate_refined" << std::endl;
	std::cout << ".Doc:" << std::endl;
	std::cout << "--help (optional): Show help guide (this guide)." << std::endl;
	std::cout << "--hard (optional): Do a hardwork test for SAME_STREAM and show the probs." << std::endl;
	std::cout << "n_events (required): Number of events to stream computes." << std::endl;
	std::cout << "n_distinct_events (required): Number of distinct numbers on the stream (0 <= n_distinct_events <= n_events)." << std::endl;
	std::cout << "constant_refined (required): constant used in refined fm algorithm test for how many elements the TS will store." << std::endl;
	std::cout << "error_rate_refined (required): Error rate for refined fm algorithm." << std::endl;
}

void print_fm_summary(int n_distinct_events,non_idealized_fm& fm){
	std::cout << "--------------------BENCHMARK-FM-------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "True value := " << n_distinct_events << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "Using Non Idealized Flajolet-Martin Algorithm: " << std::endl;
	std::cout << "Value predicted := " << fm.query() << std::endl;
	std::cout << "Diff Values := " << std::max((ll)n_distinct_events,fm.query()) - std::min((ll)n_distinct_events,fm.query()) << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
}

void print_refined_fm_summary(int n_distinct_events,refined_fm& fm){
	std::cout << "--------------------BENCHMARK-RFM------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "True value := " << n_distinct_events << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "Using Refined Flajolet-Martin Algorithm: " << std::endl;
	std::cout << "Value predicted := " << fm.query() << std::endl;
	std::cout << "Constant used := " << fm.constant_used() << std::endl;
	std::cout << "Error rate used := " << fm.error_rate() << std::endl;
	std::cout << "Error rate^2 := " << fm.error_rate() * fm.error_rate() << std::endl;
	std::cout << "Correct answer up to " << fm.constant_used()/(fm.error_rate()*fm.error_rate()) << std::endl;
	std::cout << "Diff Values := " << std::max((ll)n_distinct_events,fm.query()) - std::min((ll)n_distinct_events,fm.query()) << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
}


int main(int argc,char * argv[]){
	std::deque<std::string> args;
	std::vector<ll> numbers_on_stream;
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);
	bool hardwork = false;

	for(int i=1;i<argc;i++){
		std::string s(argv[i]);

		args.push_back(s);
	}

	if(args[0] == "--help"){
		help();
		return 0;
	}

	if(args[0] == "--hard"){
		hardwork = true;
		args.pop_front();
	}

	if(args.size() != ARGS_NUMBER){
		std::cout << "Invalid parameters, try --help" << std::endl;
		return 0;
	}

	if (std::stoi(args[0]) < std::stoi(args[1])){
		std::cout << "Error: arg[0] must be greater than arg[1]" << std::endl;
		help();
		return 0;
	}

	if(USE_BJH) std::cout << "Using Bob Jenkins Hash..." << std::endl;
	else std::cout << "Using h_map hash..." << std::endl;
	std::cout << std::endl;

	ll distinct = std::stoi(args[1]);
	std::uniform_int_distribution<ll> values_to_copy(1ULL,distinct);
	numbers_on_stream.resize(std::stoi(args[0]),0ULL);
	
	for(ll i=0;i<distinct;i++){
		numbers_on_stream[i] = i + 1;
	}

	for(int i=distinct;i<numbers_on_stream.size();i++){
		numbers_on_stream[i] = values_to_copy(generator);
	}

	std::random_shuffle(numbers_on_stream.begin(),numbers_on_stream.end());

	if(hardwork){
		double constant_used = std::stod(args[2]);

		double fm_times_wrong_med = 0;
		double rfm_times_wrong_med = 0;
		ll fm_median_value = 0;
		ll rfm_median_value = 0;

		for(int j=0;j<SAME_STREAM;j++){

			non_idealized_fm fm(numbers_on_stream.size());
			refined_fm rfm(numbers_on_stream.size(),std::stod(args[2]),std::stod(args[3]));
			std::vector<bool> elements_so_far(numbers_on_stream.size(),false);
			int fm_times_wrong = 0;
			int rfm_times_wrong = 0;
			ll distinct_saw_so_far = 0;


			std::cout << "On epoch (" << j + 1 << ") : " << std::endl;


			for(int i=0;i<numbers_on_stream.size();i++){
				if(!elements_so_far[numbers_on_stream[i]]){
					elements_so_far[numbers_on_stream[i]] = true;
					distinct_saw_so_far++;
				}


				fm.update(numbers_on_stream[i]);
				rfm.update(numbers_on_stream[i]);

				//ll fm_diff = std::max(fm.query(),distinct_saw_so_far) - std::min(fm.query(),distinct_saw_so_far);
				//ll rfm_diff = std::max(fm.query(),distinct_saw_so_far) - std::min(fm.query(),distinct_saw_so_far);

				if(fm.query()*1.0 < distinct_saw_so_far*1.0/constant_used or fm.query()*1.0 > constant_used*distinct_saw_so_far*1.0 ) fm_times_wrong++;		
				if(rfm.query()*1.0 < distinct_saw_so_far*1.0/constant_used or rfm.query()*1.0 > constant_used*distinct_saw_so_far*1.0 ) rfm_times_wrong++;

				//NOTE : Later implementation -> P[|Zj* - EZj*|< some constant] < other constant
			}

			std::cout << "fm times wrong on epoch := " << fm_times_wrong << std::endl;
			std::cout << "rfm times wrong on epoch := " << rfm_times_wrong << std::endl;

			fm_times_wrong_med += fm_times_wrong;
			rfm_times_wrong_med += rfm_times_wrong;

			fm_median_value += fm.query();
			rfm_median_value += rfm.query();

			std::cout << std::endl << std::endl;

			print_fm_summary(distinct,fm);
			std::cout << std::endl;
			print_refined_fm_summary(distinct,rfm);
			std::cout << std::endl << std::endl;
		}

		std::cout << "fm median output := " << fm_median_value*1.0/SAME_STREAM << std::endl;
		std::cout << "rfm median output := " << rfm_median_value*1.0/SAME_STREAM << std::endl;

		std::cout << "fm median for errors := " << (fm_times_wrong_med*100.0/(SAME_STREAM*numbers_on_stream.size())) << "%" << std::endl;
		std::cout << "rfm median for errors := " << (rfm_times_wrong_med*100.0/(SAME_STREAM*numbers_on_stream.size())) << "%" << std::endl;

	}else{
		non_idealized_fm fm(numbers_on_stream.size());
		refined_fm rfm(numbers_on_stream.size(),std::stod(args[2]),std::stod(args[3]));


		for(int i=0;i<numbers_on_stream.size();i++){
			fm.update(numbers_on_stream[i]);
			rfm.update(numbers_on_stream[i]);
		}

		print_fm_summary(distinct,fm);
		std::cout << std::endl;
		print_refined_fm_summary(distinct,rfm);
	}

	
	return 0;
}		