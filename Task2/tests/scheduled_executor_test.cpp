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
        std::cout << "Periodic: " << std::endl;
    }
};

class Lazy {
public:
    void operator ()() {
        std::cout << "Lazy" << std::endl;
    }
};

TEST_F(ScheduledExecutorTest, Test1) {
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

    std::cout << "HEY" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

