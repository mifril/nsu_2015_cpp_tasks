#include "timer.h"

Timer::Timer(const std::string& _title) : title(_title), start(std::chrono::steady_clock::now())
{}

Timer::~Timer() {
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << title << std::chrono::duration_cast< std::chrono::seconds >(end - start).count() << std::endl;
}
