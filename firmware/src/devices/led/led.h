#pragma once
#include <stdint.h>

// never exposes GPIO (ex: shouldn't know what pin the LED is connected to)
namespace Devices
{
    class LED
    {
    public:
        // tells us that LED is connected to GPIO X
        LED(uint8_t pin);

        void initialize();
        void on();
        void off();
        void toggle();
    
    private:
        uint8_t pin_;
        bool state_ = false;
    };
}