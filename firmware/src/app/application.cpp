#include <Arduino.h>

#include "application.h"
#include "../scheduler/scheduler.h"
#include "../config/board_config.h"
#include "../devices/button/button.h"
#include "../devices/led/led.h"
#include "../devices/servo/servo.h"
#include "../pwm/pwm.h"

int brightness0 = 0;
int brightnessDirection0 = 1;

int brightness1 = 0;
int brightnessDirection1 = 1;

// anonymous namespace (file-scope)
namespace
{
    // --------------------------------------------------
    // Devices
    // --------------------------------------------------
    Devices::LED buttonControlledLed(Board::BUTTON_CONTROLLED_LED_PIN);
    Devices::LED heartbeatLed(Board::HEARTBEAT_LED_PIN);
    Devices::LED fastFlashLED(Board::FAST_FLASH_LED_PIN);

    // These two LEDs will use PWM
    Devices::LED pwmLedDevice0(Board::PWM_LED_PIN_0);
    Devices::LED pwmLedDevice1(Board::PWM_LED_PIN_1);

    Devices::Button button;

    // --------------------------------------------------
    // PWM Configuration
    // --------------------------------------------------
    PWMConfig led0PwmConfig
    {
        0,
        8
    };

    PWMConfig led1PwmConfig
    {
        1,
        8
    };

    PWMConfig servo0PwmConfig
    {
        2,      // channel
        8       // resolution
    };

    // --------------------------------------------------
    // PWM Resources
    // --------------------------------------------------
    PWM pwmLed0(led0PwmConfig);
    PWM pwmLed1(led1PwmConfig);

    PWM pwmServo0(servo0PwmConfig);

    // --------------------------------------------------
    // Servo Configuration
    // --------------------------------------------------
    Devices::ServoConfig servo0Config
    {
        Board::PWM_SERVO_PIN_0,  // pin

        0,                       // minimum angle
        180,                     // maximum angle
        90,                      // default angle

        625,                    // minimum pulse width (us)
        2665,                    // maximum pulse width (us)

        50                       // frequency (Hz)
    };

    // --------------------------------------------------
    // Servo Device
    // --------------------------------------------------

    Devices::Servo servo0(servo0Config);


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
            pwmLedDevice0.setBrightness(brightness0);

            brightness0 += brightnessDirection0;

            if (brightness0 >= 100)
            {
                brightness0 = 100;
                brightnessDirection0 = -1;
            }
            else if (brightness0 <= 0)
            {
                brightness0 = 0;
                brightnessDirection0= 1;
            }
        };

    auto pwmCallback1 = []()
        {
            pwmLedDevice1.setBrightness(brightness1);

            brightness1 += brightnessDirection1;

            if (brightness1 >= 100)
            {
                brightness1 = 100;
                brightnessDirection1 = -1;
            }
            else if (brightness1 <= 0)
            {
                brightness1 = 0;
                brightnessDirection1= 1;
            }
        };

    // --------------------------------------------------
    // Servo Test
    // --------------------------------------------------

    int servoTestAngle = 0;

    auto servo0Callback = []()
    {
        Serial.print("Servo moving to: ");
        Serial.print(servoTestAngle);
        Serial.println(" degrees");

        servo0.moveTo(servoTestAngle);

        // Move through 0 -> 90 -> 180 -> 0...
        if (servoTestAngle == 0)
        {
            servoTestAngle = 90;
        }
        else if (servoTestAngle == 90)
        {
            servoTestAngle = 180;
        }
        else
        {
            servoTestAngle = 0;
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

    Timing::Scheduler::Task servoTask {
        servo0Callback,
        2000, // interval duration: 2000 ms (2 s)
        0
    };
}

namespace App
{
    void Application::initialize()
    {
        // --------------------------------------------------
        // Devices
        // --------------------------------------------------
        buttonControlledLed.initialize();
        heartbeatLed.initialize();
        fastFlashLED.initialize();
        button.initialize();

        // --------------------------------------------------
        // PWM configuration
        // --------------------------------------------------
        pwmLed0.configure(Board::PWM_LED_PIN_0, 1000);
        pwmLed1.configure(Board::PWM_LED_PIN_1, 5000);

        // Attach PWM resources to LED devices
        pwmLedDevice0.enablePWM(pwmLed0);
        pwmLedDevice1.enablePWM(pwmLed1);

        // Start PWM hardware
        pwmLed0.start();
        pwmLed1.start();

        // --------------------------------------------------
        // Servo PWM
        // --------------------------------------------------

        // Give the Servo access to its PWM resource.
        servo0.setPWM(pwmServo0);

        // Servo initializes/configures/starts its PWM
        // and moves to its default angle.
        servo0.initialize();

        // --------------------------------------------------
        // Scheduler tasks
        // --------------------------------------------------
        scheduler.addTask(heartbeatTask);
        scheduler.addTask(fastFlashTask);
        scheduler.addTask(statusPrintTask);
        scheduler.addTask(pwmTask0);
        scheduler.addTask(pwmTask1);
        scheduler.addTask(servoTask);

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