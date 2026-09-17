#include <Arduino.h>
#include <memory>

#include "application.h"
#include "../scheduler/scheduler.h"
#include "../config/board_config.h"
#include "../pwm/pwm.h"
#include "../devices/led/led.h"
#include "../devices/button/button.h"
#include "../devices/servo/servo.h"
#include "../devices/motor/motor.h"

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

    Devices::Button button;

    // --------------------------------------------------
    // PWM Resources and Management
    // --------------------------------------------------
    PWMResourceManager pwmResourceManager;

    PWMAllocationHandle led0PwmHandle{-1, -1};
    std::unique_ptr<PWM> pwmLed0;

    PWMAllocationHandle servo0PwmHandle{-1, -1};
    std::unique_ptr<PWM> pwmServo0;


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
        // PWM LED
        // --------------------------------------------------

        PWMRequirements led0PwmRequirements
        {
            Board::PWM_LED_PIN_0,
            1000,
            8
        };

        led0PwmHandle = pwmResourceManager.allocate(led0PwmRequirements);

        if (pwmResourceManager.validate(led0PwmHandle))
        {
            PWMConfig led0PwmConfig
            {
                led0PwmHandle.resourceId,
                led0PwmRequirements.resolution
            };

            pwmLed0 = std::make_unique<PWM>(led0PwmConfig);
            pwmLed0->configure(
                led0PwmRequirements.pin,
                led0PwmRequirements.frequency
            );
            pwmLedDevice0.enablePWM(
                *pwmLed0,
                pwmResourceManager,
                led0PwmHandle
            );
            pwmLed0->start();
        }

        // --------------------------------------------------
        // Servo PWM
        // --------------------------------------------------

        PWMRequirements servo0PwmRequirements
        {
            servo0Config.pin,
            servo0Config.pwmFrequency,
            8
        };

        servo0PwmHandle = pwmResourceManager.allocate(servo0PwmRequirements);

        if (pwmResourceManager.validate(servo0PwmHandle))
        {
            PWMConfig servo0PwmConfig
            {
                servo0PwmHandle.resourceId,
                servo0PwmRequirements.resolution
            };

            pwmServo0 = std::make_unique<PWM>(servo0PwmConfig);
            servo0.setPWM(*pwmServo0, pwmResourceManager, servo0PwmHandle);

            // Servo configures/starts its allocated PWM and moves to default.
            servo0.initialize();
        }

        // --------------------------------------------------
        // Scheduler tasks
        // --------------------------------------------------
        scheduler.addTask(heartbeatTask);
        scheduler.addTask(fastFlashTask);
        scheduler.addTask(statusPrintTask);
        scheduler.addTask(pwmTask0);
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