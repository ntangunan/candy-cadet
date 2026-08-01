# Candy Cadet Firmware Architecture

## Purpose

This document describes the architecture of the Candy Cadet firmware and the engineering decisions behind it.

The goal of this project is not only to build a functioning robot, but also to learn professional embedded firmware development practices.

The firmware is designed with the following principles:

- Readability
- Modularity
- Maintainability
- Scalability
- Testability
- Separation of concerns

---

# Design Philosophy

Rather than placing all functionality into a single `main.cpp` file, the firmware is divided into modules.

Each module has a single responsibility.

Higher-level robot behavior should not depend directly on low-level hardware details.

Instead, hardware interactions are abstracted behind reusable interfaces.

---

# Firmware Layers

Application
    │
Services
    │
Hardware Abstraction Layer
    │
Arduino Framework
    │
ESP32 Hardware

Each layer depends only on the layer beneath it.

This minimizes coupling between components.

---

# Repository Structure

src/
    app/
    config/
    drivers/
    hal/
    services/

main.cpp

docs/

test/

lib/

include/

---

# Directory Responsibilities

## app/

Contains high-level robot behavior.

Examples:

- 
- 
- 

---

## drivers/

Contains hardware drivers for specific devices.

Examples:

- 
- 
- 

Drivers should control hardware but not decide robot behavior.

---

## hal/

Contains hardware abstraction for ESP32 peripherals.

Examples:

- 
- 
- 

This layer isolates hardware-specific implementation details.

---

## services/

Contains reusable services used across the application.

Examples:

- 
- 
- 

---

## config/

Contains centralized configuration.

Examples:

- 
- 
- 

Configuration values should not be duplicated throughout the project.

---

# main.cpp

The purpose of `main.cpp` is to coordinate the firmware.

Responsibilities include:

- 
- 
- 

Business logic should remain outside of `main.cpp`.

---

# Current Status

Phase 0

- Development environment
- Repository organization
- Initial architecture

Upcoming:

- GPIO
- Timing
- PWM
- UART
- Sensors
- State machines