#include "application.h"
#include "../scheduler/scheduler.h"
#include <Arduino.h>

#include "../config/board_config.h"
#include "../devices/button/button.h"
#include "../devices/led/led.h"

// timing control variables (unsigned long)
unsigned long heartbeatLastRun = 0;
const unsigned long heartbeatInterval = 500; // interval duration: 500 ms (0.5 s)

unsigned long statusLastRun = 0;
const unsigned long statusInterval = 1000; // interval duration: 1000 ms (1 s)

// Define the devices and scheduler in an anonymous namespace (file-scope)
namespace
{
    Devices::LED buttonControlledLed(Board::BUTTON_CONTROLLED_LED_PIN);
    Devices::LED heartbeatLed(Board::HEARTBEAT_LED_PIN);
    Devices::LED fastFlashLED(Board::FAST_FLASH_LED_PIN);
    Devices::Button button;
    Timing::Scheduler scheduler;
}

namespace App
{
    void Application::initialize()
    {
        buttonControlledLed.initialize();
        heartbeatLed.initialize();
        fastFlashLED.initialize();
        button.initialize();

        auto heartbeatLedCallback = []()
        {
            heartbeatLed.toggle();
        };

        auto fastFlashCallback = []()
        {
            fastFlashLED.toggle();
        };

        auto statusCallback = []()
        {
            Serial.println("Firmware alive");
        };
    }
    
    void Application::update()
    {
        // button pressed -> blue toggled
        bool pressed = button.isPressed();

        if (pressed)
        {
            buttonControlledLed.on();
        } else {
            buttonControlledLed.off();
        }

        scheduler.update();
    }
}