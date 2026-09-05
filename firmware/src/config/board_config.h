#pragma once

#include <stdint.h>

// job is to describe hardware
namespace Board
{
    // led toggled by button
    constexpr uint8_t BUTTON_CONTROLLED_LED_PIN = 32;
    constexpr uint8_t USER_BUTTON_PIN = 14;

    // led toggled by timing
    constexpr uint8_t HEARTBEAT_LED_PIN = 33;

    // led toggled by callback
    constexpr uint8_t FAST_FLASH_LED_PIN = 13;

    // led with brightness controlled by pwm
    constexpr uint8_t PWM_LED_PIN_0 = 19;

    // led with brightness controlled by pwm
    constexpr uint8_t PWM_LED_PIN_1 = 18;
}