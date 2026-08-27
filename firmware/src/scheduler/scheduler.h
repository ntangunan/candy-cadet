#pragma once

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
        Task task_;
    };

}
