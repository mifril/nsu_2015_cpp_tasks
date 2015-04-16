#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <utility>
#include <condition_variable>

class ThreadPool {
public:
    explicit ThreadPool(size_t size = 10);
    ~ThreadPool();

    void enqueue(const std::function<void()>& f);

private:
    class Worker {
    public:
        Worker(ThreadPool &pool) : _pool(pool) { }
        void operator()();
    private:
        ThreadPool &_pool;
    };

    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;
    std::mutex _tasksMutex;
    std::condition_variable _condition;
    bool _stop;
};

#endif // THREADPOOL_H
