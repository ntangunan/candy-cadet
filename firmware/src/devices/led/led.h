#pragma once

// never exposes GPIO (ex: shouldn't know what pin the LED is connected to)
namespace Devices
{
    class LED
    {
    public:
        void initialize();
        void on();
        void off();
        void toggle();
    
    private:
        bool state_ = false;
    };
}