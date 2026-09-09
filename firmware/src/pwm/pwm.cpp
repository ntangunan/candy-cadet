#include <Arduino.h>

#include "pwm.h"

PWM::PWM(const PWMConfig& config)
    : channel_(config.channel),
      resolution_(config.resolution),
      isActive_(false),
      duty_(0)
{
}

void PWM::configure(int gpioPin, int frequency)
{
    gpioPin_ = gpioPin;
    frequency_ = frequency;

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

    int maxDuty = getMaxDuty_();

    setDuty((maxDuty * percentage) / 100);
}

void PWM::setPulseWidth(int pulseWidth)
{
    // calculate PWM period from frequency
    if (frequency_ <= 0)
    {
        return;
    }
    int periodUs = 1000000 / frequency_;
    
    // validate pulse width
    int safePulseWidth = pulseWidth;

    if (safePulseWidth < 0)
    {
        safePulseWidth = 0;
    }
    else if (safePulseWidth > periodUs)
    {
        safePulseWidth = periodUs;
    }

    // convert pulse width to duty
    int maxDuty = getMaxDuty_();
    int duty = (safePulseWidth * maxDuty) / periodUs;

    // debugging
    Serial.print("Pulse width: ");
    Serial.print(safePulseWidth);
    Serial.print(" us -> Duty: ");
    Serial.println(duty);

    // send duty to hardware
    setDuty(duty);
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

int PWM::getMaxDuty_()
{
    return (1 << resolution_) - 1;
}