What is CHUD?
    CHUD stands for Cognitive Hardware for Unified Decisionmaking. Unlike most anagrams, we started with CHUD and worked on what it could mean lol.
    CHUD is comprised of a Mechanical Engineer from UCLA, a Computer Engineer from UCLA, and a Mechanical Engineer from Tuskegee University. We're hometown friends that reconnect over summers to build interesting projects to further our own understanding of our disciplines.

What is this project?
    Name: 
    CHUDtrap

    Purpose: 
    To give our team hands-on experience in mechanical engineering, embedded systems, computer engineering, and artificial intelligence. By designing, building, and programming an interactive robot from scavanged parts, we aim to learna and apply basic robotic and engineering fundamentals in prototyping, control systems, computer vision, wireless communication, software development, and collaborative engineering practices.

    What is the hardware and their purposes?
    - Macbook, the brain/controller: receives information, analyzes it, and translates it into simpler terms
    - ESP32-CAM, the middle man: records scene, sends to brain, then receives simpler code to translate to the body
    - Arduino Uno r4 Minima, the body: receives commands and activates all motors for physical reaction
    
# Candy Cadet Firmware

Professional embedded firmware for the Candy Cadet animatronic robot.

This directory contains the firmware that runs on the ESP32 microcontroller responsible for real-time hardware control, communication, and robot behavior.

The primary goal of this project is not only to build a functioning robot, but also to develop professional embedded systems engineering skills through clean architecture, modular firmware design, and industry-standard development practices.

---

## Project Goals

This firmware is being developed as a long-term educational and portfolio project focused on learning professional embedded systems engineering.

Key learning objectives include:

- Embedded firmware architecture
- Hardware abstraction layers (HAL)
- GPIO, PWM, ADC, UART, SPI, and I²C
- Cooperative scheduling
- State machines
- Sensor integration
- Actuator control
- Safety systems
- FreeRTOS
- Embedded debugging
- Testing and maintainability
- Professional Git workflows
- Technical documentation

---

## Hardware Platform

Current target:

- ESP32 Development Board
- PlatformIO
- Arduino Framework (initial development)

Future hardware will include:

- Servo motors
- LEDs
- Buttons
- Sensors
- Audio hardware
- Communication interfaces

---

## Repository Structure

```
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

```
src/
├── app/
├── config/
├── drivers/
├── hal/
├── services/
└── main.cpp
```

---

## Current Development Status

### Phase 0 — Development Environment

Completed:

- PlatformIO project
- VS Code configuration
- Repository organization
- Firmware architecture planning
- Git workflow
- Initial documentation

Next:

- Phase 1 – Firmware Architecture
- Phase 2 – GPIO Driver Development

---

## Building the Firmware

### Requirements

- Visual Studio Code
- PlatformIO Extension
- Git

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

Current documentation includes:

- Firmware architecture
- Engineering log
- Firmware roadmap
- Architecture decision records

---

## Design Philosophy

The firmware follows several guiding engineering principles:

- Separation of concerns
- Layered architecture
- Modular design
- Hardware abstraction
- Maintainability
- Scalability
- Testability
- Readability

The objective is to build firmware that resembles what would be developed in a professional robotics company.

---

## Roadmap

- Phase 0 — Development Environment ✅
- Phase 1 — Firmware Architecture
- Phase 2 — GPIO Driver
- Phase 3 — Timing & Scheduler
- Phase 4 — PWM
- Phase 5 — UART Communication
- Phase 6 — Sensors
- Phase 7 — State Machines
- Phase 8 — Actuator Framework
- Phase 9 — Safety System
- Phase 10 — FreeRTOS

---

## License

This project is licensed under the MIT License.