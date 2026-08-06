#pragma once

#include <stdint.h>

// module should know nothing about LEDs or buttons
// should only know how to manipulate pins
namespace HAL::GPIO
{
    // use bool to avoid name collisions in different toolchains
    // `true` = Output, `false` = Input (with pull-up)
    void initialize(uint8_t pin, bool isOutput);
    void write(uint8_t pin, bool state);
    bool read(uint8_t pin);
}