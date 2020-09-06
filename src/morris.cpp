#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

class morris{
private:
	uint32_t counter;

	bool can_update(){
		return ((std::rand() % 100) < ((1.0/((double)(1U << counter)))*100.0));
	}

public:
	morris(){
		std::srand(std::time(nullptr));
		counter = 0U;
	}

	void update(void){
		counter = counter + (can_update() ? 1 : 0);
	}

	uint32_t query(void){
		return ((1U << counter) - 1);
	}
};

class morris_p{
private:
	uint32_t s;
	morris * counters;
public:
	morris_p(double error_rate = 0.33){
		int copys = (3/(2*(error_rate*error_rate)))+1;
		s = copys;
		counters = new morris[copys];
	}

	~morris_p(){
		delete[] counters;
	}

	uint32_t copys_number(void){
		return s;
	}

	void update(void){
		for(uint32_t i = 0; i < this->copys_number() ; i++){
			counters[i].update();
		}
	}

	uint32_t query(void){
		uint32_t sum = 0;
		for(uint32_t i = 0; i < this->copys_number() ; i++){
			sum += counters[i].query();
		}

		return sum/this->copys_number();
	}
};

class morris_pp{
private:
	morris_p * counters;
	uint32_t t;
public:
	morris_pp(uint32_t copys){
		t = copys;

		counters = new morris_p[copys];
	}

	~morris_pp(){
		delete[] counters;
	}

	uint32_t copys_number(void){
		return t;
	}

	void update(){
		for(uint32_t i = 0; i < this->copys_number() ; i++){
			counters[i].update();
		}
	}

	uint32_t query(void){
		std::sort(counters,counters + this->copys_number(),[](morris_p& a,morris_p& b){
			return a.query() < b.query();
		});

		return counters[this->copys_number()/2].query();
	}
};

int main(void){
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);

	const int events = 100;
	morris test;
	morris_p test2(0.10);
	morris_p test3(0.33);
	morris_pp test4(5);

	for(int i=0;i<events;i++){
		test.update();
		test2.update();
		test3.update();
		test4.update();
	}


	std::cout << "--------------------BENCHMARK------------------" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "True value := " << events << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Using Morris Algorithm: " << std::endl;
	std::cout << "Value predicted := " << test.query() << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Using Morris Plus Algorithm(" << test2.copys_number() << " copys): " << std::endl;
	std::cout << "Value predicted := " << test2.query() << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Using Morris Plus Algorithm(" << test3.copys_number() << " copys): " << std::endl;
	std::cout << "Value predicted := " << test3.query() << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Using Morris Plus Plus Algorithm(" << test4.copys_number() << " copys): " << std::endl;
	std::cout << "Value predicted := " << test4.query() << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;

	return 0;
}