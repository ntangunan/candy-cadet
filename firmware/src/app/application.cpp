#include "application.h"
#include "../scheduler/scheduler.h"
#include <Arduino.h>

#include "../config/board_config.h"
#include "../devices/button/button.h"
#include "../devices/led/led.h"

// unsigned long statusLastRun = 0;
// const unsigned long statusInterval = 1000; // interval duration: 1000 ms (1 s)

// anonymous namespace (file-scope)
namespace
{
    // --------------------------------------------------
    // Devices
    // --------------------------------------------------
    Devices::LED buttonControlledLed(Board::BUTTON_CONTROLLED_LED_PIN);
    Devices::LED heartbeatLed(Board::HEARTBEAT_LED_PIN);
    Devices::LED fastFlashLED(Board::FAST_FLASH_LED_PIN);
    Devices::Button button;

    // --------------------------------------------------
    // Callbacks (converting lambda to a func ptr whose func returns void and takes no arguments)
    // --------------------------------------------------

    void (*heartbeatLedCallback)() = []()
        {
            heartbeatLed.toggle();
        };

    void (*fastFlashCallback)() = []()
        {
            fastFlashLED.toggle();
        };

    void (*statusCallback)() = []()
        {
            Serial.println("Firmware alive");
        };
    

    // --------------------------------------------------
    // Scheduler
    // --------------------------------------------------
    Timing::Scheduler scheduler;

    // --------------------------------------------------
    // Tasks
    // --------------------------------------------------

    Timing::Scheduler::Task heartbeatTask {
        heartbeatLedCallback,
        500, // interval duration: 500 ms (0.5 s)
        0
    };
}

namespace App
{
    void Application::initialize()
    {
        // devices
        buttonControlledLed.initialize();
        heartbeatLed.initialize();
        fastFlashLED.initialize();
        button.initialize();

        // scheduler tasks
        scheduler.addTask(heartbeatTask);
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