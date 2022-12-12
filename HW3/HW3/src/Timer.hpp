#include <bits/stdc++.h>

class Timer {
private:
    // Variables
    clock_t start_time;
    double elapsed_time;

public:
    // Constructors
    Timer();

    // Functions
    void start();
    void stop();
    void stop_acc();
    bool is_timeout(double t);
    double get_elapsed_time();
};
