#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <string>
#include <chrono>

class Timer {
public:
    Timer(const std::string& _title);
    ~Timer();

private:
    std::string title;
    std::chrono::steady_clock::time_point start;
};

#endif // TIMER_H
