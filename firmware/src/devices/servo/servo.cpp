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

        // the application must allocate and attach a valid PWM resource first
        if (pwm_ == nullptr ||
            pwmManager_ == nullptr ||
            !pwmManager_->validate(pwmHandle_))
        {
            return;
        }

        // configure PWM for servo operation
        pwm_->configure(config_.pin, config_.pwmFrequency);

        // put servo at its initial/default position
        pwm_->start();
        moveToDefault();
    }

    void Servo::moveTo(int angle)
    {
        if (pwm_ == nullptr)
        {
            return;
        }

        // validate/clamp angle
        int safeAngle = angle;
        if (angle < config_.minAngle || angle > config_.maxAngle)
        {
            safeAngle = std::clamp(angle, config_.minAngle, config_.maxAngle);
        }

        int pulseWidth = convertAngleToPulseWidth_(safeAngle);

        // give pulse width to PWM
        pwm_->setPulseWidth(pulseWidth);
    }

    void Servo::moveToDefault()
    {
        moveTo(config_.defaultAngle);
    }

    void Servo::setPWM(
        PWM& pwm,
        PWMResourceManager& manager,
        PWMAllocationHandle handle
    )
    {
        if (pwm_ != nullptr || !manager.validate(handle))
        {
            return;
        }

        pwm_ = &pwm;
        pwmManager_ = &manager;
        pwmHandle_ = handle;
    }

    void Servo::releasePWM()
    {
        if (pwm_ == nullptr || pwmManager_ == nullptr)
        {
            return;
        }

        pwm_->stop();
        pwmManager_->release(pwmHandle_);

        pwm_ = nullptr;
        pwmManager_ = nullptr;
        pwmHandle_ = {-1, -1};
    }

    int Servo::convertAngleToPulseWidth_(int angle)
    {
        int pulseWidthDifference = config_.maxPulseWidth - config_.minPulseWidth;

        return config_.minPulseWidth
            + ((angle - config_.minAngle) * pulseWidthDifference)
            / (config_.maxAngle - config_.minAngle);
    }
}