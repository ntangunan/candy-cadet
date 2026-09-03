#include "pwm.h"
#include <Arduino.h>

PWM::PWM()
    : _isActive(false),
      _duty(0)
{
}

void PWM::configure(int gpioPin, int channel, int frequency, int resolution)
{
    _gpioPin = gpioPin;
    _channel = channel;
    _frequency = frequency;
    _resolution = resolution;

    ledcSetup(_channel, _frequency, _resolution);
    ledcAttachPin(_gpioPin, _channel);
}

void PWM::setDuty(int duty)
{
    _duty = duty;
    ledcWrite(_channel, duty); // ledc channel, duty cycle: 128 / 255 = ~50%
}

void PWM::start()
{
    if (_isActive == true)
    {
        return;
    }

    _isActive = true;
    setDuty(_duty);
}

void PWM::stop()
{
    if (_isActive == false)
    {
       return;
    }

    _isActive = false;
    ledcWrite(_channel, 0);
}