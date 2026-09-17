#pragma once

#include <cstdint>
#include "../../pwm/pwm.h"
#include "../../pwm/pwm_resource_manager.h"

namespace Devices
{
    struct ServoConfig
    {
        uint8_t pin;

        int minAngle;
        int maxAngle;
        int defaultAngle;
        
        int minPulseWidth;
        int maxPulseWidth;

        int pwmFrequency;
    };

    class Servo
    {
    public:
        Servo(const ServoConfig& config);

        // servo behavior
        void initialize();
        void moveTo(int angle);
        void moveToDefault();

        // pwm behavior
        void setPWM(
            PWM& pwm,
            PWMResourceManager& manager,
            PWMAllocationHandle handle
        );
        void releasePWM();
    
    private:
        ServoConfig config_;

        int convertAngleToPulseWidth_(int angle);

        PWM* pwm_ = nullptr;
        PWMResourceManager* pwmManager_ = nullptr;

        PWMAllocationHandle pwmHandle_{-1, -1};
    };
}