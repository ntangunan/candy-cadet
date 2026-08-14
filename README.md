# Candy Cadet 🤖

An experimental animatronic robot inspired by *Five Nights at Freddy's: Pizzeria Simulator*, built to explore embedded systems, robotics, computer vision, artificial intelligence, and custom hardware.

Candy Cadet is a long-term engineering project focused on designing and building a functional robotic system from the ground up. The project combines mechanical design, electronics, embedded firmware, control systems, computer vision, and software engineering into a single platform.

The goal is not simply to build a working robot, but to use the project as a hands-on environment for developing professional engineering skills.

---

## What is Candy Cadet?

Candy Cadet is an animatronic robotic platform designed to eventually operate as a semi-autonomous, interactive robot.

The robot is being developed around several major engineering systems:

* Embedded firmware
* Custom electronics
* Motor control
* Self-balancing
* Sensors
* LEDs and other actuators
* Computer vision
* Artificial intelligence
* Mechanical design
* Communication between hardware and software

Development is intentionally incremental. Each subsystem is developed and tested independently before being integrated into the larger robot.

---

# Candy Cadet Firmware

This repository contains the embedded firmware responsible for controlling Candy Cadet's hardware.

The firmware runs on an **ESP32** and provides the software foundation for the robot's real-time hardware control.

The firmware is being developed using professional embedded-systems practices, including:

* Layered architecture
* Hardware abstraction
* Modular drivers
* Cooperative scheduling
* Callback-based task execution
* State machines
* Sensor abstraction
* Actuator control
* Safety systems
* Embedded debugging
* Maintainable C++ code
* Professional Git workflows

The firmware is being developed incrementally, starting with fundamental hardware control and gradually building toward a complete robotics control system.

---

## Hardware Platform

### Current

* ESP32
* PlatformIO
* C++
* Arduino Framework
* LEDs
* Push buttons
* GPIO-based hardware control

### Planned

* DC motors
* Motor driver
* Wheel encoders
* IMU
* Additional sensors
* Servos
* Custom PCBs
* Audio hardware
* Wireless communication
* Custom LED assemblies

The final robot will combine these systems into a self-contained robotic platform.

---

## System Architecture

Candy Cadet is being designed as a layered system.

```text
┌───────────────────────────────┐
│        Robot Behavior         │
│   State Machines / AI / CV    │
├───────────────────────────────┤
│       Application Layer       │
│     Robot Control Logic       │
├───────────────────────────────┤
│          Services             │
│ Scheduler / Communication     │
├───────────────────────────────┤
│        Hardware Drivers       │
│ LED / Button / Motor / Sensor │
├───────────────────────────────┤
│      Hardware Abstraction     │
│             HAL               │
├───────────────────────────────┤
│            ESP32              │
└───────────────────────────────┘
```

This architecture allows hardware-specific implementation to remain separated from higher-level robot behavior.

---

## Repository Structure

```text
CandyCadet-Firmware/
├── docs/
├── include/
├── lib/
├── src/
├── test/
├── .github/
├── platformio.ini
└── README.md
```

### Source Layout

```text
src/
├── app/
├── config/
├── devices/
├── hal/
├── services/
└── main.cpp
```

The exact structure will evolve as the firmware grows.

---

## Current Development Status

### Phase 0 — Development Environment ✅

* PlatformIO project created
* VS Code development environment configured
* ESP32 firmware successfully built
* Firmware successfully uploaded
* Serial communication established
* Git repository established

### Phase 1 — GPIO & Hardware Abstraction ✅

* GPIO abstraction implemented
* LED driver implemented
* Button driver implemented
* Board pin configuration established
* Hardware-specific logic separated from application code

### Phase 2 — Timing & Scheduling 🚧

Current work focuses on building a reusable software timing system.

Implemented:

* Independent software timers
* Non-blocking timing using `millis()`
* Multiple concurrent timed tasks
* Callback-based task execution
* Lambda callbacks
* Callback capture by value
* Callback capture by reference

The goal is to eventually replace application-specific timing logic with a reusable scheduler.

---

## Roadmap

### Firmware

* [x] Development environment
* [x] GPIO abstraction
* [x] LED driver
* [x] Button driver
* [x] Independent software timers
* [x] Callback system
* [ ] Scheduler
* [ ] PWM
* [ ] Motor driver
* [ ] UART communication
* [ ] Sensor framework
* [ ] State machines
* [ ] Safety system
* [ ] FreeRTOS
* [ ] Automated testing

### Robotics

* [ ] Motorized drivetrain
* [ ] Two-wheel self-balancing system
* [ ] IMU integration
* [ ] Encoder feedback
* [ ] Closed-loop motor control
* [ ] PID control
* [ ] Autonomous movement
* [ ] Custom electronics
* [ ] Custom PCB
* [ ] LED system
* [ ] Audio system

### Computer Vision & AI

* [ ] Camera integration
* [ ] OpenCV pipeline
* [ ] Object detection
* [ ] Face/person detection
* [ ] Robot perception system
* [ ] AI-driven behavior

---

## Development Philosophy

Candy Cadet is being developed as an engineering learning platform.

Rather than building the entire robot at once, the project is divided into smaller systems that can be designed, tested, and understood independently.

The primary principles are:

* **Understand before abstracting**
* **Build incrementally**
* **Separate hardware from behavior**
* **Prefer modular systems**
* **Avoid unnecessary complexity**
* **Test hardware independently**
* **Document engineering decisions**
* **Use professional development practices**

The objective is to develop firmware and hardware that can eventually support a complex robotic system while maintaining a codebase that is understandable, testable, and maintainable.

---

## Development Tools

### Software

* C++
* PlatformIO
* Visual Studio Code
* Git
* GitHub

### Embedded

* ESP32
* Arduino Framework
* Serial debugging
* GPIO
* PWM
* UART
* SPI
* I²C

### Robotics

* Arduino
* Embedded C++
* PID control
* Sensors
* Motor control
* Computer vision
* OpenCV

---

## Building the Firmware

### Requirements

* Visual Studio Code
* PlatformIO
* Git
* ESP32 development board

### Build

```bash
pio run
```

### Upload

```bash
pio run --target upload
```

### Serial Monitor

```bash
pio device monitor
```

---

## Documentation

Project documentation is located in the `docs/` directory.

Documentation will include:

* Firmware architecture
* Hardware architecture
* Engineering logs
* Development roadmap
* Architecture decision records
* Hardware documentation
* Testing documentation

---

## Project Status

🚧 **Active Development**

Candy Cadet is an ongoing engineering project. Hardware and software architecture will continue to evolve as new systems are designed and integrated.

---

## License

This project is licensed under the MIT License.
