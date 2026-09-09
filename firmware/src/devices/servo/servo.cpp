#include "servo.h"
#include<Arduino.h>

#include <algorithm>

namespace Devices
{
    Servo::Servo(const ServoConfig& config)
        : config_(config)
        {
        }
    
    void Servo::initialize()
    {
        // validate ServoConfig
        if (config_.minAngle >= config_.maxAngle)
        {
            return;
        }

        if (config_.minPulseWidth >= config_.maxPulseWidth)
        {
            return;
        }

        if (config_.defaultAngle < config_.minAngle ||
            config_.defaultAngle > config_.maxAngle)
        {
            return;
        }

        // create/acquire PWM resource
        if (pwm_ == nullptr)
        {
            return;
        }

        // TODO: configure PWM for servo operation
        pwm_->configure(config_.pin, config_.frequency);

        // put servo at its initial/default position
        pwm_->start();
        moveToDefault();
    }

    void Servo::moveTo(int angle)
    {
        // validate/clamp angle
        int safeAngle = angle;
        if (angle < config_.minAngle || angle > config_.maxAngle)
        {
            safeAngle = std::clamp(angle, config_.minAngle, config_.maxAngle);
        }

        int pulseWidth = convertAngleToPulseWidth_(safeAngle);

        // debugging
        Serial.print("Safe angle: ");
        Serial.print(safeAngle);
        Serial.print(" -> Pulse width: ");
        Serial.print(pulseWidth);
        Serial.println(" us");

        // give pulse width to PWM
        pwm_->setPulseWidth(pulseWidth);
    }

    void Servo::moveToDefault()
    {
        moveTo(config_.defaultAngle);
    }

    void Servo::setPWM(PWM& pwm)
    {
        if (pwm_ != nullptr)
        {
            return;
        }

        pwm_ = &pwm;
    }

    int Servo::convertAngleToPulseWidth_(int angle)
    {
        int pulseWidthDifference = config_.maxPulseWidth - config_.minPulseWidth;

        return config_.minPulseWidth
            + ((angle - config_.minAngle) * pulseWidthDifference)
            / (config_.maxAngle - config_.minAngle);
    }
}