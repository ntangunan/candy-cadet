#include "command_callbacks.h"

namespace Communication
{

    void (*moveForward)(int speed);
    void (*moveBackward)(int speed);
    void (*turnLeft)();
    void (*turnRight)();
    void (*turnAround)();
    void (*stop)();
}