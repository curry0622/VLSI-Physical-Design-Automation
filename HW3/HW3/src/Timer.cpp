#include "Timer.hpp"

Timer::Timer() {
    start_time = clock();
    elapsed_time = 0;
}

void Timer::start() {
    start_time = clock();
}

void Timer::stop() {
    elapsed_time += (double)(clock() - start_time) / CLOCKS_PER_SEC;
}

bool Timer::is_timeout(double t) {
    elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    std::cout << "Elapsed time: " << elapsed_time << std::endl;
    return elapsed_time > t;
}

double Timer::get_elapsed_time() {
    return elapsed_time;
}
