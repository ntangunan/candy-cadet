#pragma once

class PWM
{
public:
    PWM();

    // configuration/control functions
    void configure(int pin, int channel, int frequency, int resolution);
    void setDuty(int duty);
    void setPercentage(int percentage); // whole number like: 50 represents 50%
    void start();
    void stop();

private:
    // PWM configuration
    int gpioPin_;
    int channel_;
    int frequency_;
    int resolution_;
    int duty_;

    // PWM state
    bool isActive_;
};