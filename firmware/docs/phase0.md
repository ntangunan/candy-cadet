# Phase 0 — Development Environment

**Status:** ✅ Complete

---

# Overview

Phase 0 established the development environment for the Candy Cadet firmware project.

Before implementing any embedded software, a professional firmware project requires a reliable toolchain, version control system, build process, and repository organization. Investing time in this foundation helps prevent future development issues and creates a workflow that scales as the project grows.

The objective of this phase was not to build robot functionality, but to create an environment that supports professional firmware development.

---

# Objectives

The goals for Phase 0 were:

* Configure Visual Studio Code for embedded development.
* Install and configure PlatformIO.
* Create a firmware repository using Git.
* Verify the build system.
* Flash firmware to the ESP32.
* Verify serial communication.
* Establish the initial project directory structure.
* Prepare the project for future modular firmware development.

---

# Development Environment

## Hardware

* Freenove ESP32-WROVER
* USB cable
* MacBook Pro
* Breadboard (used in later phases)

---

## Software

* Visual Studio Code
* PlatformIO
* Git
* Arduino Framework for ESP32

---

# PlatformIO

Although the Arduino IDE is useful for beginners, it becomes difficult to manage larger firmware projects.

PlatformIO provides:

* Dependency management
* Multiple build configurations
* Professional project organization
* Integrated debugging support
* Library management
* Reproducible builds
* Version-controlled project configuration

Using PlatformIO better reflects the workflows used in professional embedded software teams.

---

# Repository Structure

During Phase 0 the initial repository was created.

```text
firmware/
│
├── include/
├── lib/
├── src/
├── test/
├── platformio.ini
└── README.md
```

At this stage only the default PlatformIO structure was required.

Additional directories would be introduced during later milestones as the firmware architecture evolved.

---

# Build Verification

The initial firmware project was successfully compiled using PlatformIO.

Successful compilation confirmed:

* Compiler installation
* ESP32 toolchain configuration
* Arduino framework installation
* PlatformIO project configuration

---

# Firmware Upload

The firmware was successfully uploaded to the ESP32 development board.

This verified:

* USB communication
* Board configuration
* Flash programming
* Firmware boot

---

# Serial Communication

The Serial Monitor was configured and tested.

Serial output provides one of the most important debugging tools available during early embedded development.

During future phases it will be used to:

* Report initialization status
* Display sensor values
* Verify communication
* Diagnose hardware issues
* Debug firmware behavior

---

# Git Workflow

Git was introduced from the beginning of development.

Each milestone is intended to represent a stable firmware state.

Typical workflow:

1. Implement feature
2. Compile firmware
3. Flash to hardware
4. Test functionality
5. Fix issues
6. Commit completed milestone

This approach creates a clear development history and allows previous working versions to be restored if necessary.

---

# Engineering Decisions

## Build Before Features

Rather than immediately writing robot code, the first priority was establishing a reliable build and deployment workflow.

Without confidence in the development environment, debugging firmware becomes significantly more difficult.

---

## Use Version Control Immediately

Version control was introduced before significant firmware development began.

This encourages disciplined engineering practices and documents the evolution of the project over time.

---

## Establish a Professional Workflow

The project was intentionally developed using professional tools rather than relying on beginner-focused workflows.

This decision supports long-term maintainability and provides experience with industry-standard embedded development practices.

---

# Testing

The following tests were completed successfully.

## Build Test

* Firmware compiled successfully.

## Flash Test

* Firmware uploaded successfully.

## Boot Test

* ESP32 executed uploaded firmware.

## Serial Test

* Serial communication initialized successfully.

All tests passed.

---

# Lessons Learned

During this phase the following concepts were introduced:

* Embedded development toolchains
* PlatformIO project management
* Firmware compilation
* Flashing embedded software
* Serial communication
* Version control
* Repository organization

More importantly, this phase demonstrated that embedded software development begins with establishing a reliable engineering workflow before implementing hardware functionality.

---

# Deliverables

By the end of Phase 0 the project contained:

* Configured PlatformIO environment
* Git repository
* Functional build system
* Working firmware upload process
* Serial communication
* Initial firmware repository structure

This provided the foundation for all future firmware development.

---

# Future Work

The next milestone introduces the firmware architecture used throughout the remainder of the project.

Future work includes:

* Modular directory organization
* Application layer
* Hardware Abstraction Layer (HAL)
* Device drivers
* Board configuration
* Separation of concerns

These changes will transform the project from a simple PlatformIO application into a scalable embedded firmware codebase.

---

# Milestone Summary

**Status:** ✅ Complete

## Completed

* PlatformIO configured
* ESP32 toolchain verified
* Git repository created
* Firmware build verified
* Firmware upload verified
* Serial communication tested
* Initial project structure established

## Next Milestone

**Phase 1 — Firmware Architecture**

The next phase focuses on designing a modular firmware architecture that separates hardware access, application logic, and device-specific behavior, creating a foundation that can scale with the Candy Cadet robot.
