#include "application.h"
#include "../scheduler/scheduler.h"
#include <Arduino.h>

#include "../config/board_config.h"
#include "../devices/button/button.h"
#include "../devices/led/led.h"

int pwmDuty = 0;
int pwmDirection = 1;
// think of these as state of the fade
// pwmDuty
//    │
//    ├── 0 → LED off
//    ├── 128 → ~50%
//    └── 255 → full brightness

// pwmDirection
//    │
//    ├── +1 → getting brighter
//    └── -1 → getting darker

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
    // Callbacks
    // --------------------------------------------------

    auto heartbeatLedCallback = []()
        {
            heartbeatLed.toggle();
        };

    auto fastFlashCallback = []()
        {
            fastFlashLED.toggle();
        };

    auto statusPrintCallback = []()
        {
            Serial.println("Firmware alive");
        };

    auto pwmCallback = []()
        {
            ledcWrite(0, pwmDuty);

            pwmDuty += pwmDirection;

            if (pwmDuty >= 255)
            {
                pwmDuty = 255;
                pwmDirection = -1;
            }
            else if (pwmDuty <= 0)
            {
                pwmDuty = 0;
                pwmDirection = 1;
            }
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

    Timing::Scheduler::Task fastFlashTask {
        fastFlashCallback,
        200, // interval duration: 200 ms (0.2 s)
        0
    };

    Timing::Scheduler::Task statusPrintTask {
        statusPrintCallback,
        1000, // interval duration: 1000 ms (1 s)
        0
    };

    Timing::Scheduler::Task pwmTask {
        pwmCallback,
        10, // interval duration: 10 ms
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

        // pwm setup
        ledcSetup(0, 1000, 8); // led channel, freq, resolution
        ledcAttachPin(2, 0); // gpio pin, ledc channel
        ledcWrite(0, 16); // ledc channel, duty cycle: 128 / 255 = ~50%

        // scheduler tasks
        scheduler.addTask(heartbeatTask);
        scheduler.addTask(fastFlashTask);
        scheduler.addTask(statusPrintTask);
        scheduler.addTask(pwmTask);
    }
    
    void Application::update()
    {
        // button pressed -> led toggled
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