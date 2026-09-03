#include <Arduino.h>

#include "application.h"
#include "../scheduler/scheduler.h"
#include "../config/board_config.h"
#include "../devices/button/button.h"
#include "../devices/led/led.h"
#include "../pwm/pwm.h"

int pwmDuty0 = 0;
int pwmDirection0 = 1;

int pwmDuty1 = 0;
int pwmDirection1 = 1;

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
    // PWM
    // --------------------------------------------------
    PWM pwmLed0;
    PWM pwmLed1;
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

    auto pwmCallback0 = []()
        {
            pwmLed0.setDuty(pwmDuty0);

            pwmDuty0 += pwmDirection0;

            if (pwmDuty0 >= 255)
            {
                pwmDuty0 = 255;
                pwmDirection0 = -1;
            }
            else if (pwmDuty0 <= 0)
            {
                pwmDuty0 = 0;
                pwmDirection0 = 1;
            }
        };

    auto pwmCallback1 = []()
        {
            pwmLed1.setDuty(pwmDuty1);

            pwmDuty1 += pwmDirection1;

            if (pwmDuty1 >= 255)
            {
                pwmDuty1 = 255;
                pwmDirection1 = -1;
            }
            else if (pwmDuty1 <= 0)
            {
                pwmDuty1 = 0;
                pwmDirection1 = 1;
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

    Timing::Scheduler::Task pwmTask0 {
        pwmCallback0,
        10, // interval duration: 10 ms
        0
    };

    Timing::Scheduler::Task pwmTask1 {
        pwmCallback1,
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

        pwmLed0.configure(19, 0, 3, 8);
        pwmLed1.configure(18, 1, 5000, 8);

        pwmLed0.setDuty(255);
        pwmLed1.setDuty(32);

        pwmLed0.start();
        pwmLed1.start();

        // scheduler tasks
        scheduler.addTask(heartbeatTask);
        scheduler.addTask(fastFlashTask);
        scheduler.addTask(statusPrintTask);
        scheduler.addTask(pwmTask0);
        scheduler.addTask(pwmTask1);

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