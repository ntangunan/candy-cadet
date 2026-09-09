#include <Arduino.h>

#include "pwm.h"

PWMResourceManager::PWMResourceManager()
{
    // setting values to -1 means they currently have no value
    for (std::size_t i = 0; i < resources.size(); i++)
    {
        resources[i].resourceId = i;
        resources[i].isAllocated = false;
        resources[i].allocationId = -1;
        resources[i].pin = -1;
        resources[i].frequency = -1;
        resources[i].resolution = -1;
    }
}

int PWMResourceManager::updateNextAllocationId()
{
    int currentId = nextAllocationId;
    nextAllocationId++;

    return currentId;
}


PWMAllocationHandle PWMResourceManager::allocate(PWMRequirements requirements)
{
    for (std::size_t i = 0; i < resources.size(); i++)
    {
        // look for a free resource
        if (resources[i].isAllocated)
        {
            continue;
        }

        // store requirements/configuration here
        resources[i].pin = requirements.pin;
        resources[i].frequency = requirements.frequency;
        resources[i].resolution = requirements.resolution;

        // mark resource as allocated
        resources[i].isAllocated = true;

        // generate allocationId here
        resources[i].allocationId = updateNextAllocationId();

        // create and return the handle here
        PWMAllocationHandle handle;
        handle.resourceId = i;
        handle.allocationId = resources[i].allocationId;

        return handle;
    }
}

bool PWMResourceManager::validate(PWMAllocationHandle handle)
{
    // is the resourceId in bounds?
    if (handle.resourceId < 0 || handle.resourceId >= Board::MAX_PWM_RESOURCES)
    {
        return false;
    }

    // is the resource actually allocated?
    if (!resources[handle.resourceId].isAllocated)
    {
        return false;
    }

    // does the allocationId match?
    if (handle.allocationId != resources[handle.resourceId].allocationId)
    {
        return false;
    }

    return true;
}

void PWMResourceManager::release(PWMAllocationHandle handle)
{
    if (!validate(handle))
    {
        return;
    }

    PWMResource& resource = resources[handle.resourceId];

    resource.isAllocated = false;
    resource.allocationId = -1;
    resource.pin = -1;
    resource.frequency = -1;
    resource.resolution = -1;
}

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