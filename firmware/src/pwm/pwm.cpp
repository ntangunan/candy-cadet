#include <Arduino.h>

#include "pwm.h"

PWM::PWM()
    : isActive_(false),
      duty_(0)
{
}

void PWM::configure(int gpioPin, int channel, int frequency, int resolution)
{
    gpioPin_ = gpioPin;
    channel_ = channel;
    frequency_ = frequency;
    resolution_ = resolution;

    ledcSetup(channel_, frequency_, resolution_);
    ledcAttachPin(gpioPin_, channel_);
}

void PWM::setDuty(int duty)
{
    duty_ = duty;
    ledcWrite(channel_, duty);
}

void PWM::setPercentage(int percentage)
{
    if (percentage < 0 || percentage > 100)
    {
        return;
    }

    int maxDuty = 2;

    for (int i = 1; i < resolution_; i++)
    {
        maxDuty *= 2;
    }

    maxDuty -= 1;

    setDuty((maxDuty * percentage) / 100);
}

void PWM::start()
{
    if (isActive_ == true)
    {
        return;
    }

    isActive_ = true;
    setDuty(duty_);
}

void PWM::stop()
{
    if (isActive_ == false)
    {
       return;
    }

    isActive_ = false;
    ledcWrite(channel_, 0);
}