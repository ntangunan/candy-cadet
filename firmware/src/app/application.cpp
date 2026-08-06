#include "application.h"
#include <Arduino.h>

#include "../devices/button/button.h"
#include "../devices/led/led.h"

// Define the devices in an anonymous namespace (file-scope)
namespace
{
    Devices::LED led;
    Devices::Button button;
}

namespace App
{
    void Application::initialize()
    {
        led.initialize();
        button.initialize();
    }
    
    void Application::update()
    {
        bool pressed = button.isPressed();
        
        Serial.println(pressed);

        if (pressed)
        {
            led.on();
        }
        else
        {
            led.off();
        }
    }
}