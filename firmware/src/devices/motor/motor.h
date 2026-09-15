#pragma once

#include <Arduino.h>
#include "../../pwm/pwm.h"

struct MotorConfig
{
    uint8_t controlPin1;
    uint8_t controlPin2;

    uint8_t pwmChannel;
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

private:
    MotorConfig config_;

    PWM* pwm_ = nullptr;

    bool initialized_;
    int speed_;

    void setForward_(int speed);
    void setReverse_(int speed);
};

