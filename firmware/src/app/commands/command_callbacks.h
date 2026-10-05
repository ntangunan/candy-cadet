#pragma once

namespace Communication
{
    struct CommandCallbacks
    {
        /* movement functions */
        void (*moveForward)(int);
        void (*moveBackward)(int);
        void (*turnLeft)();
        void (*turnRight)();
        void (*turnAround)();
        void (*stop)();

        /* animation functions */
        void (*wave)();
        void (*nod)();
        void (*celebrate)();

        /* status functions */
        void(*status)();
        void(*reset)();
    };
}