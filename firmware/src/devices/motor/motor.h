#pragma once

#include <Arduino.h>
#include "../../pwm/pwm.h"
#include "../../pwm/pwm_resource_manager.h"

namespace Devices
{
    struct MotorConfig
    {
        uint8_t controlPin1;
        uint8_t controlPin2;

        uint32_t pwmFrequency;
        uint8_t pwmResolution;
    };

    class Motor
    {
    public:
        Motor(const MotorConfig& config);

        // motor behavior
        void initialize();
        void setSpeed(int speed);
        void stop();

        // pwm behavior
        void setPWM(
            PWM& pwmForward,
            PWM& pwmBackward,
            PWMResourceManager& manager,
            PWMAllocationHandle forwardPwmHandle,
            PWMAllocationHandle backwardPwmHandle
        );
        void releasePWM();

    private:
        MotorConfig config_;

        PWM* forwardPwm_ = nullptr;
        PWM* backwardPwm_ = nullptr;

        PWMResourceManager* pwmManager_ = nullptr;

        PWMAllocationHandle forwardPwmHandle_;
        PWMAllocationHandle backwardPwmHandle_;

        int speed_ = 0;

        void setForward_(int speed);
        void setBackward_(int speed);
    };


}
