#include "scheduled_executor.h"

ScheduledExecutor::ScheduledExecutor(size_t threadPoolSize) : _pool(threadPoolSize),
    _waiter(&ScheduledExecutor::Start, this), _lastId(0), _finish(false)
{}

ScheduledExecutor::~ScheduledExecutor(){
    Shutdown();
    _finish = true;
    _condition.notify_one();
    _waiter.join();
}

void ScheduledExecutor::CancelPeriodicTask(ScheduledExecutor::TaskID const & id) {
    std::unique_lock<std::mutex> lock(_futureTasksMutex);
    _futureTasks.erase(id);
}

void ScheduledExecutor::Shutdown() {
    {
        std::unique_lock<std::mutex> lock(_futureTasksMutex);
        _futureTasks.clear();
    }
}

void ScheduledExecutor::Start() {
    bool needErase = false;
    TaskID idToErase = 0;
    while (true) {
        std::unique_lock<std::mutex> lock(_futureTasksMutex);

        while (!_finish && _futureTasks.empty()) {
            _condition.wait(lock);
        }
        if (_finish) {
            return;
        }
        for (auto it = _futureTasks.begin(); it != _futureTasks.end(); ++it) {
            if (needErase) {
                _futureTasks.erase(idToErase);
                needErase = false;
            }
            if (it->second.NeedLaunch()) {
                _pool.enqueue(it->second.Function());
                if (std::chrono::milliseconds(0) != it->second.Period()) {
                    it->second.SetNextLaunchTime(it->second.Period());
                } else {
                    idToErase = it->first;
                    needErase = true;
                }
            }
        }
        _condition.wait_until(lock, (_futureTasks.begin())->second.NextLaunchTime());
    }
}

