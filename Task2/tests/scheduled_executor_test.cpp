#include <gtest/gtest.h>
#include <iostream>
#include "scheduled_executor.h"

class ScheduledExecutorTest : public ::testing::Test {
};

class Delayed {
public:
    void operator ()() {
        std::cout << "Delayed" << std::endl;
    }
};

class Periodic {
public:
    void operator ()() {
        std::cout << "Periodic" << std::endl;
    }
};

class Lazy {
public:
    void operator ()() {
        std::cout << "Lazy" << std::endl;
    }
};

TEST_F(ScheduledExecutorTest, TestSimple) {
    ScheduledExecutor ex;
    ex.Shutdown();

    ex.ScheduleDelayedTask(Delayed());
    ex.ScheduleDelayedTask(Delayed(), std::chrono::milliseconds(1000));

    auto id = ex.SchedulePeriodicTask(Periodic(), std::chrono::milliseconds(0), std::chrono::milliseconds(500));
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    ex.CancelPeriodicTask(id);

    Lazy lazyTask;
    auto fut = ex.ScheduleLazyTask(lazyTask);
    fut.get();

    fut = ex.ScheduleLazyTask(lazyTask);
    std::cout << "HEY" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    fut.get();
}

TEST_F(ScheduledExecutorTest, TestSimpleWithoutSleeps) {
    ScheduledExecutor ex;

    ex.ScheduleDelayedTask(Delayed());
    ex.ScheduleDelayedTask(Delayed(), std::chrono::milliseconds(1000));

    auto id = ex.SchedulePeriodicTask(Periodic(), std::chrono::milliseconds(0), std::chrono::milliseconds(500));
    ex.CancelPeriodicTask(id);

    Lazy lazyTask;
    auto fut = ex.ScheduleLazyTask(lazyTask);
    fut.get();

    fut = ex.ScheduleLazyTask(lazyTask);
    std::cout << "HEY" << std::endl;
    fut.get();
}

TEST_F(ScheduledExecutorTest, TestManyTasksWithoutSleep) {
    ScheduledExecutor ex;
    Lazy lazyTask;

    auto id = ex.SchedulePeriodicTask(Periodic(), std::chrono::milliseconds(0), std::chrono::milliseconds(1));
    for (int i = 0; i < 1000; ++i) {
        ex.ScheduleDelayedTask(Delayed());
        auto fut = ex.ScheduleLazyTask(lazyTask);
        fut.get();
    }
    ex.CancelPeriodicTask(id);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

