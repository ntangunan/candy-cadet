#pragma once

#include <cstddef>

namespace Timing
{
    class Scheduler
    {
    public:
        struct Task
        {
            void (*callback)();
            unsigned long interval;
            unsigned long lastRun;
        };

        void addTask(Task task);
        void update();

    private:
        static const std::size_t MAX_TASKS = 10;
        Task tasks_[MAX_TASKS];
        std::size_t taskCount_ = 0;
    };

}
