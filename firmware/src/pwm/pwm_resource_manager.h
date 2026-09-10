#pragma once

#include "../config/board_config.h"
#include <array>

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
    PWMResourceManager();

    std::array<PWMResource, Board::MAX_PWM_RESOURCES> resources; // collection of PWM resources
    
    int nextAllocationId;
    int updateNextAllocationId();
    
    PWMAllocationHandle allocate(PWMRequirements requirements);
    bool validate(PWMAllocationHandle handle);
    void release(PWMAllocationHandle handle);
};