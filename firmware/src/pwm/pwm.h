#pragma once

#include "../config/board_config.h"
#include <array>

struct PWMConfig
{
    int channel;
    int resolution;
};

struct PWMResource
{
    int resourceId;
    bool isAllocated;
    int allocationId;
    int pin;
    int frequency;
    int resolution;
};

struct PWMRequirements
{
    int pin;
    int frequency;
    int resolution;
};

struct PWMAllocationHandle
{
    int resourceId;
    int allocationId;
};

struct PWMResourceManager
{
    std::array<PWMResource, Board::MAX_PWM_RESOURCES> resources; // collection of PWM resources
    int nextAllocationId;
    int updateNextAllocationId();
    PWMAllocationHandle allocate(PWMRequirements requirements);
    bool validate(PWMAllocationHandle handle);
    void release(PWMAllocationHandle handle);
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