#pragma once

class PWM
{
public:
    PWM();

    // configuration/control functions
    void configure(int pin, int channel, int frequency, int resolution);
    void setDuty(int duty);
    void start();
    void stop();

private:
    // PWM configuration
    int _gpioPin;
    int _channel;
    int _frequency;
    int _resolution;
    int _duty;

    // PWM state
    bool _isActive;
};