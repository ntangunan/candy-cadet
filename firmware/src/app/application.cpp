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

    // pwm led
    PWMAllocationHandle led0PwmHandle{-1, -1};
    std::unique_ptr<PWM> pwmLed0;

    // pwm servo
    PWMAllocationHandle servo0PwmHandle{-1, -1};
    std::unique_ptr<PWM> pwmServo0;

    // pwm left motor
    PWMAllocationHandle leftMotorForwardPwmHandle{-1, -1};
    std::unique_ptr<PWM> leftPwmForwardMotor;
    PWMAllocationHandle leftMotorBackwardPwmHandle{-1, -1};
    std::unique_ptr<PWM> leftPwmBackwardMotor;

    // pwm right motor
    PWMAllocationHandle rightMotorForwardPwmHandle{-1, -1};
    std::unique_ptr<PWM> rightPwmForwardMotor;
    PWMAllocationHandle rightMotorBackwardPwmHandle{-1, -1};
    std::unique_ptr<PWM> rightPwmBackwardMotor;

    // --------------------------------------------------
    // Servo Configurations
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
    // Motor Configurations
    // --------------------------------------------------
    Devices::MotorConfig leftMotorConfig
    {
        Board::LEFT_A_MOTOR_FORWARD_PIN,
        Board::LEFT_A_MOTOR_BACKWARD_PIN,
        1000,
        8
    };

    Devices::Motor leftMotor(leftMotorConfig);


    Devices::MotorConfig rightMotorConfig
    {
        Board::RIGHT_B_MOTOR_FORWARD_PIN,
        Board::RIGHT_B_MOTOR_BACKWARD_PIN,
        1000,
        8
    };

    Devices::Motor rightMotor(rightMotorConfig);

    // --------------------------------------------------
    // Servo Devices
    // --------------------------------------------------

    Devices::Servo servo0(servo0Config);

    // --------------------------------------------------
    // Callbacks
    // --------------------------------------------------

    // led callbacks
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

    // servo callbacks
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

    // motor callbacks
    int motorTestStep = 0;

    auto leftMotorCallback = []()
    {
        switch (motorTestStep)
        {
            case 0:
                Serial.println("Motor: STOP");
                leftMotor.setSpeed(0);
                break;

            case 1:
                Serial.println("Motor: FORWARD 25%");
                leftMotor.setSpeed(25);
                break;

            case 2:
                Serial.println("Motor: FORWARD 50%");
                leftMotor.setSpeed(50);
                break;

            case 3:
                Serial.println("Motor: FORWARD 100%");
                leftMotor.setSpeed(100);
                break;

            case 4:
                Serial.println("Motor: STOP");
                leftMotor.setSpeed(0);
                break;

            case 5:
                Serial.println("Motor: REVERSE 25%");
                leftMotor.setSpeed(-25);
                break;

            case 6:
                Serial.println("Motor: REVERSE 50%");
                leftMotor.setSpeed(-50);
                break;

            case 7:
                Serial.println("Motor: REVERSE 100%");
                leftMotor.setSpeed(-100);
                break;

            case 8:
                Serial.println("Motor: STOP");
                leftMotor.setSpeed(0);
                break;
        }

        motorTestStep++;

        if (motorTestStep > 8)
        {
            motorTestStep = 0;
        }
    };

    auto rightMotorCallback = []()
    {
        switch (motorTestStep)
        {
            case 0:
                Serial.println("Motor: STOP");
                rightMotor.setSpeed(0);
                break;

            case 1:
                Serial.println("Motor: FORWARD 25%");
                rightMotor.setSpeed(25);
                break;

            case 2:
                Serial.println("Motor: FORWARD 50%");
                rightMotor.setSpeed(50);
                break;

            case 3:
                Serial.println("Motor: FORWARD 100%");
                rightMotor.setSpeed(100);
                break;

            case 4:
                Serial.println("Motor: STOP");
                rightMotor.setSpeed(0);
                break;

            case 5:
                Serial.println("Motor: REVERSE 25%");
                rightMotor.setSpeed(-25);
                break;

            case 6:
                Serial.println("Motor: REVERSE 50%");
                rightMotor.setSpeed(-50);
                break;

            case 7:
                Serial.println("Motor: REVERSE 100%");
                rightMotor.setSpeed(-100);
                break;

            case 8:
                Serial.println("Motor: STOP");
                rightMotor.setSpeed(0);
                break;
        }

        motorTestStep++;

        if (motorTestStep > 8)
        {
            motorTestStep = 0;
        }
    };


    // --------------------------------------------------
    // Scheduler
    // --------------------------------------------------
    Timing::Scheduler scheduler;

    // --------------------------------------------------
    // Tasks
    // --------------------------------------------------

    // led tasks
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

    // servo tasks
    Timing::Scheduler::Task servoTask {
        servo0Callback,
        2000, // interval duration: 2000 ms (2 s)
        0
    };

    // motor tasks
    Timing::Scheduler::Task leftMotorTask {
        leftMotorCallback,
        2000,
        0
    };

    Timing::Scheduler::Task rightMotorTask {
        rightMotorCallback,
        2000,
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
        // PWM Servo 
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
        // PWM Motors 
        // --------------------------------------------------

        // left motor forward PWM
        PWMRequirements leftMotorForwardPwmRequirements
        {
            Board::LEFT_A_MOTOR_FORWARD_PIN,
            leftMotorConfig.pwmFrequency,
            leftMotorConfig.pwmResolution
        };

        leftMotorForwardPwmHandle =
            pwmResourceManager.allocate(leftMotorForwardPwmRequirements);

        if (pwmResourceManager.validate(leftMotorForwardPwmHandle))
        {
            PWMConfig leftMotorForwardPwmConfig
            {
                leftMotorForwardPwmHandle.resourceId,
                leftMotorForwardPwmRequirements.resolution
            };

            leftPwmForwardMotor =
                std::make_unique<PWM>(leftMotorForwardPwmConfig);

            leftPwmForwardMotor->configure(
                leftMotorForwardPwmRequirements.pin,
                leftMotorForwardPwmRequirements.frequency
            );
        }

        // left motor backward PWM
        PWMRequirements leftMotorBackwardPwmRequirements
        {
            Board::LEFT_A_MOTOR_BACKWARD_PIN,
            leftMotorConfig.pwmFrequency,
            leftMotorConfig.pwmResolution
        };

        leftMotorBackwardPwmHandle =
            pwmResourceManager.allocate(leftMotorBackwardPwmRequirements);

        if (pwmResourceManager.validate(leftMotorBackwardPwmHandle))
        {
            PWMConfig leftMotorBackwardPwmConfig
            {
                leftMotorBackwardPwmHandle.resourceId,
                leftMotorBackwardPwmRequirements.resolution
            };

            leftPwmBackwardMotor =
                std::make_unique<PWM>(leftMotorBackwardPwmConfig);

            leftPwmBackwardMotor->configure(
                leftMotorBackwardPwmRequirements.pin,
                leftMotorBackwardPwmRequirements.frequency
            );
        }

        // Give both PWM controllers to the left motor
        if (pwmResourceManager.validate(leftMotorForwardPwmHandle) &&
            pwmResourceManager.validate(leftMotorBackwardPwmHandle))
        {
            leftMotor.setPWM(
                *leftPwmForwardMotor,
                *leftPwmBackwardMotor,
                pwmResourceManager,
                leftMotorForwardPwmHandle,
                leftMotorBackwardPwmHandle
            );

            leftMotor.initialize();
        }

        // right motor forward PWM
        PWMRequirements rightMotorForwardPwmRequirements
        {
            Board::RIGHT_B_MOTOR_FORWARD_PIN,
            rightMotorConfig.pwmFrequency,
            rightMotorConfig.pwmResolution
        };

        rightMotorForwardPwmHandle =
            pwmResourceManager.allocate(rightMotorForwardPwmRequirements);

        if (pwmResourceManager.validate(rightMotorForwardPwmHandle))
        {
            PWMConfig rightMotorForwardPwmConfig
            {
                rightMotorForwardPwmHandle.resourceId,
                rightMotorForwardPwmRequirements.resolution
            };

            rightPwmForwardMotor =
                std::make_unique<PWM>(rightMotorForwardPwmConfig);

            rightPwmForwardMotor->configure(
                rightMotorForwardPwmRequirements.pin,
                rightMotorForwardPwmRequirements.frequency
            );
        }

        // right motor backward PWM
        PWMRequirements rightMotorBackwardPwmRequirements
        {
            Board::RIGHT_B_MOTOR_BACKWARD_PIN,
            rightMotorConfig.pwmFrequency,
            rightMotorConfig.pwmResolution
        };

        rightMotorBackwardPwmHandle =
            pwmResourceManager.allocate(rightMotorBackwardPwmRequirements);

        if (pwmResourceManager.validate(rightMotorBackwardPwmHandle))
        {
            PWMConfig rightMotorBackwardPwmConfig
            {
                rightMotorBackwardPwmHandle.resourceId,
                rightMotorBackwardPwmRequirements.resolution
            };

            rightPwmBackwardMotor =
                std::make_unique<PWM>(rightMotorBackwardPwmConfig);

            rightPwmBackwardMotor->configure(
                rightMotorBackwardPwmRequirements.pin,
                rightMotorBackwardPwmRequirements.frequency
            );
        }

        // Give both PWM controllers to the right motor
        if (pwmResourceManager.validate(rightMotorForwardPwmHandle) &&
            pwmResourceManager.validate(rightMotorBackwardPwmHandle))
        {
            rightMotor.setPWM(
                *rightPwmForwardMotor,
                *rightPwmBackwardMotor,
                pwmResourceManager,
                rightMotorForwardPwmHandle,
                rightMotorBackwardPwmHandle
            );

            rightMotor.initialize();
        }

        // --------------------------------------------------
        // Scheduler tasks
        // --------------------------------------------------
        scheduler.addTask(heartbeatTask);
        scheduler.addTask(fastFlashTask);
        scheduler.addTask(statusPrintTask);
        scheduler.addTask(pwmTask0);
        scheduler.addTask(servoTask);
        scheduler.addTask(leftMotorTask);
        scheduler.addTask(rightMotorTask);
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