# Phase 1 — Firmware Architecture

**Status:** ✅ Complete

---

# Overview

Phase 1 established the software architecture that will support the entire Candy Cadet firmware project.

Rather than writing application logic directly inside `main.cpp`, this phase focused on creating a layered architecture that separates hardware access, device behavior, and application logic. This organization improves readability, maintainability, scalability, and testability as the project grows.

The objective of this milestone was to create a professional firmware skeleton that can support future peripherals, communication interfaces, and robot behaviors without requiring major architectural changes.

---

# Objectives

The goals for Phase 1 were:

* Design a modular firmware architecture.
* Minimize the responsibilities of `main.cpp`.
* Separate application logic from hardware-specific code.
* Introduce a Hardware Abstraction Layer (HAL).
* Create a dedicated device layer.
* Centralize board-specific configuration.
* Establish a scalable directory structure.
* Prepare the project for future firmware modules.

---

# Firmware Architecture

The firmware follows a layered architecture where each layer has a single responsibility.

```text
Application Layer
        │
        ▼
Device Drivers
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

This architecture prevents application code from depending directly on hardware implementation details.

---

# Architecture Goals

The firmware architecture was designed around several engineering principles.

## Separation of Concerns

Each module owns one responsibility.

Examples:

* The application coordinates behavior.
* Device drivers represent physical hardware.
* The HAL communicates with the microcontroller.
* Configuration files describe the hardware.

Each layer depends only on the layer directly beneath it.

---

## Encapsulation

Hardware details remain hidden inside their respective modules.

Higher-level code should never manipulate GPIO registers or Arduino functions directly.

Instead, application code communicates with devices through well-defined interfaces.

---

## Scalability

The architecture is intended to support future robot features including:

* LEDs
* Buttons
* Servos
* Sensors
* UART communication
* PWM outputs
* State machines
* Safety systems
* FreeRTOS tasks

Adding new functionality should require creating new modules rather than modifying existing ones whenever possible.

---

# Project Structure

The project directory was reorganized to separate firmware responsibilities.

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
│
├── hal/
│
└── main.cpp
```

This structure will continue to expand throughout the project while maintaining the same architectural principles.

---

# Module Responsibilities

## main.cpp

Responsibilities:

* Initialize the firmware.
* Create the application object.
* Execute the application update loop.

The goal is to keep `main.cpp` as small as possible.

Example responsibility:

```text
setup()
    ↓
Application::initialize()

loop()
    ↓
Application::update()
```

Business logic should never be placed directly inside `main.cpp`.

---

## Application Layer

The Application module represents the highest layer of the firmware.

Responsibilities include:

* System initialization
* Robot behavior
* Coordination between devices
* Future state management

The Application does not communicate directly with hardware.

Instead, it interacts with reusable device modules.

---

## Device Layer

The Device layer represents physical components attached to the robot.

Examples include:

* LED
* Button
* Servo
* Speaker
* Sensors

Each device exposes meaningful behavior rather than hardware-specific operations.

For example:

```text
LED
 ├── initialize()
 ├── on()
 ├── off()
 └── toggle()
```

instead of exposing GPIO functions directly.

---

## Hardware Abstraction Layer

The HAL provides reusable interfaces for microcontroller peripherals.

Future HAL modules will include:

* GPIO
* PWM
* UART
* SPI
* I2C
* ADC

Only the HAL communicates directly with the Arduino framework.

---

## Board Configuration

Board-specific information is centralized inside the configuration module.

Examples include:

* GPIO assignments
* Hardware constants
* Future peripheral configuration

This allows hardware revisions without modifying application logic.

---

# Engineering Decisions

## Keep main.cpp Minimal

One of the primary architectural goals was minimizing the responsibilities of `main.cpp`.

This approach improves readability and mirrors the structure used in larger embedded software projects.

---

## Design for Growth

Although the firmware currently supports only simple hardware, the architecture was intentionally designed to support future complexity.

No architectural changes should be required when additional devices are introduced.

Instead, new modules should integrate naturally into the existing structure.

---

## Favor Composition Over Monolithic Design

Rather than placing all firmware inside a single source file, functionality is divided into focused modules.

This reduces coupling and improves maintainability.

---

# Testing

Since this milestone focused primarily on architecture, testing emphasized software organization rather than hardware functionality.

Validation included:

* Project builds successfully.
* Directory structure compiles correctly.
* Header dependencies resolve correctly.
* Application object initializes successfully.
* Firmware executes without errors.

These tests confirmed that the architecture supports successful compilation and execution.

---

# Lessons Learned

This milestone introduced several important embedded software concepts.

Topics learned include:

* Layered firmware architecture
* Separation of concerns
* Hardware abstraction
* Modular C++ organization
* Interface design
* Project organization
* Configuration management

More importantly, this phase demonstrated that embedded software architecture should be established before implementing hardware functionality.

A well-designed architecture significantly reduces future complexity.

---

# Deliverables

By the end of Phase 1 the firmware contained:

* Modular project structure
* Application layer
* Hardware Abstraction Layer directory
* Device layer directory
* Configuration module
* Minimal `main.cpp`
* Scalable software architecture

This architecture provides the foundation for every future firmware subsystem.

---

# Future Work

The next milestone introduces the first hardware abstraction module.

Future work includes:

* GPIO driver
* LED device
* Button device
* Hardware validation
* GPIO testing
* Board bring-up

This will represent the first interaction between the firmware architecture and physical hardware.

---

# Milestone Summary

**Status:** ✅ Complete

## Completed

* Layered firmware architecture
* Modular project organization
* Application layer
* Device layer
* HAL organization
* Board configuration module
* Minimal `main.cpp`
* Scalable firmware foundation

## Next Milestone

**Phase 2 — GPIO Bring-Up**

The next phase implements the project's first reusable Hardware Abstraction Layer, introduces LED and Button device drivers, and validates the architecture using real ESP32 hardware.
