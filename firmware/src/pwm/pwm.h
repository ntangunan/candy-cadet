#pragma once

struct PWMConfig
{
    int channel;
    int resolution;
};

class PWM
{
public:
    PWM(const PWMConfig& config);

    // configuration/control functions
    void configure(int pin, int frequency);
    void setDuty(int duty);
    void setPercentage(int percentage); // whole number like: 50 represents 50%
    void setPulseWidth(int pulseWidth); // whole number in micro seconds
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

    // helpers
    int getMaxDuty_();
};