#ifndef BD_TIMER_H
#define BD_TIMER_H
#include <chrono>

class timer{
    std::chrono::high_resolution_clock::time_point start_point, end_point;
public:
    timer();
    ~timer();

    void start();
    void end();
    double count();
};

#endif