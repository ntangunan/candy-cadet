#include "application.h"
#include <Arduino.h>

#include "../config/board_config.h"
#include "../devices/button/button.h"
#include "../devices/led/led.h"

// timing control variables (unsigned long)
unsigned long heartbeatLastRun = 0;
const unsigned long heartbeatInterval = 500; // interval duration: 500 ms (0.5 s)

unsigned long statusLastRun = 0;
const unsigned long statusInterval = 1000; // interval duration: 1000 ms (1 s)

// Define the devices in an anonymous namespace (file-scope)
namespace
{
    Devices::LED statusLed(Board::STATUS_LED_PIN);
    Devices::LED heartbeatLed(Board::HEARTBEAT_LED_PIN);
    Devices::LED callbackLed(Board::CALLBACK_LED_PIN);
    Devices::Button button;

    // void (*heartbeatCallback)();

    // void taskA()
    // {
    //     Serial.println("A");
    // }

    // void taskB()
    // {
    //     Serial.println("B");
    // }
}

namespace App
{
    void Application::initialize()
    {
        statusLed.initialize();
        heartbeatLed.initialize();
        callbackLed.initialize();
        button.initialize();

        // capture by heartbeatLed by reference
        auto testCallback = [&]()
        {
            callbackLed.toggle();
        };
        
        testCallback();
    }
    
    void Application::update()
    {
        // button pressed -> blue toggled
        bool pressed = button.isPressed();

        if (pressed)
        {
            statusLed.on();
        }
        else
        {
            statusLed.off();
        }

        // capture time
        unsigned long currMillis = millis();
        
        // toggle red through timing interval
        if (currMillis - heartbeatLastRun >= heartbeatInterval) {
            heartbeatLastRun = currMillis;
            heartbeatLed.toggle();
        }

        // print status to serial monitor
        if (currMillis - statusLastRun >= statusInterval) {
            statusLastRun = currMillis;
            Serial.println("Firmware alive");
        }
        
    }
}