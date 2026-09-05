#pragma once

#include <stdint.h>

#include "../../pwm/pwm.h"


// never exposes GPIO (ex: shouldn't know what pin the LED is connected to)
namespace Devices
{
    class LED
    {
    public:
        // tells us that LED is connected to GPIO X
        LED(uint8_t pin);

        // standard led behavior
        void initialize();
        void on();
        void off();
        void toggle();

        // pwm behavior
        void enablePWM(PWM& pwm);
        void setBrightness(int percentage);
    
    private:
        uint8_t pin_;
        bool state_ = false;

        PWM* pwm_ = nullptr;

    };
}