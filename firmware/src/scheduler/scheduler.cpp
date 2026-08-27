#include "scheduler.h"
#include <Arduino.h>

namespace Timing
{
    void Scheduler::addTask(Task task)
    {
        task_ = task;
    }

    void Scheduler::update()
    {
        // get current time
        unsigned long currMillis = millis();

        // check if it is time to run a task
        if (currMillis - task_.lastRun >= task_.interval) {
            task_.lastRun = currMillis;
            task_.callback();
        }
    }
}
