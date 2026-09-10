#pragma once

#include <cstdint>
#include "../../pwm/pwm.h"

namespace Devices
{
    class Servo
    {
    public:

        struct ServoConfig
        {
            std::uint8_t pin;

            int minAngle;
            int maxAngle;
            int defaultAngle;
            
            int minPulseWidth;
            int maxPulseWidth;

            int frequency;
        };

        Servo(const ServoConfig& config);

        // servo behavior
        void initialize();
        void moveTo(int angle);
        void moveToDefault();

        // pwm behavior
        void setPWM(PWM& pwm);
    
    private:
        ServoConfig config_;

        int convertAngleToPulseWidth_(int angle);

        PWM* pwm_ = nullptr;
    };
}