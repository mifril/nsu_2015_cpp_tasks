#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <string>
#include <chrono>

class Timer {
public:
    Timer();
    ~Timer();
    void start();
    void stop();
    std::chrono::duration<size_t>::rep getDuration();
private:
    std::chrono::steady_clock::time_point _start;
    std::chrono::steady_clock::time_point _end;
};

#endif // TIMER_H
