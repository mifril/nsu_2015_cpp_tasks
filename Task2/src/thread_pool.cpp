#include "thread_pool.h"

void ThreadPool::Worker::operator()() {
    std::function<void()> task;
    while(true) {
        {
            std::unique_lock<std::mutex> lock(_pool._tasksMutex);
            while(!_pool._stop && _pool._tasks.empty()) {
                _pool._condition.wait(lock);
            }
            if(_pool._stop) {
                return;
            }
            task = _pool._tasks.front();
            _pool._tasks.pop();
        }
        task();
    }
}

ThreadPool::ThreadPool(size_t size) : _stop(false) {
    for(auto i = 0; i < size; ++i) {
        _workers.push_back(std::thread(Worker(*this)));
    }
}

ThreadPool::~ThreadPool() {
    _stop = true;
    _condition.notify_all();
    for(auto i = 0; i < _workers.size(); ++i) {
        _workers[i].join();
    }
}

void ThreadPool::enqueue(const std::function<void()>& f) {
    {
        std::unique_lock<std::mutex> lock(_tasksMutex);
        _tasks.push(f);
    }
    _condition.notify_one();
}

