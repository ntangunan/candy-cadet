#include "motor.h"


namespace Devices
{
    Motor::Motor(const MotorConfig& config)
        : config_(config),
        speed_(0)
        {
        }
    
    void Motor::initialize()
    {
        // create/acquire PWM resource
        if (forwardPwm_ == nullptr || backwardPwm_ == nullptr)
        {
            return;
        }

        forwardPwm_->configure(config_.controlPin1, config_.pwmFrequency);
        backwardPwm_->configure(config_.controlPin2, config_.pwmFrequency);

        // setup initial state
        stop();
        
        forwardPwm_->start();
        backwardPwm_->start();
    }

    void Motor::setSpeed(int speed)
    {
        if (config_.directionInverted)
        {
            speed = -speed;
        }
        
        if (speed > 0)
        {
            setForward_(speed);
        } else if (speed < 0)
        {
            setBackward_(speed);
        } 
        else
        {
            stop();
        }

        speed_ = speed;

    }

    void Motor::stop()
    {
        forwardPwm_->setPercentage(0);
        backwardPwm_->setPercentage(0);

        speed_ = 0;
    }

    void Motor::setPWM(
        PWM& forwardPwm,
        PWM& backwardPwm,
        PWMResourceManager& manager,
        PWMAllocationHandle forwardPwmHandle,
        PWMAllocationHandle backwardPwmHandle

    )
    {
        if (forwardPwm_ != nullptr ||
            backwardPwm_ != nullptr || 
            !manager.validate(forwardPwmHandle)||
            !manager.validate(backwardPwmHandle))
        {
            return;
        }

        forwardPwm_ = &forwardPwm;
        backwardPwm_ = &backwardPwm;
        pwmManager_ = &manager;
        forwardPwmHandle_ = forwardPwmHandle;
        backwardPwmHandle_ = backwardPwmHandle;
    }

    void Motor::releasePWM()
    {
        if (forwardPwm_ == nullptr || 
            backwardPwm_ == nullptr || 
            pwmManager_ == nullptr)
        {
            return;
        }

        forwardPwm_->stop();
        backwardPwm_->stop();

        pwmManager_->release(forwardPwmHandle_);
        pwmManager_->release(backwardPwmHandle_);

        forwardPwm_ = nullptr;
        backwardPwm_ = nullptr;
        pwmManager_= nullptr;
        forwardPwmHandle_ = {-1, -1};
        backwardPwmHandle_ = {-1, -1};
    }

    void Motor::setForward_(int speed)
    {
        forwardPwm_->setPercentage(speed);
        backwardPwm_->setPercentage(0);
    }

    void Motor::setBackward_(int speed)
    {
        // cancelling the negative speed argument because PWM::setPercentage does not accept negative values
        backwardPwm_->setPercentage(-speed);
        forwardPwm_->setPercentage(0);
    }
    
}