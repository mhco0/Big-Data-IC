#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <random>

class morris{
/*
	Fix probability error:

	P[|n-ñ| > e*n] < 1/(2*e^2)
*/
private:
	uint64_t counter;

	bool can_update(){
		std::default_random_engine generator;
 		std::uniform_real_distribution<double> distribution(0.0,1.0);
 		double number = distribution(generator);

		return (number < (1.0/(1U << counter)));
	}

public:
	morris(){
		counter = 0U;
	}

	void update(void){
		counter = counter + can_update();
	}

	uint64_t query(void){
		return ((1U << counter) - 1);
	}
};

class morris_p{
/*
	Running s copys of Morris algorithm and averaging their output give us:
	- P[|n-ñ| > e*n] < delta
	- P[|n-ñ| > e*n] < 1/(2*s*e^2)
	So, s > 1/(2*delta*e^2)
*/
private:
	uint64_t s;
	double error;
	double delt;
	std::vector<morris> counters;
public:

	morris_p(double error_rate = 0.33,double dlt = 0.33){
		int copys = 1;

		error = error_rate;
		delt = dlt;
		copys = (int)(1.0/(2*delt*(error*error)))+1;
		s = copys;
		counters.resize(copys,morris());
	}

	~morris_p(){
	}

	double error_rate(void){
		return error;
	}

	double delta(void){
		return delt;
	}

	uint32_t copys_number(void){
		return s;
	}

	void update(void){
		for(int i = 0; i < this->copys_number() ; i++){
			counters[i].update();
		}
	}

	uint64_t query(void){
		uint64_t sum = 0;
		for(int i = 0; i < this->copys_number() ; i++){
			sum += counters[i].query();
		}

		return sum/this->copys_number();
	}
};

class morris_pp{
/*
	Running t instantiations of Morris+ with delta = 1/3(delta for Morris+, not Morris++). 
	Output the median value:
	- t = omega(lg(1/delta))
*/
private:
	std::vector<morris_p> counters;
	uint64_t t;
	double delt;
public:
	morris_pp(double deltt){
		delt = deltt;
		t = (int)std::ceil(std::log2(1.0/delt));

		counters.resize(t,morris_p(0.33,delt));
	}

	~morris_pp(){
	}

	uint64_t copys_number(void){
		return t;
	}

	double delta(){
		return delt;
	}

	void update(){
		for(int i = 0; i < this->copys_number() ; i++){
			counters[i].update();
		}
	}

	uint64_t query(void){
		std::sort(counters.begin(),counters.end(),[](morris_p& a,morris_p& b){
			return a.query() < b.query();
		});

		return counters[this->copys_number()/2].query();
	}
};


void help(void){
	std::cout << "./morris.exe [--help/--no-wait] n_events error_rate_morris_p delta_morris_p delta_morris_pp morris_relative" << std::endl;
	std::cout << ".Doc:" << std::endl;
	std::cout << "--help (optional): Show help guide (this guide)." << std::endl;
	std::cout << "--no-wait (optional): Jumps to conclusive results, stop the wait for input." << std::endl;
	std::cout << "n_events (required): Number of events to stream computes." << std::endl;
	std::cout << "error_rate_morris_p (required): Error rate for morris_p algorithm test." << std::endl;
	std::cout << "delta_morris_p (required): Delta value for morris_p algorithm test." << std::endl;
	std::cout << "delta_morris_pp (required): Fail propability used in morris_pp algorithm." << std::endl;
	std::cout << "morris_relative (required): E-risk expected for morris algorithm.(>~ 70%)" << std::endl;
}
	
void benchmark(int event_number,morris& test,morris_p& test2, morris_pp& test3,double morris_relative = 0.70){
	static int fail_morris = 0;
	static int fail_morris_p = 0;
	static int fail_morris_pp = 0;
	int diff_morris = (int)abs(test.query() - event_number);
	int diff_morris_p = (int)abs(test2.query() - event_number);
	int diff_morris_pp = (int)abs(test3.query() - event_number);

	if(diff_morris > morris_relative*event_number) fail_morris++;
	if(diff_morris_p > test2.error_rate()*event_number) fail_morris_p++;
	if(diff_morris_pp > 0.33*event_number) fail_morris_pp++;

	if (event_number%1000 == 0){
		std::cout << "--------------------BENCHMARK------------------" << std::endl;
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "True value := " << event_number << std::endl;
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Using Morris Algorithm: " << std::endl;
		std::cout << "Value predicted := " << test.query() << std::endl;
		std::cout << "Diff Between expected value and true value := " << diff_morris << std::endl;
		std::cout << "Fail count := " << fail_morris << std::endl;
		std::cout << "Fail prob := "<< morris_relative << std::endl;
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Using Morris Plus Algorithm(" << test2.copys_number() << " copys): " << std::endl;
		std::cout << "Value predicted := " << test2.query() << std::endl;
		std::cout << "Diff Between expected value and true value := " << diff_morris_p << std::endl;
		std::cout << "Fail count := " << fail_morris_p << std::endl;
		std::cout << "Fail prob := "<< test2.delta() << std::endl;
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Using Morris Plus Plus Algorithm(" << test3.copys_number() << " copys): " << std::endl;
		std::cout << "Value predicted := " << test3.query() << std::endl;
		std::cout << "Diff Between expected value and true value := " << diff_morris_pp << std::endl;
		std::cout << "Fail count := " << fail_morris_pp << std::endl;
		std::cout << "Fail prob := "<< test3.delta() << std::endl;
		std::cout << "-----------------------------------------------" << std::endl;
	}

}

int main(int argc,char* argv[]){
	std::deque<std::string> args;
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);
	bool wait_input = true;

	for(int i=1;i<argc;i++){
		std::string s(argv[i]);

		args.push_back(s);
	}

	if(args[0] == "--help"){
		help();
		return 0;
	}

	if(args[0] == "--no-wait"){
		wait_input = false;
		args.pop_front();
	}

	if(args.size() != 5){
		std::cout << "Invalid parameters, try --help" << std::endl;
		return 0;
	}

	const int events = std::stoi(args[0]);
	morris test;
	morris_p test2(std::stod(args[1]),std::stod(args[2]));
	morris_pp test3(std::stod(args[3]));

	for(int i=1;i<=events;i++){
		//std::cout << std::flush;
		//system("cls");
		test.update();
		test2.update();
		test3.update();

		benchmark(i,test,test2,test3,std::stod(args[4]));

		if(wait_input){
			system("pause");
		}
	}

	return 0;
}