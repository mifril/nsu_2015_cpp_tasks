#ifndef TASK_H
#define TASK_H

#include <chrono>
#include <functional>

class Task {
public:
    template <class Fn>
    Task(Fn && f, std::chrono::milliseconds delay, std::chrono::milliseconds period = std::chrono::milliseconds(0)) :
        _taskFunction(f), _delay(delay), _period(period), _nextLaunchTime(std::chrono::steady_clock::now() + delay)
    {}
    ~Task()
    {}

    inline void operator()() {
        _taskFunction();
    }
    inline bool NeedLaunch() const {
        return std::chrono::steady_clock::now() >= _nextLaunchTime;
    }
    inline const std::chrono::steady_clock::time_point& NextLaunchTime() const {
        return _nextLaunchTime;
    }
    inline const std::chrono::steady_clock::time_point& SetNextLaunchTime(std::chrono::milliseconds delay) {
        return _nextLaunchTime = std::chrono::steady_clock::now() + delay;
    }
    inline const std::chrono::milliseconds& Period() const {
        return _period;
    }
    inline const std::function<void()>& Function() const {
        return _taskFunction;
    }

private:
    std::function<void()> _taskFunction;
    std::chrono::milliseconds _delay;
    std::chrono::steady_clock::time_point _nextLaunchTime;
    std::chrono::milliseconds _period;
};

#endif // TASK_H
