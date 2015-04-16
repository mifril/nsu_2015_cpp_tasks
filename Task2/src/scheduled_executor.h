#ifndef SHEDULEDEXECUTOR_H
#define SHEDULEDEXECUTOR_H

#include <iostream>
#include <utility>
#include <functional>
#include <chrono>
#include <future>
#include <map>
#include "thread_pool.h"
#include "task.h"

class ScheduledExecutor {
public:
    // Уникальный в рамках одного executor-а идентификатор задачи.
    typedef size_t TaskID;

    explicit ScheduledExecutor(size_t threadPoolSize = 10);
    ~ScheduledExecutor();

    ScheduledExecutor(ScheduledExecutor const &) = delete;
    ScheduledExecutor(ScheduledExecutor &&) = delete;
    ScheduledExecutor & operator= (ScheduledExecutor const &) = delete;
    ScheduledExecutor & operator= (ScheduledExecutor &&) = delete;

    // Запускает отложенную задачу, которая будет выполнена 1 раз с заданным delay.
    // Если delay == 0, задача будет немедленно поставлена в очередь.
    // Параметры:
    //  fn - функция, которую следует запустить.
    //	delay - время в миллисекундах, через которое нужно запустить задачу.
    template<typename Fn>
    TaskID ScheduleDelayedTask(Fn && fn, std::chrono::milliseconds delay = std::chrono::milliseconds(0)) {
        std::unique_lock<std::mutex> lock(_futureTasksMutex);
        Task task(std::forward<Fn>(fn), delay);
        _futureTasks.insert(std::make_pair(_lastId++, task));
        _condition.notify_one();
        return _lastId++;
    }

    // Запускает отложенную задачу, которая будет выполнена 1 раз с заданным delay.
    // Если delay == 0, задача будет _немедленно_ запущена.
    // Параметры:
    // 	fn - функция, которую следует запустить.
    //	delay - время, через которое нужно запустить задачу, в миллисекундах.
    //	period - время в миллисекундах, через которое задача будет повторяться.
    template<typename Fn>
    TaskID SchedulePeriodicTask(Fn && fn, std::chrono::milliseconds delay, std::chrono::milliseconds period) {
        std::unique_lock<std::mutex> lock(_futureTasksMutex);
        Task task(std::forward<Fn>(fn), delay, period);
        _futureTasks.insert(std::make_pair(_lastId, task));
        _condition.notify_one();
        return _lastId++;
    }

    // Запускает задачу, которая будет посчитана только в тот момент,
    // когда на объекте std::future, возвращаемом этой функцией, будет вызван метод get().
    // Параметры:
    //	fn - задача, которая будет запущена.
    template<typename Fn>
    auto ScheduleLazyTask(Fn fn) -> std::future<typename std::result_of<Fn()>::type> {
        return std::async(std::launch::deferred, fn);
    }

    // Прекращает запуски задания с заданным id.
    // Если в данный момент это задание выполняется, то прерывать выполнение не требуется.
    void CancelPeriodicTask(TaskID const & id);

    // Прекращает запуски всех заданий.
    // Если какие-то из них выполняются в данный момент, то прерывать их выполнение не требуется.
    void Shutdown();

private:
    void AddTasksToThreadPool();
    void Start();

    ThreadPool _pool;
    std::map<TaskID, Task> _futureTasks;
    std::mutex _futureTasksMutex;
    std::condition_variable _condition;
    std::thread _waiter;
    bool _finish;
    TaskID _lastId;
};

#endif // SHEDULEDEXECUTOR_H
