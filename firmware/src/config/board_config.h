#pragma once

#include <stdint.h>

// job is to describe hardware
namespace Board
{
    // PWM hardware resources (ESP32 hardware provides 16 pwn channels)
    constexpr int MAX_PWM_RESOURCES = 16; 

    // led toggled by button
    constexpr uint8_t BUTTON_CONTROLLED_LED_PIN = 32;
    constexpr uint8_t USER_BUTTON_PIN = 33;

    // led toggled by timing
    constexpr uint8_t HEARTBEAT_LED_PIN = 4;

    // led toggled by callback
    constexpr uint8_t FAST_FLASH_LED_PIN = 13;

    // led with brightness controlled by pwm
    constexpr uint8_t PWM_LED_PIN_0 = 18;

    // servo controlled by pwm
    constexpr uint8_t PWM_SERVO_PIN_0 = 19;

    // tt motors controlled by pwm 
    // left motor A
    constexpr uint8_t LEFT_A_MOTOR_FORWARD_PIN = 25;
    constexpr uint8_t LEFT_A_MOTOR_BACKWARD_PIN = 26;

    // right motor B
    constexpr uint8_t RIGHT_B_MOTOR_FORWARD_PIN = 27;
    constexpr uint8_t RIGHT_B_MOTOR_BACKWARD_PIN = 14;

}