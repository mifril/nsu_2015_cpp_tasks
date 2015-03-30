#include "timer.h"

Timer::Timer()
{}

Timer::~Timer()
{}

void Timer::start() {
    _start = std::chrono::steady_clock::now();
}

void Timer::stop() {
    _end = std::chrono::steady_clock::now();
}

std::chrono::duration<size_t>::rep Timer::getDuration() {
//    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count() << std::endl;
    return std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
}
