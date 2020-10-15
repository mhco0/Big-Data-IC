#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <map>
#include <set>
#include <cassert>
#include "bob_jenkins_hash.h"

#define ARGS_NUMBER 5
#define SAME_STREAM 1000
#define USE_BJH false		
//#define NDEBUG
using ll = unsigned long long int;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);

// Function that returns true if n  
// is prime else returns false  
bool isPrime(int n)  
{  
    // Corner cases  
    if (n <= 1)  return false;  
    if (n <= 3)  return true;  
    
    // This is checked so that we can skip   
    // middle five numbers in below loop  
    if (n%2 == 0 || n%3 == 0) return false;  
    
    for (int i=5; i*i<=n; i=i+6)  
        if (n%i == 0 || n%(i+2) == 0)  
           return false;  
    
    return true;  
}  
  
// Function to return the smallest 
// prime number greater than N 
int nextPrime(int N) 
{ 
  
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

	//f:[n] -> [n] 
*/
private:
	ll a,b;
	ll universe;
public:

	two_wise_family(){
	}

	two_wise_family(ll m){
		std::uniform_int_distribution<int> distribution(1,m);
		this->a = distribution(generator);
		this->b = distribution(generator);
		this->universe = m;
		// a can't be zero;
		// a and b must be random numbers in [1..m] choose properly after
		// where l
	}

	two_wise_family(ll a, ll b, ll m){
		this->a = a;
		this->b = b;
		this->universe = m;
	}

	ll hash(ll x){
		//See later the reason for the prim commented here
		return (((this->a * x + this->b)  /*% ((1ULL<<61) - 1)*/ ) % (this->universe)) + 1;
	}
};

class non_idealized_fm{
	/*
	- save the max value for distinct elements you saw

		To j* not in [lgt - lgc , logt + lgc]
		then 2^j* not in [t/c , c*t]

		and P[ 2^j* not in [t/c , c*t] ] <= (2^(c+1) + 2^(c-1))/2^(2*c)

	*/
private:
	two_wise_family h;
	ll counter;
	ll universe;
public:

	non_idealized_fm(){
	}

	non_idealized_fm(ll n){
		universe = nextPrime(n);//nextp2(n);
		//after you need to upper the n to the next power of 2 (round up)
		counter = 0ULL;
		h = two_wise_family(universe);
		
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


	double fail_prob(int constant_interval){
		/*
			2^jmax !belongs [t/c,c*t]
			
			~ j !belongs [lgt - lgc , lgt + lgc]

			P[Zj- = 0] = P[|Zj- - E[Zj-]| >= t/(2^((lgt - lgc)+1))] <= [t/(2^((lgt - lgc)+1))]/[t/(2^((lgt - lgc)+1))]^2

			P[Zj- = 0] <= (2^((lgt - lgc)+1))/t 
									
			P[Zj- = 0] <= (2/c)
			
			...

			P[Zj+ >= 1] <= t/(2^((lgt + lgc)+1))
						
			P[Zj+ >= 1] <= (1/(2*c))

			...

			P[j !belongs [lgt - lgc , lgt + lgc]] = 2/c + 1/2*c

			P[j !belongs [lgt - lgc , lgt + lgc]] = (5/(2*c))

		*/
		return 5.0/(2.0*constant_interval);
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
	refined_fm(ll n,double err,double cst){
		universe = nextp2(n);
		constant = cst;
		error = err;

		ts_for_small_values = trivial_solution_dep(n,constant,error);
		fm_to_find_t = non_idealized_fm(n);
		g = two_wise_family(universe);
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

	int many_busted(){
		int busted = 0;

		for(auto&it : ts){
			if(it.bust()) busted++;
		}

		return busted;
	}
};

// BJKST

class bjkst{
private:
	two_wise_family h;
	std::vector<std::set<ll>> bucket;
	int bucket_bottom;
	int bucket_logic_size;
	double bucket_max_size;
	int universe;
	double error;
	double delt;

	double calc_const(){
		/*
			1/12+48/c <= delta/2
			(c+12*48)/12*c <= delta/2
			(c+12*48) <= 12*delta/2*c

			[12*delta/2 - 1]*c >= 12*48
		*/
		assert((6.0*this->delt-1.0) > 0.0);

		return (576.0);//(6.0*this->delt - 1.0);
	}
public:
	bjkst(){
	}

	bjkst(int n,double err,double dlt){
		//calcule const, then compare const / error^2 with bucket_logic_size
		bucket_bottom = 0;
		bucket_logic_size = 0;
		error = err;
		delt = dlt;
		universe = nextp2(n);
		h = two_wise_family(universe);

		bucket.resize(std::log2(universe) + 1,{});

		assert(error == 0.5);

		bucket_max_size = calc_const()/(error*error);
	 
		//std::cout << "bucket sz : "<< bucket_max_size << std::endl;

		assert(bucket_max_size > 576);

	}	

	~bjkst(){
	}

	void update(ll elem){

		assert(lsb(h.hash(elem)) >= 0 and lsb(h.hash(elem)) < (std::log2(universe) + 1));

		if(lsb(h.hash(elem)) >= bucket_bottom){
			if(bucket[lsb(h.hash(elem))].find(elem) == bucket[lsb(h.hash(elem))].end()){
				bucket[lsb(h.hash(elem))].insert(elem);
				bucket_logic_size++;
			}

			while(bucket_logic_size >= bucket_max_size){
				bucket_logic_size -= bucket[bucket_bottom].size();
				bucket[bucket_bottom].clear();

				bucket_bottom++;
			}
		}
	}


	ll query(){
		return (1ULL << bucket_bottom) * bucket_logic_size;
	}

	double error_rate(){
		return error;
	}

	double delta(){
		return delt;
	}

	int busted(){
		return (bucket_bottom);
	}

	double max_store(){
		return bucket_max_size;// (576/e^2)
	}
};

void help(void){
	std::cout << "./fm.exe [--help/--hard] n_events n_distinct_events interval_nfm error_rate_djkst delta_djkst" << std::endl;
	std::cout << ".Doc:" << std::endl;
	std::cout << "--help (optional): Show help guide (this guide)." << std::endl;
	std::cout << "--hard (optional): Do a hardwork test for SAME_STREAM and show the probs." << std::endl;
	std::cout << "n_events (required): Number of events to stream computes." << std::endl;
	std::cout << "n_distinct_events (required): Number of distinct numbers on the stream (0 <= n_distinct_events <= n_events)." << std::endl;
	std::cout << "interval_nfm (required) : Interval for Non Idealized FM just for probability test." << std::endl;
	std::cout << "error_rate_djkst (required): Error rate used in Bjkst algorithm." << std::endl;
	std::cout << "delta_djkst (required): Fail probability for Bjkst algorithm." << std::endl;
}

void print_fm_summary(int n_distinct_events,int interval_nfm,non_idealized_fm& fm){
	std::cout << "--------------------BENCHMARK-FM-------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "True value := " << n_distinct_events << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "Using Non Idealized Flajolet-Martin Algorithm: " << std::endl;
	std::cout << "Value predicted := " << fm.query() << std::endl;
	std::cout << "Diff Values := " << std::max((ll)n_distinct_events,fm.query()) - std::min((ll)n_distinct_events,fm.query()) << std::endl;
	std::cout << "Fail Prob := " << fm.fail_prob(interval_nfm)*100.0 << "%" << std::endl;
	std::cout << "Interval expected := " << "[" << n_distinct_events*1.0/interval_nfm <<" , " <<interval_nfm*n_distinct_events <<"]" << std::endl;
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

void print_bjkst_summary(int n_distinct_events,bjkst& fm){
	std::cout << "------------------------BJKST----------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "True value := " << n_distinct_events << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "Using BJKST Algorithm: " << std::endl;
	std::cout << "Value predicted := " << fm.query() << std::endl;
	std::cout << "Error rate used := " << fm.error_rate() << std::endl;
	std::cout << "Fail prob := " << fm.delta()*100.0 << "%" <<  std::endl;
	std::cout << "Diff Values := " << std::max((ll)n_distinct_events,fm.query()) - std::min((ll)n_distinct_events,fm.query()) << std::endl;
	std::cout << "How many buckets bust := " << fm.busted() << std::endl;
	std::cout << "Should be write up to := " << fm.max_store() << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
}

int testHash(){
	std::vector<int> v(129,0);

	for(int i=1;i<=128;i++){
		for(int j=1;j<=128;j++){
			two_wise_family h(i,j,128);

			for(int k=1;k<=128;k++){
				v[h.hash(k)]++;
			}
		}
	}

	for(int i=0;i<129;i++){
		std::cout << "(" << i << " , " << v[i] << ") ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	return 0;
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
	else std::cout << "Using two wish family hash..." << std::endl;
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
		int interval_nfm = std::stoi(args[2]);
		double constant_used = std::stod(args[3]);

		double fm_times_wrong_med = 0;
		double bjkst_times_wrong_med = 0;
		ll fm_average_value = 0;
		ll bjkst_average_value = 0;

		for(int j=0;j<SAME_STREAM;j++){

			non_idealized_fm fm(numbers_on_stream.size());
			bjkst bjk(numbers_on_stream.size(),std::stod(args[3]),std::stod(args[4]));
			bool fm_wrong_on_epoch = false;
			bool bjkst_wrong_on_epoch = false;


			std::cout << "On epoch (" << j + 1 << ") : " << std::endl;


			for(int i=0;i<numbers_on_stream.size();i++){
				fm.update(numbers_on_stream[i]);
				//bjk.update(numbers_on_stream[i]);
			}

			if((fm.query()*1.0 < (distinct*1.0/interval_nfm*1.0)) or (fm.query()*1.0 > (interval_nfm*distinct*1.0))){
				fm_wrong_on_epoch = true;
			}

			/*if(std::fabs(bjk.query() - distinc) > (bjk.error_rate() * distinct)){
				bjkst_wrong_on_epoch = true;
			}*/

			std::cout << "fm wrong on epoch := " << fm_wrong_on_epoch << std::endl;
			//std::cout << "bjkst wrong on epoch := " << bjkst_wrong_on_epoch << std::endl;

			fm_times_wrong_med += fm_wrong_on_epoch;
			bjkst_times_wrong_med += bjkst_wrong_on_epoch;

			fm_average_value += fm.query();
			bjkst_average_value += bjk.query();

			std::cout << std::endl << std::endl;

			print_fm_summary(distinct,interval_nfm,fm);
			std::cout << std::endl;
			//print_bjkst_summary(distinct,bjk);
			std::cout << std::endl << std::endl;
		}

		std::cout << "RESUME:" << std::endl;

		std::cout << "fm median output := " << fm_average_value*1.0/SAME_STREAM << std::endl;
		//std::cout << "bjkst median output := " << bjkst_average_value*1.0/SAME_STREAM << std::endl;

		std::cout << "fm median for errors := " << (fm_times_wrong_med*100.0/(SAME_STREAM)) << "%" << std::endl;
		//std::cout << "bjkst median for errors := " << (bjkst_times_wrong_med*100.0/(SAME_STREAM)) << "%" << std::endl;

	}else{
		int interval_nfm = std::stoi(args[2]);
		non_idealized_fm fm(numbers_on_stream.size());
		bjkst bjk(numbers_on_stream.size(),std::stod(args[3]),std::stod(args[4]));


		for(int i=0;i<numbers_on_stream.size();i++){
			fm.update(numbers_on_stream[i]);
			bjk.update(numbers_on_stream[i]);
		}

		print_fm_summary(distinct,interval_nfm,fm);
		std::cout << std::endl;
		print_bjkst_summary(distinct,bjk);
	}
	
	std::cout << "finishing..." << std::endl;

	return 0;
}		