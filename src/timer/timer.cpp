#include "timer.h"
using namespace std;
using namespace chrono;

namespace qsbd{
	timer::timer(){
		start_point = high_resolution_clock::now();
		end_point = start_point;
	}

	timer::~timer(){
	}

	void timer::start(){
		start_point = high_resolution_clock::now();
	}

	void timer::end(){
		end_point = high_resolution_clock::now();
	}

	double timer::count(){
		duration<double> time_interval = duration_cast<duration<double>> (end_point - start_point);

		return time_interval.count();
	}
}