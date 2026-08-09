#pragma once

#include <stdint.h>

// job is to describe hardware
namespace Board
{
    // blue led toggled by button
    constexpr uint8_t STATUS_LED_PIN = 32;
    constexpr uint8_t USER_BUTTON_PIN = 14;

    // red led toggled by timing
    constexpr uint8_t HEARTBEAT_LED_PIN = 33;

    // green led toggled by callback
    constexpr uint8_t CALLBACK_LED_PIN = 13;
}