#include "scheduler.h"
#include <Arduino.h>

namespace Timing
{
    void Scheduler::addTask(Task task)
    {
        if (taskCount_ < MAX_TASKS)
        {
            tasks_[taskCount_] = task;
            taskCount_++;
        }
    }

    void Scheduler::update()
    {
        // get current time
        unsigned long currMillis = millis();

        // for each task check if it is time to run it
        for(int i = 0; i < taskCount_; i++)
        {
            if (currMillis - tasks_[i].lastRun >= tasks_[i].interval)
            {
                tasks_[i].lastRun = currMillis;
                tasks_[i].callback();
            }
        }
    }
}
