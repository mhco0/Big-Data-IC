#include "commum_header.h"
#include "global_generator.h"
#include "k_wise_family.h"
#include "binary_two_wise_family.h"
#define SAME_STREAM 1000
using namespace std;
using ll = long long int;

/*
	Pick a function from a 4-wise independent family then pick the last random bit and returns {1,-1}
*/
int last_random_bit(function<ll(ll)>& hash_function,ll elem){
	ll hash_response = hash_function(elem);

	if(hash_response & 1) return 1;
	else return -1;
}

class tug_of_war{
private:
	ll counter;
	function<int(ll)> four_wise_hash;
public:

	tug_of_war(function<int(ll)> fn){
		this->counter = 0;
		this->four_wise_hash = fn;
	}

	~tug_of_war(){
	}
	
	void update(ll elem,ll elem_change){
		counter += elem_change*four_wise_hash(elem);
	}

	ll query(){
		return counter*counter;
	}
};


class tug_of_war_p{
private:
	vector<tug_of_war> estimators;
	int t_copys;
	double error;
	double delta;
public:

	tug_of_war_p(double error, double delta,ll universe){
		this->error = error;
		this->delta = delta;

		this->t_copys = (int) (2.0/(delta*error*error)) + 1;

		for(int i=0;i<t_copys;i++){
			k_wise_family f(4,universe);

			function<ll(ll)> four_wise_function = bind<ll>(&k_wise_family::operator(),f,placeholders::_1);
			auto hash_function = bind<int>(last_random_bit,four_wise_function,placeholders::_1);
			tug_of_war tow(hash_function);

			estimators.push_back(tow);
		}
	}

	~tug_of_war_p(){
	}


	void update(ll elem, ll elem_change){
		for(int i=0;i<t_copys;i++){
			estimators[i].update(elem,elem_change);
		}
	}

	ll query(){
		ll parse_sum = 0LL;

		for(int i=0;i<t_copys;i++){
			parse_sum += estimators[i].query();
		}

		return parse_sum/t_copys;
	}

	int how_many_estimators(){
		return t_copys;
	}
};


class tug_of_war_pp{
private:
	vector<tug_of_war_p> estimators;
	int t_copys;
	double error;
	double delta;
	const int t_constant = 36; // for delta = 33% int Tug of War Plus 
public:
	tug_of_war_pp(double error, double delta,ll universe){
		this->error = error;
		this->delta = delta;

		this->t_copys = (int) t_constant*log(1.0/delta) + 1;

		for(int i=0;i<t_copys;i++){
			tug_of_war_p towp(error,0.33,universe);

			estimators.push_back(towp);
		}
	}

	~tug_of_war_pp(){
	}

	void update(ll elem, ll elem_change){
		for(int i=0;i<t_copys;i++){
			estimators[i].update(elem,elem_change);
		}
	}

	ll query(){
		std::sort(estimators.begin(),estimators.end(),[](tug_of_war_p& a,tug_of_war_p& b){
			return a.query() < b.query();
		});

		return estimators[t_copys/2].query();
	}
	
	int how_many_estimators(){
		return t_copys;
	}

	ll how_many_estimators_from_twop(){
		ll parse_sum = 0LL;

		for(int i=0;i<t_copys;i++){
			parse_sum += estimators[i].how_many_estimators();
		}

		return parse_sum;
	}
};


/*
	Calculate the next power of 2 closer to x
*/
ll nextp2(ll x){
	return (1ULL << (int)std::ceil(std::log2(x)));
}


/*
	Return the F2 moment for some stream.
*/
ll calc_F2_moment(vector<ll> &stream){
	ll real_f2_moment = 0LL;

	for(auto&it:stream){
		real_f2_moment += it*it;
	}

	return real_f2_moment;
}

ll calc_F4_moment(vector<ll> &stream){
	ll real_f4_moment = 0LL;

	for(auto&it:stream){
		real_f4_moment += it*it*it*it;
	}

	return real_f4_moment;
}

/*
	Make a stream vector with vector_size elements, using add operations and sub operations as passed in the args
*/
pair<vector<ll>,vector<pair<ll,ll>>> make_stream_and_commands(ll vector_size, int number_of_add_operations, int number_of_sub_operations,int random_add = 1000){
	uniform_int_distribution<ll> who_pick(0LL,1LL);
	uniform_int_distribution<ll> how_many_give(0LL, 1LL);
	vector<ll> stream(vector_size,0);
	vector<pair<ll,ll>> commands_to_stream;

	for(int i=0;i<number_of_add_operations;i++){
		ll random_element = who_pick(generator);
		
		ll delta_to_random_element = how_many_give(generator);

		stream[random_element] += delta_to_random_element;

		commands_to_stream.push_back({random_element,delta_to_random_element});
	}

	for(int i=0;i<number_of_sub_operations;i++){
		ll random_element = who_pick(generator);
		ll delta_to_random_element = max(how_many_give(generator),stream[random_element]);

		stream[random_element] -= delta_to_random_element;

		commands_to_stream.push_back({random_element,-delta_to_random_element});
	}

	return {stream, commands_to_stream};
}

/*
	Test for Tug of War Plus Plus algorithm.
*/
void test_towpp(double error_expected, double delta_expected, ll vector_size, int number_of_add_operations, int number_of_sub_operations,int same_stream){
	auto stream_and_commands = make_stream_and_commands(vector_size,number_of_add_operations,number_of_sub_operations);
	vector<ll> stream = stream_and_commands.first;
	vector<pair<ll,ll>> commands = stream_and_commands.second;

	int estimators_used = 0LL;
	ll estimators_from_towp = 0LL;
	ll real_f2_moment = calc_F2_moment(stream);
	ll average_error = 0LL;
	ll average_value = 0LL;

	for(int i=0;i<same_stream;i++){
		tug_of_war_pp towpp(error_expected,delta_expected,stream.size());

		estimators_used = towpp.how_many_estimators();
		estimators_from_towp = towpp.how_many_estimators_from_twop();

		bool fail = false;

		for(auto&command: commands){
			towpp.update(command.first,command.second);
		}

		if(fabs((towpp.query()*1.0) - real_f2_moment) > error_expected*real_f2_moment){
			fail = true;
		}

		average_error += fail;
		average_value += towpp.query();
	}

	cout << fixed << setprecision(2);
	cout << "Results for Tug of War Plus: " << endl;
	cout << "True value : " << real_f2_moment << endl;
	cout << "Average F2 estimated : " << (average_value*1.0)/(same_stream*1.0) << endl;
	cout << "How many estimators : " << estimators_used << endl;
	cout << "How many estimators from Tug of War Plus : " << estimators_from_towp << endl;
	cout << "Times wrong on "<< same_stream << " tests : " << average_error << endl;
	cout << "Average of errors : " << (average_error*100.0)/(same_stream*1.0) << "%" << endl;
	cout << "Error for the test :" << error_expected << endl;
	cout << "Fail prob expected : " << delta_expected*100.0 << "%" << endl;
}

/*
	Test for Tug of War Plus algorithm.
*/
void test_towp(double error_expected, double delta_expected, ll vector_size, int number_of_add_operations, int number_of_sub_operations,int same_stream){
	auto stream_and_commands = make_stream_and_commands(vector_size,number_of_add_operations,number_of_sub_operations);
	vector<ll> stream = stream_and_commands.first;
	vector<pair<ll,ll>> commands = stream_and_commands.second;

	int estimators_used = 0LL;
	ll real_f2_moment = calc_F2_moment(stream);
	ll average_error = 0LL;
	ll average_value = 0LL;

	for(int i=0;i<same_stream;i++){
		tug_of_war_p towp(error_expected,delta_expected,stream.size());

		estimators_used = towp.how_many_estimators();

		bool fail = false;

		for(auto&command: commands){
			towp.update(command.first,command.second);
		}

		if(fabs((towp.query()*1.0) - real_f2_moment) > error_expected*real_f2_moment){
			fail = true;
		}

		average_error += fail;
		average_value += towp.query();
	}

	cout << fixed << setprecision(2);
	cout << "Results for Tug of War Plus: " << endl;
	cout << "True value : " << real_f2_moment << endl;
	cout << "Average F2 estimated : " << (average_value*1.0)/(same_stream*1.0) << endl;
	cout << "How many estimators : " << estimators_used << endl;
	cout << "Times wrong on "<< same_stream << " tests : " << average_error << endl;
	cout << "Average of errors : " << (average_error*100.0)/(same_stream*1.0) << "%" << endl;
	cout << "Error for the test :" << error_expected << endl;
	cout << "Fail prob expected : " << delta_expected*100.0 << "%" << endl;
}

/*
	Test for the Tug of War algorithm
*/
void test_tow(double error_expected,ll vector_size, int number_of_add_operations, int number_of_sub_operations, int same_stream){
	auto stream_and_commands = make_stream_and_commands(vector_size,number_of_add_operations,number_of_sub_operations);
	vector<ll> stream = stream_and_commands.first;
	vector<pair<ll,ll>> commands = stream_and_commands.second;

	ll real_f2_moment = calc_F2_moment(stream);
	ll real_f4_moment = calc_F4_moment(stream);
	ll average_error = 0LL;
	ll average_value = 0LL;


	for(int i=0;i<same_stream;i++){
		k_wise_family f(4,stream.size());

		function<ll(ll)> function_from_four_wise = bind<ll>(&k_wise_family::operator(),f,placeholders::_1);
		auto hash_function = bind<int>(last_random_bit,function_from_four_wise,placeholders::_1);
		tug_of_war tow(hash_function);

		bool fail = false;
		for(auto&command : commands){
			tow.update(command.first,command.second);
		}

		if(fabs((tow.query()*1.0) - real_f2_moment) > error_expected*real_f2_moment){
			fail = true;
		}

		average_error += fail;
		average_value += tow.query();
	}

	cout << fixed << setprecision(2);
	cout << "Results for Tug of War: " << endl;
	cout << "True F2 : " << real_f2_moment << endl;
	cout << "Average F2 estimated : " << (average_value*1.0)/(same_stream*1.0) << endl;
	cout << "Times wrong on "<< same_stream << " tests : " << average_error << endl;
	cout << "Average of errors : " << (average_error*100.0)/(same_stream*1.0) << "%" << endl;
	cout << "Error for the test :" << error_expected << endl;
	cout << "Fail prob expected : " << (200.0)/(error_expected*error_expected*1.0) << "%" << endl;
	cout << "Real prob expected : " << (100.0*((2.0/(error_expected*error_expected)) - ((2.0*real_f4_moment)/(error_expected*error_expected*real_f2_moment*real_f2_moment*1.0)))) << "%" << endl;	
}

/*
	Test if the make_stream_and_commands is working.
*/
void test_stream_creation(ll vector_size, int number_of_add_operations, int number_of_sub_operations,int random_add){
	auto stream_and_commands = make_stream_and_commands(vector_size,number_of_sub_operations,number_of_sub_operations,random_add);

	for(auto &it:stream_and_commands.first){
		cout << it << " ";
	}

	cout << endl;

	for(auto &it:stream_and_commands.second){
		cout << "(" << it.first << "," << it.second << ")" << " ";
	}

	cout << endl;
}

/*
	Test if the hash_family is 4-wise indepent.
*/
bool test_if_family_is_4_wise(ll universe){
	bool is_four_wise = true;
	ll contra_domain = nextPrime(universe);
	vector<unsigned long long int> constants = {0,0,0,0};
	vector<vector<vector<vector<vector<vector<vector<vector<ll>>>>>>>> counter;

	counter.resize(contra_domain);
	for(int i=0;i<contra_domain;i++){
		counter[i].resize(contra_domain);
		for(int j=0;j<contra_domain;j++){
			counter[i][j].resize(contra_domain);
			for(int k=0;k<contra_domain;k++){
				counter[i][j][k].resize(contra_domain);
				for(int i2=0;i2<contra_domain;i2++){
					counter[i][j][k][i2].resize(contra_domain);
					for(int j2=0;j2<contra_domain;j2++){
						counter[i][j][k][i2][j2].resize(contra_domain);
						for(int k2=0;k2<contra_domain;k2++){
							counter[i][j][k][i2][j2][k2].resize(contra_domain);
							for(int i3=0;i3<contra_domain;i3++){
								counter[i][j][k][i2][j2][k2][i3].assign(contra_domain,0);
							}
						}
					}
				}
			}
		}
	}

	for(int i=0;i<contra_domain;i++){
		for(int j=0;j<contra_domain;j++){
			for(int k=0;k<contra_domain;k++){
				for(int l=0;l<contra_domain;l++){
					constants[0] = i;
					constants[1] = j;
					constants[2] = k;
					constants[3] = l;

					k_wise_family f(4,universe,constants);

					for(int x1=0;x1<contra_domain;x1++){
						for(int x2=0;x2<contra_domain;x2++){
							for(int x3=0;x3<contra_domain;x3++){
								for(int x4=0;x4<contra_domain;x4++){
									counter[x1][x2][x3][x4][f(x1)][f(x2)][f(x3)][f(x4)]++;
								}
							}
						}
					}
				}
			}
		}
	}

	assert(counter.size() > 4);
	ll frequency = counter[0][1][2][3][0][1][2][3];

	for(int x1=0;x1<contra_domain;x1++){
		for(int x2=0;x2<contra_domain;x2++){
			for(int x3=0;x3<contra_domain;x3++){
				for(int x4=0;x4<contra_domain;x4++){
					for(int f1=0;f1<contra_domain;f1++){
						for(int f2=0;f2<contra_domain;f2++){
							for(int f3=0;f3<contra_domain;f3++){
								for(int f4=0;f4<contra_domain;f4++){
									cout << counter[x1][x2][x3][x4][f1][f2][f3][f4] << " ";

									bool xs_eq = x1 == x2 or x1 == x3 or x1 == x4;
									xs_eq = xs_eq or (x2 == x3 or x2 == x4);
									xs_eq = xs_eq or (x3 == x4);
									if(not xs_eq and frequency != counter[x1][x2][x3][x4][f1][f2][f3][f4]) is_four_wise = false;
								}
							}
						}
					}
					cout << endl;
				}
			}
		}
	}
	cout << endl;

	ll contra_domain_p4 = contra_domain*contra_domain*contra_domain*contra_domain;

	if(fabs((frequency/(contra_domain_p4*1.0))-(1/(contra_domain_p4*1.0))) > 1e-6) is_four_wise = false;

	return is_four_wise;
}

/*
	Test if the k_wise_family is 2-wise independent when the k = 2, when the hash is from [nextPrime(universe)] -> [nextPrime(universe)].
*/
bool test_if_family_is_2_wise(ll universe){
	bool is_two_wise = true;
	ll contra_domain = nextPrime(universe);
	vector<unsigned long long int> constants = {0,0};
	vector<vector<vector<vector<ll>>>> counter;

	counter.resize(contra_domain);
	for(int i=0;i<contra_domain;i++){
		counter[i].resize(contra_domain);
		for(int j=0;j<contra_domain;j++){
			counter[i][j].resize(contra_domain);
			for(int k=0;k<contra_domain;k++){
				counter[i][j][k].assign(contra_domain,0);
			}
		}
	}

	for(int i=0;i<contra_domain;i++){
		for(int j=0;j<contra_domain;j++){
			constants[0] = i;
			constants[1] = j;

			k_wise_family f(2,universe,constants);

			for(int x1=0;x1<contra_domain;x1++){
				for(int x2=0;x2<contra_domain;x2++){
					counter[x1][x2][f(x1)][f(x2)]++;
				}
			}
		}
	}

	ll frequency = counter[0][1][0][1];

	cout << "x/f(x): ";
	for(int f1=0;f1<contra_domain;f1++){
		for(int f2=0;f2<contra_domain;f2++){
			cout << "(" << (f1) << "," << (f2) << ")\t"; 
		}
	}
	cout << endl;

	for(int x1=0;x1<contra_domain;x1++){
		for(int x2=0;x2<contra_domain;x2++){
			cout << "(" << (x1) << "," << (x2) << ")\t  ";
			for(int f1=0;f1<contra_domain;f1++){
				for(int f2=0;f2<contra_domain;f2++){
					cout << counter[x1][x2][f1][f2] << "\t  ";

					if(x1 != x2 and frequency != counter[x1][x2][f1][f2]) is_two_wise = false;
				}
			}
			cout << endl;
		}
	}
	cout << endl;

	if(fabs((frequency/(contra_domain*contra_domain*1.0))-(1/(contra_domain*contra_domain*1.0))) > 1e-6) is_two_wise = false;

	return is_two_wise;
}

/*
	Test if the values for the k_wise_family is equaly distributed for k = 2, when the hash is from [nextPrime(universe)] -> [nextPrime(universe)].
*/
bool test_2_wise_family_uniform_dist(ll universe){
	bool is_uniform_dist = true;
	ll contra_domain = nextPrime(universe);
	vector<unsigned long long int> constants = {0,0};
	vector<vector<ll>> counter;

	counter.resize(contra_domain);
	for(int i=0;i<contra_domain;i++){
		counter[i].assign(contra_domain,0);
	}

	for(int i=0;i<contra_domain;i++){
		for(int j=0;j<contra_domain;j++){
			constants[0] = i;
			constants[1] = j;

			k_wise_family f(2,universe,constants);

			for(int x=0;x<contra_domain;x++){
				counter[x][f(x)]++;
			}
		}
	}

	ll frequency = counter[0][0];
	for(int i=0;i<contra_domain;i++){
		for(int j=0;j<contra_domain;j++){
			cout << counter[i][j] << "\t";
			if(counter[i][j] != frequency){
				is_uniform_dist = false;
			}
		}
		cout << endl;
	}

	cout << endl;

	if(fabs((frequency/(contra_domain*contra_domain*1.0))-(1/(contra_domain*1.0))) > 1e-6) is_uniform_dist = false;

	return is_uniform_dist;
}

/*
	Test if the bind functions works for the test.
*/
void test_bind_functions(int range_test_values){
	k_wise_family f(4,range_test_values);
	function<ll(ll)> four_wise_function = bind<ll>(&k_wise_family::operator(),f,placeholders::_1);
	auto hash_function = bind<int>(last_random_bit,four_wise_function,placeholders::_1);


	for(int i=1;i<range_test_values;i++){
		cout << hash_function(i) << endl;
	}
}

/*
	Show the help guide.
*/
void help(void){
	cout << "tug_of_war.exe [--help/--tow/--towp/--towpp] [args...]" << endl;
	cout << endl;
	cout << "1o arg (required): " << endl;
	cout << "=> --help: Show this guide." << endl;
	cout << "=> --tow: Run the Tug of War algorithm using [args...]" << endl;
	cout << "=> --towp: Run the Tug of War algorithm with the average trick using [args...]" << endl;
	cout << "=> --towpp: Run the Tug of War algorithm with the median trick using [args...]" << endl;
	cout << endl;
	cout << "2o arg (required): " << endl;
	cout << "=> [--help] (optional) : No argument is required." << endl;
	cout << endl;
	cout << "=> [--tow] epson vector_size num_add_op num_sub_op : " << endl;
	cout << "1. epson (required) : epson error used to the algorithm analysis." << endl;
	cout << "2. vector_size (required) : vector size used to find F2." << endl;
	cout << "3. num_add_op (required) : number of add operations used in the vector." << endl;
	cout << "4. num_sub_op (required) : number of sub operations used in the vector." << endl;
	cout << endl;
	cout << "=> [--towp] epson delta vector_size num_add_op num_sub_op : " << endl;
	cout << "1. epson (required) : epson error used in Tug of War Plus." << endl;
	cout << "2. delta (required) : delta fail prob used in Tug of War Plus." << endl;
	cout << "3. vector_size (required) : vector size used to find F2." << endl;
	cout << "4. num_add_op (required) : number of add operations used in the vector." << endl;
	cout << "5. num_sub_op (required) : number of sub operations used in the vector." << endl;
	cout << endl;
	cout << "=> [--towpp] epson delta vector_size num_add_op num_sub_op : " << endl;
	cout << "1. epson (required) : epson error used in Tug of War Plus Plus." << endl;
	cout << "2. delta (required) : delta fail prob used in Tug of War Plus Plus." << endl;
	cout << "3. vector_size (required) : vector size used to find F2." << endl;
	cout << "4. num_add_op (required) : number of add operations used in the vector." << endl;
	cout << "5. num_sub_op (required) : number of sub operations used in the vector." << endl;
}

/*
	This function process the args for the program and check if they size match.
*/
deque<string> process_args(int argc,char* argv[]){
	deque<string> args;

	for(int i=1;i<argc;i++){
		string arg(argv[i]);

		args.push_back(arg);
	}

	bool check_parameters = (args.size() == 1 and (args[0] == "--help" or args[0] == "--test"));
	check_parameters = check_parameters or (args.size() == 5 and args[0] == "--tow");
	check_parameters = check_parameters or (args.size() == 6 and (args[0] == "--towp" or args[0] == "--towpp"));

	assert(check_parameters);

	return args;
}

int main(int argc,char* argv[]){
	cin.tie(0);
	ios::sync_with_stdio(0);
	
	deque<string> args = process_args(argc,argv);

	if(args[0] == "--help"){
		help();
	}else if(args[0] == "--tow"){
		test_tow(stod(args[1]),stoll(args[2]),stoi(args[3]),stoi(args[4]),SAME_STREAM);
	}else if(args[0] == "--towp"){
		test_towp(stod(args[1]),stod(args[2]),stoll(args[3]),stoi(args[4]),stoi(args[5]),SAME_STREAM);
	}else if(args[0] == "--towpp"){
		test_towpp(stod(args[1]),stod(args[2]),stoll(args[3]),stoi(args[4]),stoi(args[5]),SAME_STREAM);
	}else{
		//test_bind_functions(256LL);
		cout << test_if_family_is_4_wise(4) << endl;
	}

	return 0;
}	