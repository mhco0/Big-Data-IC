#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <map>
#include <set>
#include <cassert>
#include "bob_jenkins_hash.h"

#define ARGS_NUMBER 4
#define SAME_STREAM 1
#define USE_BJH false		
#define NDEBUG
using ll = unsigned long long int;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);
ll diff_max_without_g = 0ULL;
ll diff_max_with_g = 0ULL;
// Function that returns true if n  
// is prime else returns false  
bool isPrime(int n){  
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

ll lsb(ll y){
	
	if(y == 0ULL) throw std::invalid_argument("0 don't has a bit set");

	ll index = 0ULL;

	while(!(y & (1ULL << index))){
		index++;
	}

	return index;
}

std::vector<unsigned short> binaryExpansion(ll y, ll expansionSize){
	std::vector<unsigned short> result(expansionSize,0);

	ll index = 0ULL;

	while(index < expansionSize){
		if((y & (1ULL << index)))
			result[index] = 1;
		index++;
	}

	return result;
}

ll binaryToDecimal(std::vector<unsigned short>& expansion){
	ll expansionSize = expansion.size();
	ll index = 0ULL;
	ll result = 0ULL;
	
	while(index < expansionSize){
		if(expansion[index])
			result += ((1ULL << index));
		index++;
	}

	return result;
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
		return (((this->a * x + this->b)  % ((1ULL<<61) - 1) ) % (this->universe)) + 1;
	}
};


class binary_two_wise_family{
/*
	The class for two-wise family functions.

	//f:[n] -> [n] 
*/
private:
	ll domainBits;
	ll rangeBits;
	//
	std::vector<unsigned short> binaryMatrix;
	std::vector<unsigned short> binaryCteVector;
public:

	binary_two_wise_family(){
	}

	binary_two_wise_family(ll db, ll rb):
	domainBits(db),
	rangeBits(rb) {
		std::uniform_int_distribution<int> distribution(0,1);

		//
		for(ll i = 0 ; i < domainBits ; ++i){
			for(ll j = 0 ; j < rangeBits ; ++j){
				binaryMatrix.push_back(distribution(generator));
			}
		}
		//
		for(ll j = 0 ; j < rangeBits ; ++j){
			binaryCteVector.push_back(distribution(generator));
		}
	}

	ll hash(ll x){
		using namespace std;
		//write binary expantion of x with domainBits size
		vector<unsigned short> xb = binaryExpansion(x,domainBits);

		//compute result
		vector<unsigned short> result;
		for(ll k = 0 ; k < rangeBits ; ++k){
			short aux = 0;
			for(ll n = 0 ; n < domainBits ; ++n){
				aux = (aux + binaryMatrix[k*domainBits + n]*xb[n]) % 2;
			}
			result.push_back(aux);
		}
		
		//get back to result
		return binaryToDecimal(result) + 1;
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
	binary_two_wise_family h;
	ll counter;
	ll universe;
public:

	non_idealized_fm(){
	}

	non_idealized_fm(ll n){
		universe = nextp2(n);
		//after you need to upper the n to the next power of 2 (round up)
		counter = 0ULL;
		h = binary_two_wise_family(log2(universe),log2(universe));
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
	binary_two_wise_family h;
	std::vector<std::set<ll>> bucket;
	ll universe;
	ll bucket_bottom;
	ll bucket_logic_size;
	double bucket_max_size;
	double error;

	binary_two_wise_family g;
	std::set<std::pair<ll,ll>> bucket2;
	ll bucket_bottom2;
	ll bucket_logic_size2;
	double space_amout = 3.0;
	bool use_g_hash;
public:
	bjkst(){
	}

	bjkst(int n,double err,bool ugh = false){
		bucket_bottom = 0ULL;
		bucket_logic_size = 0ULL;
		bucket_bottom2 = 0ULL;
		bucket_logic_size2 = 0ULL;
		error = err;
		universe = nextp2(n);
		h = binary_two_wise_family(std::log2(universe),std::log2(universe));

		ll contra_domain = space_amout*(1.0/(error*error*error*error))*std::log2(universe)*std::log2(universe);
		g = binary_two_wise_family(std::log2(universe),std::log2(contra_domain));

		bucket.resize(std::log2(universe) + 1,{});

		bucket_max_size = 576.0/(error*error);

		use_g_hash = ugh;
	}	

	~bjkst(){
	}

	void update(ll elem){

		assert(lsb(h.hash(elem)) >= 0 and lsb(h.hash(elem)) < (std::log2(universe) + 1));

		if(!use_g_hash){
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
		}else{
			if(lsb(h.hash(elem)) >= bucket_bottom2){
				if(bucket2.find({g.hash(elem),lsb(h.hash(elem))}) == bucket2.end()){
					bucket2.insert({g.hash(elem),lsb(h.hash(elem))});
					bucket_logic_size2++;
				}

				while(bucket_logic_size2 >= bucket_max_size){
					bool shrink_all = false;

					do{
						auto it = std::find_if(bucket2.begin(),bucket2.end(),[this](const std::pair<ll,ll>& p){
							return (p.second == bucket_bottom2);
						});

						if(it != bucket2.end()){
							bucket2.erase(it);

							bucket_logic_size2--;
						}else{
							shrink_all = true;
						}

					}while(!shrink_all);

					bucket_bottom2++;
				}
			}
		}
	}

	ll query(){
		if (use_g_hash) return (1ULL << bucket_bottom2) * bucket_logic_size2;
		else return (1ULL << bucket_bottom) * bucket_logic_size;
	}

	double error_rate(){
		return error;
	}

	double delta(){
		return 1.0/6.0;
	}

	int busted(){
		return bucket_bottom;
	}

	double max_store(){
		return bucket_max_size;// (576/e^2)
	}

	void final_state(){
		std::ofstream f;

		f.open("bjkst_final_state.out",std::ios::ate);

		if(!use_g_hash){
			f << "z (without g): " << bucket_bottom << std::endl;
			f << "bucket size (without g): " << bucket_logic_size << std::endl;
			f << std::endl;
		}else{
			f << "z (with g): " << bucket_bottom2 << std::endl;
			f << "bucket size (with g): " << bucket_logic_size2 << std::endl;
			f << std::endl;
		}

		f << "bucket max : " << bucket_max_size << std::endl;

		f.close();
	}

	bool uses_g(){
		return use_g_hash;
	}
};

void help(void){
	std::cout << "./fm.exe [--help/--hard] n_events n_distinct_events interval_nfm error_rate_djkst " << std::endl;
	std::cout << ".Doc:" << std::endl;
	std::cout << "--help (optional): Show help guide (this guide)." << std::endl;
	std::cout << "--hard (optional): Do a hardwork test for SAME_STREAM and show the probs." << std::endl;
	std::cout << "n_events (required): Number of events to stream computes." << std::endl;
	std::cout << "n_distinct_events (required): Number of distinct numbers on the stream (0 <= n_distinct_events <= n_events)." << std::endl;
	std::cout << "interval_nfm (required) : Interval for Non Idealized FM just for probability test." << std::endl;
	std::cout << "error_rate_djkst (required): Error rate used in Bjkst algorithm." << std::endl;
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

	if(fm.uses_g()){
		std::cout << "Value predicted with g hash := ";
	}else{
		std::cout << "Value predicted without g hash := ";
	}

	std::cout << fm.query() << std::endl;

	std::cout << "Error rate used := " << fm.error_rate() << std::endl;
	std::cout << "Fail prob := " << fm.delta()*100.0 << "%" <<  std::endl;

	if(fm.uses_g()){
		std::cout << "Diff Values with g := ";
	}else{
		std::cout << "Diff Values without g := ";
	}

	std::cout << std::max((ll)n_distinct_events,fm.query()) - std::min((ll)n_distinct_events,fm.query()) << std::endl;

	std::cout << "How many buckets bust := " << fm.busted() << std::endl;
	std::cout << "Should be write up to := " << fm.max_store() << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;


	ll old_diff_g = diff_max_with_g;
	ll old_diff_wt_g = diff_max_without_g;

	if(fm.uses_g()){
		diff_max_with_g = std::max(diff_max_with_g,(std::max((ll)n_distinct_events,fm.query()) - std::min((ll)n_distinct_events,fm.query())));	
	}else{
		diff_max_without_g = std::max(diff_max_without_g,(std::max((ll)n_distinct_events,fm.query()) - std::min((ll)n_distinct_events,fm.query())));	
	}

	if (old_diff_g != diff_max_with_g or old_diff_wt_g != diff_max_without_g){
		fm.final_state();
	}
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

int testBinaryHash(){
	using namespace std;
	ll x = 8;
	vector<unsigned short> result = binaryExpansion(x,5);
	for(int i = 0 ; i < result.size(); ++i){ 
		cout << result[i]  << " ";
	}
	cout << endl;

	cout << binaryToDecimal(result) << endl;
	//
	binary_two_wise_family h = binary_two_wise_family(3, 2);
	for(int i = 0 ; i < 8; ++i){
		cout << "i: "<< i << " hash: " << h.hash(i) << endl;
	}

	//
	return 0;
}

std::vector<ll> generate_with_binomial_distribution(ll t, double prob,ll numbers_of_elements){
	std::binomial_distribution<ll> gen(t,prob);
	std::vector<ll> v;

	for(ll i=0;i<numbers_of_elements;i++){
		v.push_back(gen(generator));
		v[i]++;
	}

	return v;
}

int distinct_in(std::vector<ll>& v){
	std::map<ll,int> mp;
	int count = 0;

	for(auto&it:v){
		if(!mp[it]){
			count++;
			mp[it] = 1;
		}
	}

	return count;
}


int main2(){
	std::vector<ll> v = generate_with_binomial_distribution(9,0.9,20);

	for(auto &it:v){
		std::cout << it << " ";
	}

	std::cout << std::endl;

	std::cout << distinct_in(v)  << std::endl;
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

		double fm_times_wrong_med = 0.0;
		double bjkst_times_wrong_med = 0.0;
		double bjkst_times_wrong_med_with_g = 0.0;
		ll fm_average_value = 0ULL;
		ll bjkst_average_value = 0ULL;
		ll bjkst_average_value_with_g = 0ULL; 

		for(int j=0;j<SAME_STREAM;j++){

			non_idealized_fm fm(numbers_on_stream.size());
			bjkst bjk(numbers_on_stream.size(),std::stod(args[3]));
			bjkst bjk2(numbers_on_stream.size(),std::stod(args[3]),true);
			bool fm_wrong_on_epoch = false;
			bool bjkst_wrong_on_epoch = false;
			bool bjkst_wrong_on_epoch_with_g = false;


			std::cout << "On epoch (" << j + 1 << ") : " << std::endl;


			for(int i=0;i<numbers_on_stream.size();i++){
				fm.update(numbers_on_stream[i]);
				bjk.update(numbers_on_stream[i]);
				bjk2.update(numbers_on_stream[i]);
			}

			if((fm.query()*1.0 < (distinct*1.0/interval_nfm*1.0)) or (fm.query()*1.0 > (interval_nfm*distinct*1.0))){
				fm_wrong_on_epoch = true;
			}

			if(std::fabs((bjk.query()*1.0) - (distinct*1.0)) > (bjk.error_rate() * (distinct*1.0))){
				bjkst_wrong_on_epoch = true;
			}

			if(std::fabs((bjk2.query()*1.0) - (distinct*1.0)) > (bjk2.error_rate() * (distinct*1.0))){
				bjkst_wrong_on_epoch_with_g = true;
			}

			std::cout << "fm wrong on epoch := " << fm_wrong_on_epoch << std::endl;
			std::cout << "bjkst wrong on epoch without g hash := " << bjkst_wrong_on_epoch << std::endl;
			std::cout << "bjkst wrong on epoch with g  hash := " << bjkst_wrong_on_epoch_with_g << std::endl;

			fm_times_wrong_med += fm_wrong_on_epoch;
			bjkst_times_wrong_med += bjkst_wrong_on_epoch;
			bjkst_times_wrong_med_with_g += bjkst_wrong_on_epoch_with_g;

			fm_average_value += fm.query();
			bjkst_average_value += bjk.query();
			bjkst_average_value_with_g += bjk2.query();

			std::cout << std::endl << std::endl;

			print_fm_summary(distinct,interval_nfm,fm);
			std::cout << std::endl;
			print_bjkst_summary(distinct,bjk);
			std::cout << std::endl;
			print_bjkst_summary(distinct,bjk2);
			std::cout << std::endl << std::endl;
		}

		std::cout << "Summary:" << std::endl;

		std::cout << "fm average output := " << fm_average_value*1.0/SAME_STREAM << std::endl;
		std::cout << "fm average for errors := " << (fm_times_wrong_med*100.0/(SAME_STREAM)) << "%" << std::endl;
		std::cout << std::endl;
		std::cout << "bjkst average output := " << bjkst_average_value*1.0/SAME_STREAM << std::endl;
		std::cout << std::endl;
		std::cout << "Times wrong without g hash := " << bjkst_times_wrong_med << std::endl;
		std::cout << "bjkst average for errors without g hash := " << (bjkst_times_wrong_med*100.0/(SAME_STREAM)) << "%" << std::endl;
		std::cout << "Max diff on all runs without g hash := " << diff_max_without_g << std::endl;
		std::cout << std::endl;
		std::cout << "Times wrong with g hash := " << bjkst_times_wrong_med_with_g << std::endl;
		std::cout << "bjkst average for errors with g hash := " << ((bjkst_times_wrong_med_with_g*100.0)/(SAME_STREAM)) << "%" << std::endl;
		std::cout << "Max diff on all runs with g hash := " << diff_max_with_g << std::endl;
		std::cout << std::endl;

	}else{
		int interval_nfm = std::stoi(args[2]);
		non_idealized_fm fm(numbers_on_stream.size());
		bjkst bjk(numbers_on_stream.size(),std::stod(args[3]));


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