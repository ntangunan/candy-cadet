#include "application.h"
#include <Arduino.h>

#include "../config/board_config.h"
#include "../devices/button/button.h"
#include "../devices/led/led.h"

// timing control variables (unsigned long)
unsigned long prevMillis = 0;
const unsigned long interval = 500; // interval duration: 500 ms (0.5 s)

// Define the devices in an anonymous namespace (file-scope)
namespace
{
    Devices::LED buttonLed(Board::STATUS_LED_PIN);
    Devices::LED heartbeatLed(Board::TIMER_LED_PIN);
    Devices::Button button;
}

namespace App
{
    void Application::initialize()
    {
        buttonLed.initialize();
        heartbeatLed.initialize();
        button.initialize();
    }
    
    void Application::update()
    {
        // button pressed -> blue toggled
        bool pressed = button.isPressed();

        if (pressed)
        {
            buttonLed.on();
        }
        else
        {
            buttonLed.off();
        }

        // capture time -> red toggled
        unsigned long currMillis = millis();
        
        if (currMillis - prevMillis >= interval) {
            prevMillis = currMillis;
            heartbeatLed.toggle();
        }
    }
}