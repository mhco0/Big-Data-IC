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
	- t = teta(lg(1/delta))
*/
private:
	std::vector<morris_p> counters;
	uint64_t t;
	double delt;
	double err;
public:
	morris_pp(double er, double deltt){
		const int c = 36;
		// The constant that we found by using Chernoff bound
		delt = deltt;
		err = er;
		t = (int)std::round(c*std::log(1.0/delt));

		counters.resize(t,morris_p(err,0.33));
	}

	~morris_pp(){
	}

	uint64_t copys_number(void){
		return t;
	}

	double delta(){
		return delt;
	}

	double error_rate(){
		return err;
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
	std::cout << "./morris.exe [--help/--no-wait/--no-wait-no-error-prob] n_events error_rate_morris_p delta_morris_p error_rate_morris_pp delta_morris_pp morris_relative" << std::endl;
	std::cout << ".Doc:" << std::endl;
	std::cout << "--help (optional): Show help guide (this guide)." << std::endl;
	std::cout << "--no-wait (optional): Jumps to conclusive results (with error prob), stop the wait for input." << std::endl;
	std::cout << "--no-wait-no-error-prob (optional): Jumps to conclusive results (without error prob), stop the wait for input." << std::endl;
	std::cout << "n_events (required): Number of events to stream computes." << std::endl;
	std::cout << "error_rate_morris_p (required): Error rate for morris_p algorithm test." << std::endl;
	std::cout << "delta_morris_p (required): Fail probability used in morris_p algorithm test." << std::endl;
	std::cout << "error_rate_morris_pp (required): Error rate for morris_pp algorithm." << std::endl;
	std::cout << "delta_morris_pp (required): Fail propability used in morris_pp algorithm." << std::endl;
	std::cout << "morris_relative (required): E-risk expected for morris algorithm.(>~ 70%)" << std::endl;
}

void print_error_prob(std::vector<int> error_score[3],int events,int times_running){
	std::cout << "Printing fails for Morris Algorithm:" << std::endl;
	for(int i=0;i<events;i++){
		std::cout << (i+1) <<" : " << error_score[0][i]*100/(double)times_running << "%" << std::endl;
	}
	std::cout << "Printing fails for Morris Plus Algorithm:" << std::endl;
	for(int i=0;i<events;i++){
		std::cout << (i+1) <<" : " << error_score[1][i]*100/(double)times_running << "%" << std::endl;
	}
	std::cout << "Printing fails for Morris Plus Plus Algorithm:" << std::endl;
	for(int i=0;i<events;i++){
		std::cout << (i+1) <<" : " << error_score[2][i]*100/(double)times_running << "%" << std::endl;
	}
}

void print_summary(int local_event,morris& test,morris_p& test2,morris_pp& test3,double morris_relative,std::vector<int> event_fail_counts[3]){
	std::cout << "--------------------BENCHMARK------------------" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "True value := " << local_event << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Using Morris Algorithm: " << std::endl;
	std::cout << "Value predicted := " << test.query() << std::endl;
	std::cout << "Fail count for event {"<< local_event <<"} := " << event_fail_counts[0][local_event-1] << std::endl;
	std::cout << "Fail prob := "<< morris_relative << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Using Morris Plus Algorithm(" << test2.copys_number() << " copys): " << std::endl;
	std::cout << "Value predicted := " << test2.query() << std::endl;
	std::cout << "Fail count for event {"<< local_event <<"} := " << event_fail_counts[1][local_event-1] << std::endl;
	std::cout << "Fail prob := "<< test2.delta() << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Using Morris Plus Plus Algorithm(" << test3.copys_number() << " copys): " << std::endl;
	std::cout << "Value predicted := " << test3.query() << std::endl;
	std::cout << "Fail count for event {"<< local_event <<"} := " << event_fail_counts[2][local_event-1] << std::endl;
	std::cout << "Fail prob := "<< test3.delta() << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
}
	
void benchmark(int event_number,morris& test,morris_p& test2, morris_pp& test3,double morris_relative,bool print_relative,std::vector<int> event_fail_counts[3]){
	for(int i=1;i<=event_number;i++){
		test.update();
		test2.update();
		test3.update();

		double diff_morris = abs(test.query() - i);
		double diff_morris_p = abs(test2.query() - i);
		double diff_morris_pp = abs(test3.query() - i);

		if(diff_morris > morris_relative*i){
			event_fail_counts[0][i-1]++;
		}

		if(diff_morris_p > test2.error_rate()*i){
			event_fail_counts[1][i-1]++;
		}

		if(diff_morris_pp > test3.error_rate()*i){
			event_fail_counts[2][i-1]++;
		}

		if (print_relative){
			print_summary(i,test,test2,test3,morris_relative,event_fail_counts);
		}
	}
}

int main(int argc,char* argv[]){
	std::vector<int> error_score[3];
	std::deque<std::string> args;
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);
	bool wait_input = true;
	bool see_error_prob = true;

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

	if(args[0] == "--no-wait-no-error-prob"){
		wait_input = false;
		see_error_prob = false;
		args.pop_front();
	}

	if(args.size() != 6){
		std::cout << "Invalid parameters, try --help" << std::endl;
		return 0;
	}

	const int events = std::stoi(args[0]);
	
	for(int i=0;i<3;i++){
		error_score[i].resize(events,0);
	}

	for(int i=0;i<100;i++){
		//std::cout << std::flush;
		//system("cls");
		morris test;
		morris_p test2(std::stod(args[1]),std::stod(args[2]));
		morris_pp test3(std::stod(args[3]),std::stod(args[4]));

		benchmark(events,test,test2,test3,std::stod(args[5]),wait_input,error_score);
		//make a vector to run this N times and take the median hoping that (P[Ni fails] <= delta)

		if(wait_input){
			system("pause");
		}

		if (i == 99){
			print_summary(events,test,test2,test3,std::stod(args[5]),error_score);
			std::cout << std::endl;
		}
	}

	if(see_error_prob) print_error_prob(error_score,events,100);

	return 0;
}