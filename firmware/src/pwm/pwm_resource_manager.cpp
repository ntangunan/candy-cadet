#include "pwm_resource_manager.h"


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
