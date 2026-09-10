#include <unity.h>

#include "../../src/pwm/pwm_resource_manager.h"
#include "../../src/config/board_config.h"

void testAllocateOneResource()
{
    PWMResourceManager manager;

    PWMRequirements requirements{
        19,
        1000,
        8
    };

    PWMAllocationHandle handle = manager.allocate(requirements);

    TEST_ASSERT_TRUE(manager.validate(handle));

    TEST_ASSERT_TRUE(manager.resources[handle.resourceId].isAllocated);
    TEST_ASSERT_EQUAL(19, manager.resources[handle.resourceId].pin);
    TEST_ASSERT_EQUAL(1000, manager.resources[handle.resourceId].frequency);
    TEST_ASSERT_EQUAL(8, manager.resources[handle.resourceId].resolution);
}

void testAllocateMultipleResources()
{
    PWMResourceManager manager;

    PWMRequirements requirements{
        19,
        1000,
        8
    };

    PWMAllocationHandle handle1 = manager.allocate(requirements);
    PWMAllocationHandle handle2 = manager.allocate(requirements);
    PWMAllocationHandle handle3 = manager.allocate(requirements);

    TEST_ASSERT_TRUE(manager.validate(handle1));
    TEST_ASSERT_TRUE(manager.validate(handle2));
    TEST_ASSERT_TRUE(manager.validate(handle3));

    TEST_ASSERT_NOT_EQUAL(handle1.resourceId, handle2.resourceId);
    TEST_ASSERT_NOT_EQUAL(handle1.resourceId, handle3.resourceId);
    TEST_ASSERT_NOT_EQUAL(handle2.resourceId, handle3.resourceId);
}

void testReleaseResource()
{
    PWMResourceManager manager;

    PWMRequirements requirements{
        19,
        1000,
        8
    };

    PWMAllocationHandle handle = manager.allocate(requirements);

    TEST_ASSERT_TRUE(manager.validate(handle));

    manager.release(handle);

    TEST_ASSERT_FALSE(manager.validate(handle));

    TEST_ASSERT_FALSE(
        manager.resources[handle.resourceId].isAllocated
    );

    TEST_ASSERT_EQUAL(
        -1,
        manager.resources[handle.resourceId].allocationId
    );

    TEST_ASSERT_EQUAL(
        -1,
        manager.resources[handle.resourceId].pin
    );

    TEST_ASSERT_EQUAL(
        -1,
        manager.resources[handle.resourceId].frequency
    );

    TEST_ASSERT_EQUAL(
        -1,
        manager.resources[handle.resourceId].resolution
    );
}

void testReallocateReleasedResource()
{
    PWMResourceManager manager;

    PWMRequirements requirements{
        19,
        1000,
        8
    };

    PWMAllocationHandle firstHandle = manager.allocate(requirements);

    int releasedResourceId = firstHandle.resourceId;

    manager.release(firstHandle);

    PWMAllocationHandle secondHandle = manager.allocate(requirements);

    TEST_ASSERT_TRUE(manager.validate(secondHandle));

    TEST_ASSERT_EQUAL(
        releasedResourceId,
        secondHandle.resourceId
    );
}

void testAllocationIdChanges()
{
    PWMResourceManager manager;

    PWMRequirements requirements{
        19,
        1000,
        8
    };

    PWMAllocationHandle firstHandle = manager.allocate(requirements);

    int firstAllocationId = firstHandle.allocationId;

    manager.release(firstHandle);

    PWMAllocationHandle secondHandle = manager.allocate(requirements);

    TEST_ASSERT_EQUAL(
        firstHandle.resourceId,
        secondHandle.resourceId
    );

    TEST_ASSERT_NOT_EQUAL(
        firstAllocationId,
        secondHandle.allocationId
    );
}

void testStaleHandle()
{
    PWMResourceManager manager;

    PWMRequirements requirements{
        19,
        1000,
        8
    };

    PWMAllocationHandle oldHandle = manager.allocate(requirements);

    manager.release(oldHandle);

    PWMAllocationHandle newHandle = manager.allocate(requirements);

    TEST_ASSERT_EQUAL(
        oldHandle.resourceId,
        newHandle.resourceId
    );

    TEST_ASSERT_NOT_EQUAL(
        oldHandle.allocationId,
        newHandle.allocationId
    );

    // Old handle should now be invalid.
    TEST_ASSERT_FALSE(manager.validate(oldHandle));

    // New allocation must remain valid.
    TEST_ASSERT_TRUE(manager.validate(newHandle));

    // Attempt to release using stale handle.
    manager.release(oldHandle);

    // New allocation must still be valid.
    TEST_ASSERT_TRUE(manager.validate(newHandle));
}

void testDoubleRelease()
{
    PWMResourceManager manager;

    PWMRequirements requirements{
        19,
        1000,
        8
    };

    PWMAllocationHandle handle = manager.allocate(requirements);

    manager.release(handle);

    TEST_ASSERT_FALSE(manager.validate(handle));

    // Second release should not corrupt anything.
    manager.release(handle);

    TEST_ASSERT_FALSE(
        manager.resources[handle.resourceId].isAllocated
    );

    TEST_ASSERT_EQUAL(
        -1,
        manager.resources[handle.resourceId].allocationId
    );

    TEST_ASSERT_EQUAL(
        -1,
        manager.resources[handle.resourceId].pin
    );

    TEST_ASSERT_EQUAL(
        -1,
        manager.resources[handle.resourceId].frequency
    );

    TEST_ASSERT_EQUAL(
        -1,
        manager.resources[handle.resourceId].resolution
    );
}

void testResourceExhaustion()
{
    PWMResourceManager manager;

    PWMRequirements requirements{
        19,
        1000,
        8
    };

    PWMAllocationHandle handles[Board::MAX_PWM_RESOURCES];

    // Occupy every resource.
    for (std::size_t i = 0;
         i < Board::MAX_PWM_RESOURCES;
         i++)
    {
        handles[i] = manager.allocate(requirements);

        TEST_ASSERT_TRUE(manager.validate(handles[i]));
    }

    // Now there should be no resources left.
    PWMAllocationHandle failedHandle = manager.allocate(requirements);

    // Allocation should fail.
    TEST_ASSERT_FALSE(manager.validate(failedHandle));
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(testAllocateOneResource);
    RUN_TEST(testAllocateMultipleResources);
    RUN_TEST(testReleaseResource);
    RUN_TEST(testReallocateReleasedResource);
    RUN_TEST(testAllocationIdChanges);
    RUN_TEST(testStaleHandle);
    RUN_TEST(testDoubleRelease);
    RUN_TEST(testResourceExhaustion);

    return UNITY_END();
}