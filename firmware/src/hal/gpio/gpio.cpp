#include "gpio.h"
#include <Arduino.h>

namespace HAL::GPIO
{
    void initialize(uint8_t pin, bool isOutput)
    {
        pinMode(pin, isOutput ? OUTPUT : INPUT_PULLUP);
    }

    void write(uint8_t pin, bool state)
    {
        digitalWrite(pin, state ? HIGH : LOW);
    }

    bool read(uint8_t pin)
    {
        return digitalRead(pin);
    }
}