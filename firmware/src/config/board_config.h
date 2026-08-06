#pragma once

#include <stdint.h>

// job is to describe hardware
namespace Board
{
    // blue led toggled by button
    constexpr uint8_t STATUS_LED_PIN = 2;
    constexpr uint8_t USER_BUTTON_PIN = 15;

    // red led toggled by timing
    constexpr uint8_t TIMER_LED_PIN = 0;
    
}