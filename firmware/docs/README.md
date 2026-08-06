# Phase 2 — GPIO Bring-Up

## Overview

The goal of this milestone was to establish the first professional firmware architecture for the Candy Cadet project while validating communication between the ESP32 and simple digital hardware.

Rather than creating a single Arduino sketch, this phase focused on building a modular firmware architecture that separates hardware access, device drivers, and application logic. The result is a reusable foundation that future firmware features—including PWM, sensors, communication, and state machines—can build upon.

---

## Objectives

* Create a reusable GPIO Hardware Abstraction Layer (HAL)
* Separate hardware-specific code from application logic
* Implement reusable LED and Button device drivers
* Validate firmware architecture using real hardware
* Test the firmware on an ESP32 development board

---

## Hardware

### Components

* Freenove ESP32-WROVER
* Breadboard
* External LED
* 220 Ω resistor
* Push button
* Jumper wires

### Wiring

#### LED

* GPIO2 → 220 Ω resistor → LED anode (+)
* LED cathode (−) → GND

#### Button

* GPIO15 → Push button → GND

The button uses the ESP32's internal pull-up resistor (`INPUT_PULLUP`), so no external resistor is required.

---

## Firmware Architecture

```text
main.cpp
    │
    ▼
Application Layer
    │
    ▼
Device Layer
 ├── LED
 └── Button
    │
    ▼
Hardware Abstraction Layer (HAL)
    │
    ▼
Arduino Framework
    │
    ▼
ESP32 Hardware
```

### Layer Responsibilities

#### main.cpp

* Entry point of the firmware
* Initializes the system
* Starts the application
* Runs the main update loop

#### Application Layer

Responsible for robot behavior.

For this milestone:

* Initialize the LED
* Initialize the button
* Read the button state
* Control the LED

#### Device Layer

Represents hardware devices.

**LED**

* Initialize LED
* Turn on
* Turn off
* Toggle

**Button**

* Initialize button
* Detect button presses

#### Hardware Abstraction Layer (HAL)

Provides a reusable interface for GPIO operations.

Responsibilities:

* Configure pins
* Read digital inputs
* Write digital outputs

This layer is the only part of the firmware that directly interacts with the Arduino GPIO API.

#### Board Configuration

The board configuration file contains hardware-specific information, including:

* Status LED GPIO
* User button GPIO

Changing hardware connections only requires updating this configuration instead of modifying application code.

---

## Directory Structure

```text
src/
│
├── main.cpp
│
├── app/
│   ├── application.cpp
│   └── application.h
│
├── config/
│   └── board_config.h
│
├── devices/
│   ├── led/
│   │   ├── led.cpp
│   │   └── led.h
│   │
│   └── button/
│       ├── button.cpp
│       └── button.h
│
└── hal/
    └── gpio/
        ├── gpio.cpp
        └── gpio.h
```

---

## Validation Tests

### Build Test

* Firmware compiled successfully using PlatformIO.

### Flash Test

* Firmware successfully uploaded to the ESP32.

### GPIO Output Test

* LED initialized correctly.
* LED responded to software commands.

### GPIO Input Test

* Button state correctly detected using the internal pull-up resistor.

### Integration Test

* Pressing the button turned the LED on.
* Releasing the button turned the LED off.

All tests passed successfully on hardware.

---

## Engineering Decisions

### Hardware Abstraction Layer

A GPIO HAL was introduced to isolate the rest of the firmware from the Arduino API. This allows higher-level modules to work with generic GPIO operations instead of directly calling `pinMode()`, `digitalRead()`, and `digitalWrite()`.

### Device Drivers

The LED and Button were implemented as reusable device drivers. These modules expose behavior instead of hardware details, allowing the application layer to remain focused on robot logic.

### Separation of Concerns

Each module has a single responsibility:

* Board configuration describes the hardware.
* HAL communicates with the microcontroller.
* Device drivers represent physical components.
* Application coordinates device behavior.
* `main.cpp` serves only as the program entry point.

This separation improves readability, maintainability, and scalability.

---

## Lessons Learned

During this milestone I learned:

* How to organize a professional embedded firmware project.
* Why hardware abstraction layers are important.
* How to separate application logic from hardware-specific code.
* How to build reusable device drivers.
* How to validate firmware using real hardware.
* How to debug GPIO input and output on an ESP32.
* How modular firmware architecture scales better than a single Arduino sketch.

---

## Next Milestone

**Phase 3 — Timing**

The next phase will replace blocking programming techniques with non-blocking timing using `millis()`. This will introduce cooperative scheduling and prepare the firmware for managing multiple tasks simultaneously, laying the groundwork for servos, sensors, communication, and more advanced robot behaviors.
