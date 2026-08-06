# Phase 2 — GPIO Bring-Up

**Status:** ✅ Complete

---

# Overview

Phase 2 marks the first milestone where the firmware architecture interacts with physical hardware.

The primary objective of this phase was not simply to blink an LED or read a button. Instead, the goal was to validate that the layered firmware architecture designed in Phase 1 could successfully communicate with real hardware while maintaining clear separation of responsibilities.

To accomplish this, a reusable GPIO Hardware Abstraction Layer (HAL) was implemented along with simple device drivers for an LED and a push button. These modules were integrated through the Application layer and verified on an ESP32 development board.

Completing this milestone demonstrates that the firmware architecture is functional, scalable, and ready to support more advanced peripherals in future phases.

---

# Objectives

The goals for Phase 2 were:

* Implement a reusable GPIO Hardware Abstraction Layer.
* Create an LED device driver.
* Create a Button device driver.
* Introduce board-specific GPIO configuration.
* Validate the layered firmware architecture on physical hardware.
* Practice systematic firmware testing and debugging.

---

# Hardware

## Components

* Freenove ESP32-WROVER
* Breadboard
* External LED
* 220 Ω resistor
* Push button
* Jumper wires

---

## Wiring

### LED

```text
GPIO2
  │
220 Ω
  │
LED (+)
LED (-)
  │
 GND
```

The LED was connected as an active-high output.

---

### Button

```text
GPIO15
   │
Button
   │
 GND
```

The button uses the ESP32's internal pull-up resistor (`INPUT_PULLUP`), eliminating the need for an external resistor.

Electrical behavior:

* Released → HIGH
* Pressed → LOW

The Button driver converts this into a logical `true` when pressed.

---

# Firmware Architecture

Phase 2 populated the architecture introduced in Phase 1.

```text
main.cpp
        │
        ▼
Application
        │
        ▼
Device Drivers
 ├── LED
 └── Button
        │
        ▼
GPIO HAL
        │
        ▼
Arduino Framework
        │
        ▼
ESP32 Hardware
```

Each layer owns a distinct responsibility.

---

# Module Responsibilities

## main.cpp

Responsibilities:

* Initialize the Serial interface.
* Create the Application object.
* Start the firmware.
* Execute the application update loop.

No application logic is implemented inside `main.cpp`.

---

## Application

Responsibilities:

* Initialize firmware modules.
* Coordinate device interaction.
* Implement robot behavior.

Current behavior:

* Read button state.
* Turn LED on while the button is pressed.
* Turn LED off when released.

The Application communicates only with device drivers and has no knowledge of GPIO operations.

---

## LED Driver

Responsibilities:

* Initialize LED hardware.
* Turn LED on.
* Turn LED off.
* Toggle LED state.

The LED module hides all GPIO implementation details behind a simple interface.

---

## Button Driver

Responsibilities:

* Configure the button input.
* Read button state.
* Convert the electrical signal into a logical button press.

Because the ESP32 uses an internal pull-up resistor, the driver inverts the GPIO reading so higher-level code can simply ask whether the button is pressed.

---

## GPIO Hardware Abstraction Layer

Responsibilities:

* Configure GPIO direction.
* Read digital inputs.
* Write digital outputs.

This layer is the only firmware module that directly calls Arduino GPIO functions.

Higher-level modules remain independent of the underlying framework.

---

## Board Configuration

Board-specific information is centralized within the configuration module.

Current configuration includes:

* Status LED GPIO
* User button GPIO

Future hardware revisions should require only updates to the configuration file instead of changes throughout the firmware.

---

# Directory Structure

```text
src/
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
├── hal/
│   └── gpio/
│       ├── gpio.cpp
│       └── gpio.h
│
└── main.cpp
```

---

# Engineering Decisions

## Hardware Abstraction

Rather than allowing application code to call `digitalWrite()` or `digitalRead()` directly, all GPIO access is routed through the Hardware Abstraction Layer.

This isolates hardware-specific implementation details from higher-level software.

Benefits include:

* Improved portability
* Better modularity
* Easier testing
* Cleaner interfaces

---

## Device Drivers

Physical components are represented as reusable C++ classes.

Instead of manipulating GPIO pins directly, application code interacts with behaviors such as:

* `led.on()`
* `led.off()`
* `button.isPressed()`

This approach improves readability and keeps application logic independent of electrical implementation.

---

## Minimal main.cpp

The firmware entry point intentionally contains almost no logic.

Its responsibilities are limited to:

* Hardware initialization
* Application startup
* Executing the update loop

This keeps the program entry point easy to understand and prevents architectural drift as the project grows.

---

## Board Configuration

GPIO assignments are centralized inside the board configuration module.

This prevents pin numbers from being scattered throughout the firmware and simplifies future hardware revisions.

---

# Testing Strategy

Hardware validation was performed incrementally.

## Build Test

* Firmware compiled successfully using PlatformIO.

---

## Flash Test

* Firmware uploaded successfully to the ESP32.

---

## Boot Test

* Firmware initialized correctly.
* Application startup completed without errors.

---

## LED Test

The LED driver was tested independently.

Expected behavior:

* LED initializes in the OFF state.
* LED responds correctly to software commands.

Result:

✅ Passed

---

## Button Test

The button driver was validated using the internal pull-up resistor.

Expected behavior:

* Released → not pressed
* Pressed → pressed

Result:

✅ Passed

---

## Integration Test

The complete firmware stack was tested.

Expected behavior:

* Press button → LED turns on.
* Release button → LED turns off.

Result:

✅ Passed

This confirmed that all firmware layers were communicating correctly.

---

# Debugging

During development several debugging techniques were used:

* PlatformIO compiler diagnostics
* Serial output
* Incremental hardware testing
* Module-by-module validation

One notable issue encountered was a namespace conflict with the ESP32 framework when using the name `GPIO` for the abstraction layer. The issue was resolved by renaming the namespace to avoid colliding with symbols defined by the ESP32 SDK.

This reinforced the importance of choosing clear and unique namespace names in embedded software.

---

# Lessons Learned

Phase 2 introduced several important embedded systems concepts.

Topics learned include:

* GPIO fundamentals
* Digital inputs
* Digital outputs
* Internal pull-up resistors
* Hardware abstraction
* Device driver design
* Board configuration
* Layered firmware architecture
* Hardware bring-up
* Systematic embedded debugging

Most importantly, this milestone demonstrated that firmware development extends beyond writing code. Successful embedded development requires hardware validation, structured debugging, and careful architectural design.

---

# Deliverables

By the end of Phase 2 the firmware included:

* GPIO Hardware Abstraction Layer
* LED device driver
* Button device driver
* Board configuration module
* Application layer integration
* Successful hardware validation
* Modular firmware architecture verified on real hardware

---

# Future Work

The next milestone introduces timing concepts.

Upcoming topics include:

* `millis()`
* Blocking vs. non-blocking code
* Cooperative scheduling
* Periodic task execution

These concepts will allow the firmware to manage multiple independent activities simultaneously without relying on blocking delays.

---

# Milestone Summary

**Status:** ✅ Complete

## Completed

* GPIO Hardware Abstraction Layer
* LED driver
* Button driver
* Board configuration
* Hardware bring-up
* ESP32 hardware validation
* Layered firmware verification
* Embedded debugging experience

## Next Milestone

**Phase 3 — Timing**

The next phase will introduce non-blocking firmware design using `millis()` and cooperative scheduling. These techniques will form the foundation for responsive robot behavior and prepare the firmware for servos, sensors, communication interfaces, and future real-time software architecture.
