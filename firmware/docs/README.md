# Candy Cadet Firmware

Embedded firmware for the **Candy Cadet** robotics project.

This repository contains the firmware that runs on the ESP32 microcontroller responsible for low-level hardware control, device management, and real-time robot behavior.

This firmware is designed using professional embedded software engineering practices, emphasizing modularity, hardware abstraction, maintainability, scalability, and testability.

---

# Purpose

The firmware serves two primary purposes:

1. Control the hardware that powers Candy Cadet.
2. Serve as a learning platform for professional embedded firmware development.

The project is intentionally built using engineering practices commonly found in robotics and embedded software teams rather than focusing solely on making the robot function.

---

# Project Goals

The firmware is being developed to gain practical experience with:

* Embedded software architecture
* Hardware abstraction layers (HAL)
* Modular C++ design
* GPIO
* Timers
* PWM
* UART
* SPI
* I2C
* ADC
* State machines
* Cooperative scheduling
* Real-time programming
* Embedded debugging
* Power management
* Firmware testing
* Documentation
* Professional Git workflows

Every milestone builds toward creating firmware that is clean, reusable, and easy to extend as the robot becomes more capable.

---

# Development Philosophy

The firmware follows several guiding principles.

## Separation of Concerns

Each module owns a single responsibility.

Application logic should not manipulate hardware directly.

Hardware-specific code should remain isolated inside the Hardware Abstraction Layer.

---

## Hardware Abstraction

Application code should never call Arduino functions such as:

* `pinMode()`
* `digitalWrite()`
* `digitalRead()`

Instead, higher-level modules communicate with reusable hardware interfaces.

---

## Modular Design

Each physical device is represented by its own module.

Examples include:

* LED
* Button
* Servo
* Sensors
* Motors

This makes devices reusable and easier to test independently.

---

## Readability

Firmware should prioritize clarity over cleverness.

Code should be understandable months later without requiring extensive explanation.

---

## Scalability

The firmware architecture is intended to scale from a simple LED and button to a complete interactive animatronic robot with multiple sensors, actuators, communication interfaces, and behavioral systems.

---

# Firmware Architecture

```
Application
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

## Layer Responsibilities

### Application Layer

Responsible for robot behavior.

Examples:

* Initialize devices
* Coordinate subsystems
* Control robot logic
* Execute state transitions

---

### Device Layer

Represents physical hardware.

Each module exposes behaviors rather than GPIO operations.

Examples:

* LED
* Button
* Servo
* Sensor

---

### Hardware Abstraction Layer

Provides reusable interfaces to ESP32 peripherals.

Examples:

* GPIO
* PWM
* UART
* ADC
* SPI
* I2C

Only the HAL communicates directly with the Arduino framework.

---

### Board Configuration

The board configuration centralizes hardware-specific information such as GPIO assignments.

Changing wiring should require modifying only the board configuration rather than application logic.

---

# Directory Structure

```
firmware/
│
├── README.md
│
├── docs/
│   ├── architecture.md
│   ├── phase0.md
│   ├── phase1.md
│   ├── phase2.md
│   ├── phase3.md
│   ├── phase4.md
│   ├── phase5.md
│   ├── phase6.md
│   ├── phase7.md
│   ├── phase8.md
│   ├── phase9.md
│   └── phase10.md
│
├── include/
│
├── lib/
│
├── src/
│   ├── app/
│   ├── config/
│   ├── devices/
│   ├── hal/
│   └── main.cpp
│
├── test/
│
└── platformio.ini
```

---

# Current Progress

## Phase 0 — Development Environment

**Status:** ✅ Complete

Completed:

* PlatformIO project setup
* VS Code configuration
* Git repository
* Build system verification
* Firmware upload
* Serial monitor
* Repository organization

---

## Phase 1 — Firmware Architecture

**Status:** ✅ Complete

Completed:

* Project directory structure
* Application layer
* Hardware Abstraction Layer
* Device layer
* Board configuration
* Modular firmware organization

---

## Phase 2 — GPIO Bring-Up

**Status:** ✅ Complete

Completed:

* GPIO HAL
* LED driver
* Button driver
* Board configuration
* Hardware validation
* ESP32 GPIO testing
* LED and button integration

The firmware was successfully validated on hardware using an external LED and push button.

---

## Remaining Roadmap

* Phase 3 — Timing
* Phase 4 — PWM
* Phase 5 — UART Communication
* Phase 6 — Sensors
* Phase 7 — State Machines
* Phase 8 — Actuator Framework
* Phase 9 — Safety
* Phase 10 — FreeRTOS

---

# Development Workflow

Every new feature follows the same engineering process.

```
Requirements
    ↓
Architecture
    ↓
Interface Design
    ↓
Implementation
    ↓
Compilation
    ↓
Hardware Validation
    ↓
Debugging
    ↓
Documentation
    ↓
Git Commit
```

No feature is considered complete until it has been validated on physical hardware.

---

# Testing Philosophy

Firmware is tested incrementally.

Each subsystem is validated independently before integration.

Typical testing sequence:

1. Build successfully
2. Flash firmware
3. Verify boot
4. Test individual hardware modules
5. Integrate modules
6. Validate complete behavior
7. Commit working milestone

This approach minimizes debugging complexity and isolates failures.

---

# Coding Principles

The firmware follows several design rules.

* Keep `main.cpp` minimal.
* Avoid blocking delays.
* Eliminate magic numbers.
* Prefer strongly typed interfaces.
* Encapsulate hardware details.
* Give every module one clear responsibility.
* Favor readability over clever optimizations.
* Design modules to be reusable.

---

# Documentation

Engineering documentation is maintained alongside the firmware.

The `docs/` directory contains:

* Architecture documentation
* Coding standards
* Testing procedures
* Development workflow
* Phase reports
* Design decisions

Each completed milestone includes its own report documenting:

* Objectives
* Architecture
* Hardware
* Testing
* Lessons learned
* Future improvements

---

# Long-Term Vision

The firmware will evolve from a simple GPIO demonstration into a complete embedded software platform capable of supporting an interactive animatronic robot.

Future capabilities include:

* Coordinated servo animation
* Sensor fusion
* UART communication
* Audio synchronization
* Robot state management
* Fault detection
* Watchdog recovery
* Real-time task scheduling using FreeRTOS

Throughout development, the emphasis remains on building firmware that reflects professional engineering practices rather than simply achieving functionality.
