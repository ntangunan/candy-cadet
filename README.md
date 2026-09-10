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

Development is intentionally incremental. Each subsystem is designed, implemented, tested, documented, and reviewed independently before being integrated into the larger robot.

---

# Candy Cadet Firmware

This repository contains the embedded firmware responsible for controlling Candy Cadet's hardware.

The firmware currently runs on a **Freenove ESP32-WROVER** using **PlatformIO, C++, and the Arduino framework**.

The firmware is being developed using professional embedded-systems practices, including:

* Layered architecture
* Hardware abstraction
* Modular device drivers
* Cooperative scheduling
* Callback-based task execution
* Configuration-driven hardware
* Reusable timing systems
* PWM-based actuator control
* Embedded debugging
* Maintainable C++ code
* Professional Git workflows
* Incremental development and testing

The firmware is being developed incrementally, starting with fundamental hardware control and gradually building toward a complete robotics control system.

---

## Hardware Platform

### Current

* Freenove ESP32-WROVER
* PlatformIO
* C++
* Arduino Framework
* LEDs
* Push buttons
* GPIO
* PWM
* Servo control
* Serial debugging

### Planned

* DC motors
* Motor driver
* Wheel encoders
* MPU6050 IMU
* Additional sensors
* Raspberry Pi 5 high-level computer
* Custom PCBs
* Audio hardware
* Wireless communication
* Custom LED assemblies

The final robot will combine these systems into a self-contained robotic platform.

---

# System Architecture

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

The architecture is intentionally being built from the bottom up.

Hardware-specific implementation is kept separate from application behavior so that individual components can be tested and replaced without requiring large portions of the firmware to change.

---

# Repository Structure

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

The exact structure will continue to evolve as additional subsystems are implemented.

---

# Current Development Status

## Phase 0 — Development Environment ✅

Established the initial embedded development environment.

Completed:

* PlatformIO project created
* VS Code development environment configured
* ESP32 firmware successfully built
* Firmware successfully uploaded to the ESP32
* Serial communication established
* Git repository established
* Initial project structure created
* Development workflow established

---

## Phase 1 — GPIO & Hardware Abstraction ✅

Built the initial hardware abstraction and device layer.

Completed:

* GPIO abstraction
* LED driver
* Button driver
* Board pin configuration
* Hardware-specific pin definitions
* Separation between hardware configuration and application logic
* Basic hardware testing through the ESP32

The board configuration now provides a central location for hardware pin assignments rather than scattering pin numbers throughout the application.

---

## Phase 2 — Timing & Scheduling ✅

Built a reusable non-blocking timing and scheduling system.

Completed:

* Independent software timers
* Non-blocking timing using `millis()`
* Multiple concurrent timed tasks
* Callback-based task execution
* Lambda callbacks
* Callback capture by value
* Callback capture by reference
* Reusable scheduler
* Periodic task execution
* Configurable task intervals
* Maximum task capacity

The scheduler uses a cooperative execution model, allowing multiple periodic tasks to run without blocking the main firmware loop.

Example responsibilities include:

* Heartbeat LED
* Status reporting
* Fast flashing indicators
* Other periodic firmware tasks

---

## Phase 3 — Scheduler Integration & Firmware Organization ✅

Expanded the scheduler into a more structured firmware service.

Completed:

* Scheduler service implementation
* `Scheduler::Task` abstraction
* Callback function pointers
* Task intervals
* Last-run timestamps
* Maximum task count
* Multiple scheduled callbacks
* Lambda-based callbacks
* Integration with the main firmware loop
* Documentation of scheduler behavior

The firmware can now execute multiple independent periodic behaviors from a centralized scheduling system.

This establishes the foundation for future systems such as:

* Sensor polling
* Motor control updates
* Communication tasks
* Safety monitoring
* Diagnostics
* Control loops

---

# Phase 4 — PWM & Actuator Control 🚧

Phase 4 introduced PWM as a reusable hardware-control primitive and expanded the actuator architecture.

### PWM System

Implemented:

* Reusable PWM abstraction
* PWM channel configuration
* Frequency configuration
* Resolution configuration
* Duty-cycle control
* Percentage-based duty control
* PWM start/stop behavior
* LED integration with PWM
* Hardware configuration through board configuration

The PWM system currently supports an 8-bit duty range:

```text
0   → 0%   duty
128 → ~50% duty
255 → 100% duty
```

PWM frequency was also experimentally tested to understand its effect on visible LED behavior.

Observed behavior included:

* High-frequency PWM producing smooth brightness changes
* Lower frequencies producing increasingly visible flicker
* Very low frequencies approaching visible blinking behavior

These experiments helped establish a practical understanding of PWM frequency, duty cycle, and perceived output.

---

## Servo Control

A reusable servo abstraction was also designed and tested using PWM.

Implemented:

* `ServoConfig`
* Configurable servo pin
* Default angle
* Minimum angle
* Maximum angle
* Minimum pulse width
* Maximum pulse width
* Servo initialization
* Target-angle control
* Default-position movement
* Angle-to-pulse-width conversion
* Pulse-width-to-PWM-duty conversion
* Angle validation/clamping
* PWM integration

The servo system converts a requested angle into the appropriate pulse width and then into the corresponding PWM duty cycle.

Conceptually:

```text
Requested Angle
       ↓
Safe / Validated Angle
       ↓
Pulse Width
       ↓
PWM Duty Cycle
       ↓
ESP32 PWM Hardware
       ↓
Servo Position
```

This establishes a reusable foundation for future actuators.

---

# Current Firmware Architecture

The current firmware is organized around several conceptual layers.

```text
Application
    │
    ├── Robot behavior
    ├── Hardware-independent logic
    │
    ▼
Services
    │
    ├── Scheduler
    ├── Timing
    └── Future communication
    │
    ▼
Devices
    │
    ├── LED
    ├── Button
    ├── PWM
    ├── Servo
    └── Future Motor / Sensor devices
    │
    ▼
Hardware Abstraction
    │
    ├── GPIO
    └── ESP32 hardware APIs
    │
    ▼
ESP32 Hardware
```

The goal is to continue expanding this architecture without allowing hardware-specific details to leak into higher-level robot behavior.

---

# Next Development Phase — Motor Control & Self-Balancing

The next major milestone is transitioning from individual actuators into the robot's drivetrain and control system.

The planned platform is a **two-wheel self-balancing base**.

The first target architecture is:

```text
             Raspberry Pi 5
          High-Level Computing
                  │
                  │
             Communication
                  │
                  ▼
              ESP32
        Real-Time Motor Control
                  │
        ┌─────────┴─────────┐
        ▼                   ▼
   Motor Driver         Motor Driver
        │                   │
        ▼                   ▼
    Left Motor          Right Motor
        │                   │
        └─────────┬─────────┘
                  │
               Wheels
                  ▲
                  │
              MPU6050
                 IMU
```

The ESP32 will eventually be responsible for the real-time control loop while the Raspberry Pi handles higher-level computation and behavior.

---

## Planned Motor Control Development

The motor system will be developed incrementally.

### Motor Abstraction

First establish a reusable motor interface capable of representing:

* Motor configuration
* Direction
* Speed
* PWM control
* Enable/disable behavior
* Stop behavior
* Motor driver pins

The planned speed interface uses:

```text
-100 → full reverse
   0 → stopped
+100 → full forward
```

This provides a hardware-independent interface for higher-level control logic.

---

### Motor Driver

The motor abstraction will eventually interface with a physical motor driver.

The driver will handle:

* Direction control
* PWM speed control
* Enable signals
* Motor stopping
* Hardware-specific behavior

The higher-level `Motor` abstraction should not need to know the exact implementation of the motor driver.

---

# Self-Balancing System

After basic motor control is working, the next goal is a closed-loop two-wheel balancing system.

The initial sensor will be an **MPU6050 IMU**.

The intended control flow is:

```text
             MPU6050
                │
                ▼
        Sensor Measurements
                │
                ▼
        Orientation Estimate
                │
                ▼
          Target Angle
                │
                ▼
              Error
                │
                ▼
              PID
                │
                ▼
        Motor Control Output
                │
          ┌─────┴─────┐
          ▼           ▼
      Left Motor   Right Motor
          │           │
          └─────┬─────┘
                ▼
          Robot Movement
                │
                ▼
             MPU6050
                │
              Feedback
```

The goal is to build this system from first principles rather than treating the balancing algorithm as a black box.

---

# Planned Control-System Milestones

The self-balancing system will be developed through progressively more complex milestones:

* [ ] MPU6050 communication
* [ ] I²C abstraction
* [ ] Raw accelerometer readings
* [ ] Raw gyroscope readings
* [ ] Sensor calibration
* [ ] Angle estimation
* [ ] Complementary filter
* [ ] Motor driver integration
* [ ] Motor speed control
* [ ] Encoder integration
* [ ] Closed-loop motor control
* [ ] PID controller
* [ ] Balance control loop
* [ ] Stable two-wheel balancing
* [ ] Forward/backward movement
* [ ] Turning
* [ ] Autonomous movement

---

# Full Firmware Roadmap

### Foundation

* [x] Development environment
* [x] GPIO abstraction
* [x] LED driver
* [x] Button driver
* [x] Board configuration
* [x] Serial debugging

### Timing & Services

* [x] Independent software timers
* [x] Non-blocking timing
* [x] Callback system
* [x] Lambda callbacks
* [x] Scheduler
* [x] Periodic task execution

### Actuators

* [x] PWM abstraction
* [x] Duty-cycle control
* [x] Percentage-based PWM
* [x] PWM frequency configuration
* [x] LED PWM control
* [x] Servo configuration
* [x] Servo angle control
* [x] Servo PWM conversion
* [ ] Reusable gradual-transition system
* [ ] Motor abstraction
* [ ] Motor driver

### Sensors

* [ ] I²C abstraction
* [ ] MPU6050 driver
* [ ] Sensor calibration
* [ ] Orientation estimation
* [ ] Wheel encoders
* [ ] Additional sensors

### Control Systems

* [ ] Motor closed-loop control
* [ ] PID controller
* [ ] Balance controller
* [ ] Self-balancing
* [ ] Autonomous movement

### Communication

* [ ] UART communication
* [ ] ESP32 ↔ Raspberry Pi communication
* [ ] Command protocol
* [ ] Telemetry
* [ ] Diagnostics

### Reliability

* [ ] Safety system
* [ ] Fault handling
* [ ] Watchdog strategy
* [ ] Error reporting
* [ ] Structured logging
* [ ] Automated testing
* [ ] Hardware-in-the-loop testing

### Advanced Firmware

* [ ] State machines
* [ ] FreeRTOS
* [ ] Multi-tasking architecture
* [ ] Persistent configuration
* [ ] Firmware diagnostics

---

# Robotics Roadmap

* [ ] Two-wheel robotic base
* [ ] Motorized drivetrain
* [ ] Motor driver
* [ ] MPU6050 integration
* [ ] Wheel encoder feedback
* [ ] Closed-loop motor control
* [ ] Self-balancing
* [ ] PID control
* [ ] Autonomous movement
* [ ] Obstacle detection
* [ ] Custom electronics
* [ ] Custom PCB
* [ ] LED system
* [ ] Audio system
* [ ] Raspberry Pi integration

---

# Computer Vision & AI Roadmap

Once the physical platform and low-level control systems are stable, Candy Cadet will gradually expand into perception and intelligent behavior.

Planned systems include:

* [ ] Camera integration
* [ ] OpenCV pipeline
* [ ] Object detection
* [ ] Face detection
* [ ] Person tracking
* [ ] Scene understanding
* [ ] Obstacle perception
* [ ] Robot perception system
* [ ] AI-driven behavior
* [ ] Speech interaction
* [ ] Personality system
* [ ] Long-term memory

---

# Development Philosophy

Candy Cadet is being developed as an engineering learning platform.

Rather than building the entire robot at once, the project is divided into smaller systems that can be designed, tested, understood, and documented independently.

The development process follows:

```text
Understand
    ↓
Design
    ↓
Implement
    ↓
Test
    ↓
Review
    ↓
Document
    ↓
Integrate
```

The primary principles are:

* **Understand before abstracting**
* **Build incrementally**
* **Separate hardware from behavior**
* **Prefer modular systems**
* **Avoid unnecessary complexity**
* **Test hardware independently**
* **Document engineering decisions**
* **Use professional development practices**
* **Build reusable abstractions only when their requirements are understood**
* **Treat failures and experiments as part of the engineering process**

The objective is not only to create a functioning robot, but to develop the engineering judgment required to design complex embedded and robotic systems.

---

# Development Tools

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

* Embedded C++
* PID control
* IMU sensors
* Motor control
* Wheel encoders
* Computer vision
* OpenCV

### High-Level Computing

* Raspberry Pi 5
* Python
* Linux
* Future robotics/AI software

---

# Building the Firmware

## Requirements

* Visual Studio Code
* PlatformIO
* Git
* ESP32 development board

## Build

```bash
pio run
```

## Upload

```bash
pio run --target upload
```

## Serial Monitor

```bash
pio device monitor
```

---

# Documentation

Project documentation is located in the `docs/` directory.

Documentation includes and will continue to expand to cover:

* Firmware architecture
* Hardware architecture
* Phase development
* Engineering logs
* Development roadmap
* Architecture decisions
* Hardware documentation
* Testing documentation
* Control-system experiments
* Design decisions and lessons learned

Each major development phase is documented separately so that the evolution of the system can be understood over time.

---

# Project Status

🚧 **Active Development**

Candy Cadet is an ongoing engineering project.

The firmware has progressed from basic GPIO control into a modular embedded architecture with scheduling, PWM, and servo control. The next major milestone is building the motor-control foundation and integrating the MPU6050 toward a two-wheel self-balancing platform.

Hardware and software architecture will continue to evolve as new systems are designed, tested, and integrated.

---

# Long-Term Goal

Candy Cadet is intended to become a portfolio-scale robotics platform demonstrating practical experience across multiple engineering disciplines:

```text
Mechanical Engineering
        │
        ▼
      Motors
        │
        ▼
Electronics ──────── Sensors
        │               │
        ▼               ▼
     Embedded Firmware
        │
        ▼
   Control Systems
        │
        ▼
      Robotics
        │
        ▼
 Computer Vision
        │
        ▼
 Artificial Intelligence
        │
        ▼
   Autonomous Robot
```

The project is intentionally designed to grow alongside my engineering knowledge.

Every subsystem is an opportunity to learn a new concept, make engineering decisions, test those decisions against real hardware, and document the results.

The ultimate goal is to build Candy Cadet into a functional autonomous robot while developing a strong foundation in **embedded systems, robotics, controls, computer vision, AI, electronics, and hardware engineering**.

---

# License

This project is licensed under the MIT License.
