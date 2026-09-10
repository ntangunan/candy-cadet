# Phase 4 — PWM & Servo Control

## Overview

Phase 4 introduced **Pulse Width Modulation (PWM)** into the Candy Cadet firmware.

The main goal of this phase was to move beyond simple digital HIGH/LOW control and understand how PWM can be used to control hardware with a continuous range of values.

This phase focused on:

* Understanding PWM fundamentals
* Configuring PWM hardware on the ESP32
* Controlling LED brightness using PWM
* Creating a reusable `PWM` abstraction
* Connecting PWM functionality to the existing `LED` abstraction
* Understanding frequency, duty cycle, and resolution
* Designing and beginning a `Servo` abstraction
* Converting servo angles into PWM pulse widths
* Converting pulse widths into PWM duty-cycle values
* Establishing a foundation for future motor, servo, and LED control

The overall design philosophy continued to follow:

> **Understand → Design → Implement → Test → Review → Document**

---

# Milestones

## Milestone 1 — Understand PWM

### Goal

Understand what PWM is and how it can be used to control hardware.

### What We Learned

PWM rapidly switches a digital output between HIGH and LOW.

Instead of changing the voltage continuously, PWM changes the amount of time the signal spends HIGH during each cycle.

This is represented by the **duty cycle**.

For example:

| Duty Cycle | Result                     |
| ---------: | -------------------------- |
|         0% | Always LOW                 |
|        25% | HIGH for 25% of the cycle  |
|        50% | HIGH for half of the cycle |
|        75% | HIGH for 75% of the cycle  |
|       100% | Always HIGH                |

For an LED, the human eye perceives this as a change in brightness.

For other hardware, PWM can represent a control signal such as:

* LED brightness
* Motor speed
* Servo position
* Other actuator control

### Frequency

We also learned that PWM frequency determines how quickly the PWM cycle repeats.

The experiments demonstrated the difference between frequency and duty cycle:

* Increasing frequency makes PWM cycles occur more quickly.
* Changing duty cycle changes the proportion of time the signal is HIGH.
* Frequency and duty cycle control different properties.

Testing demonstrated that:

* Around `1000 Hz`, LED fading appeared smooth.
* Around `100 Hz`, the fading became slower/more visibly stepped.
* Around `10 Hz`, the LED behavior became visibly similar to blinking.
* Very low frequencies can produce behavior that no longer looks like normal brightness control.

### Resolution

PWM resolution determines how many discrete duty-cycle values are available.

The project currently uses:

```text
8-bit resolution
```

This provides:

```text
0–255
```

possible duty values.

Therefore:

```text
0   = 0%
128 ≈ 50%
255 = 100%
```

This established the relationship:

```text
PWM Percentage
      ↓
Duty Cycle
      ↓
Hardware PWM Output
```

---

# Milestone 2 — Configure ESP32 PWM

### Goal

Configure the ESP32's PWM hardware and verify that PWM output works correctly.

### Configuration

The project currently uses:

```text
Frequency: 1000 Hz
Resolution: 8 bits
```

The ESP32 PWM channel is configured with the appropriate GPIO pin, channel, frequency, and resolution.

The project initially experimented with newer ESP32 PWM APIs and platform configurations, but compatibility and build-time issues led us to return to the currently working Arduino/ESP32 environment.

The current project therefore prioritizes:

* Stable PlatformIO builds
* The existing ESP32 Arduino framework
* The working `ledcSetup`
* The working `ledcAttachPin`
* The working `ledcWrite`

This gives the project a stable foundation while keeping the hardware abstraction independent from most of the application code.

---

# Milestone 3 — Create PWM Abstraction

### Goal

Prevent the rest of the firmware from needing to directly interact with ESP32 PWM APIs.

Instead of having hardware-specific PWM code spread throughout the project, PWM functionality was placed behind a reusable `PWM` class.

### PWM Responsibilities

The `PWM` abstraction is responsible for:

* Configuring PWM hardware
* Setting the PWM duty cycle
* Starting PWM
* Stopping PWM
* Converting a percentage into a duty-cycle value

Conceptually:

```text
Application
    ↓
PWM abstraction
    ↓
ESP32 PWM hardware
```

### Core Interface

The current PWM abstraction provides functionality conceptually equivalent to:

```cpp
configure(pin, channel, frequency, resolution)
setDuty(duty)
setPercentage(percentage)
start()
stop()
```

### Percentage Conversion

The abstraction allows application code to think in percentages rather than raw hardware values.

For example:

```text
0%   → 0
50%  → 128
100% → 255
```

This is important because hardware-specific values should not need to leak into higher-level application logic.

Instead of:

```text
"Set PWM duty to 128"
```

higher-level code can express:

```text
"Set brightness to 50%"
```

The PWM class handles the conversion.

---

# Milestone 4 — Integrate PWM With LED

### Goal

Extend the existing LED abstraction so an LED can optionally use PWM.

Previously, the LED abstraction primarily represented digital LED control:

```text
ON
OFF
```

PWM introduced a third capability:

```text
Brightness control
```

### LED/PWM Relationship

The LED owns or references an optional PWM controller.

Conceptually:

```text
LED
 ├── Digital control
 └── Optional PWM control
```

The LED can therefore expose a higher-level interface such as:

```cpp
setBrightness(percentage)
```

without requiring the caller to understand:

* PWM channels
* PWM resolution
* Duty-cycle values
* ESP32 LEDC functions

### Data Flow

```text
Application
    │
    ▼
LED::setBrightness(percentage)
    │
    ▼
PWM::setPercentage(percentage)
    │
    ▼
Percentage → Duty Cycle
    │
    ▼
PWM Hardware
    │
    ▼
ESP32 GPIO
    │
    ▼
LED brightness
```

This represents an important architectural improvement because the LED abstraction expresses **what the hardware should do**, while the PWM abstraction handles **how the ESP32 accomplishes it**.

---

# Milestone 5 — PWM Testing & Experiments

### Goal

Verify our understanding of PWM by predicting hardware behavior before running experiments.

This milestone emphasized learning through experimentation rather than simply making the hardware work.

### Experiment: Frequency

We predicted that increasing PWM frequency would cause the PWM signal to switch faster.

The experiments confirmed that frequency affects how rapidly the PWM cycles occur.

At sufficiently low frequencies, the individual PWM cycles become visually noticeable and the LED begins to appear as though it is blinking.

### Experiment: Duty Cycle

We tested the relationship between duty cycle and brightness.

The important observation was:

> **Duty cycle controls the average amount of time the LED is receiving power, which changes perceived brightness.**

For example:

```text
25% → dimmer
50% → medium brightness
75% → brighter
100% → maximum brightness
```

### Key Distinction

We established that:

```text
Frequency ≠ Brightness
```

and:

```text
Duty Cycle → Brightness
Frequency → PWM cycle speed
```

Although frequency can affect how PWM is perceived at very low values, the primary brightness control is duty cycle.

---

# Milestone 6 — Begin Servo Abstraction

### Goal

Apply the PWM concepts learned from LED control to a more complex actuator: a servo.

A servo does not use PWM in exactly the same conceptual way as LED brightness control.

Instead, the servo interprets the **pulse width** of the PWM signal as a position command.

The design therefore required an additional layer of conversion:

```text
Servo Angle
    ↓
Pulse Width
    ↓
PWM Duty Cycle
    ↓
ESP32 PWM Hardware
    ↓
Servo
```

---

# Servo Design

The current servo configuration contains parameters such as:

```cpp
ServoConfig
{
    pin,
    defaultAngle,
    minAngle,
    maxAngle,
    minPulseWidth,
    maxPulseWidth
}
```

These values allow different servos to potentially have different operating characteristics.

### Why Configuration Is Important

Instead of hardcoding servo-specific values directly into the `Servo` class, the configuration describes the servo's capabilities.

This allows the same abstraction to potentially be reused with different servos.

---

# Servo Angle Validation

The servo accepts an angle and validates it before converting it into a PWM signal.

The conceptual flow is:

```text
Requested Angle
       ↓
Validate / Clamp
       ↓
Safe Angle
       ↓
Convert Angle → Pulse Width
       ↓
Convert Pulse Width → Duty Cycle
       ↓
PWM
```

For the current testing setup:

```text
0°   → minimum position
90°  → middle position
180° → maximum position
```

The servo also has a configured default angle.

This allows the servo to return to a known position through functionality such as:

```cpp
moveToDefault()
```

---

# Milestone 7 — Angle → Pulse Width Conversion

### Goal

Convert a desired servo angle into the appropriate pulse width.

The servo currently uses a minimum and maximum pulse width.

Conceptually:

```text
minAngle ───────────────────── maxAngle
   │                              │
   ▼                              ▼
minPulseWidth ─────────────── maxPulseWidth
```

For the current test configuration, we observed:

```text
0°   → 675 µs
90°  → 1650 µs
180° → 2625 µs
```

This demonstrates the relationship between physical position and PWM pulse width.

### Conversion

The conversion can be thought of as a linear mapping:

```text
Angle
  ↓
Position within angle range
  ↓
Position within pulse-width range
  ↓
Pulse width
```

This abstraction allows the application to request:

```text
"Move servo to 90°"
```

rather than:

```text
"Generate a 1650 µs pulse."
```

---

# Milestone 8 — Pulse Width → PWM Duty Cycle

### Goal

Convert the servo's desired pulse width into the numerical duty-cycle value required by the ESP32 PWM hardware.

This is an important distinction.

The servo is conceptually controlled using:

```text
Pulse Width
```

while the ESP32 PWM peripheral is configured using:

```text
Duty Cycle
```

Therefore the firmware needs to perform another conversion.

The complete chain is:

```text
Angle
  ↓
Pulse Width
  ↓
Duty Cycle
  ↓
PWM Hardware
```

For example, our testing produced values such as:

```text
0°
→ 675 µs
→ duty ≈ 8

90°
→ 1650 µs
→ duty ≈ 21

180°
→ 2625 µs
→ duty ≈ 33
```

The exact duty values depend on the configured PWM frequency and resolution.

This reinforced an important concept:

> The numerical duty value is not universal. It depends on the PWM configuration.

---

# Milestone 9 — Connect Servo to PWM Abstraction

### Goal

Keep the `Servo` class independent from the low-level ESP32 PWM implementation.

The servo was designed with a PWM dependency:

```cpp
PWM* pwm_;
```

The servo can therefore use an existing PWM abstraction rather than directly calling ESP32 PWM functions.

Conceptually:

```text
Servo
  │
  │ owns configuration
  │ calculates position
  │ calculates pulse width
  │
  ▼
PWM
  │
  │ converts/outputs duty cycle
  ▼
ESP32 PWM Hardware
```

This follows the same abstraction philosophy established with the LED.

---

# Current Project Structure

The firmware is organized around separating hardware-specific behavior from higher-level hardware abstractions.

The current conceptual structure is:

```text
Candy Cadet Firmware
│
├── Configuration
│   └── board_config
│
├── Application / Main
│   └── main.cpp
│
├── Scheduling
│   └── Scheduler
│
├── Hardware Abstractions
│   ├── LED
│   ├── PWM
│   └── Servo
│
└── ESP32 Hardware
    └── Arduino / LEDC PWM APIs
```

The exact source/header organization may vary as the project continues to evolve, but the architectural responsibilities currently follow this structure.

---

# Major Responsibilities

## `board_config`

Contains board-specific configuration such as GPIO assignments and hardware constants.

Current relevant pins include:

```text
BUTTON_CONTROLLED_LED_PIN = 32
USER_BUTTON_PIN            = 14
HEARTBEAT_LED_PIN          = 33
FAST_FLASH_LED_PIN         = 13
PWM_LED_PIN_1              = 19
PWM_LED_PIN_2              = 18
```

The purpose of keeping these values in configuration is to prevent GPIO numbers from being scattered throughout the implementation.

---

## `Scheduler`

The scheduler provides a lightweight mechanism for periodically executing tasks.

The current scheduler uses tasks conceptually structured as:

```cpp
Task
{
    callback,
    interval,
    lastRun
}
```

The scheduler currently supports a maximum number of tasks and is already being used for periodic firmware behavior such as:

```text
Heartbeat
Status output
Fast flashing
```

This provides the foundation for eventually scheduling periodic hardware behaviors without blocking the main firmware loop.

---

## `LED`

The LED abstraction represents LED behavior.

It can handle normal digital LED control and can optionally use PWM for brightness control.

The important abstraction is:

```text
LED → "how bright should I be?"
```

rather than:

```text
LED → "what raw ESP32 duty value should I write?"
```

---

## `PWM`

The PWM abstraction represents the ESP32 PWM functionality.

Its responsibilities include:

```text
Configure
Start
Stop
Set duty
Set percentage
```

It acts as the bridge between hardware abstractions and the ESP32 PWM peripheral.

---

## `Servo`

The servo abstraction represents a positional actuator.

Its responsibilities include:

```text
Receive angle
Validate angle
Clamp to safe range
Convert angle → pulse width
Convert pulse width → PWM duty
Send duty to PWM
```

This keeps servo-specific calculations inside the servo abstraction.

---

# Major PWM Data Flows

## LED Brightness Event

When the application requests a new LED brightness:

```text
Application
    │
    │ setBrightness(50%)
    ▼
LED
    │
    │ forwards percentage
    ▼
PWM
    │
    │ percentage → duty
    ▼
PWM Duty
    │
    │ write duty value
    ▼
ESP32 PWM Peripheral
    │
    ▼
GPIO
    │
    ▼
LED
```

Example:

```text
50%
 ↓
128 duty
 ↓
PWM output
 ↓
LED ≈ 50% perceived brightness
```

---

# Servo Movement Event

When the application requests a servo movement:

```text
Application
    │
    │ moveTo(90°)
    ▼
Servo
    │
    │ validate angle
    ▼
Safe Angle
    │
    │ 90°
    ▼
Angle → Pulse Width
    │
    │ 90° → 1650 µs
    ▼
Pulse Width → Duty
    │
    │ 1650 µs → duty ≈ 21
    ▼
PWM
    │
    ▼
ESP32 PWM Peripheral
    │
    ▼
GPIO
    │
    ▼
Servo
    │
    ▼
Physical Position
```

---

# Complete PWM Architecture

The major PWM-related architecture can therefore be represented as:

```text
                  APPLICATION
                       │
             ┌─────────┴─────────┐
             │                   │
             ▼                   ▼
            LED                Servo
             │                   │
             │                   │
      brightness %          angle °
             │                   │
             ▼                   ▼
            PWM          angle → pulse width
             │                   │
             │                   ▼
             │             pulse width → duty
             │                   │
             └─────────┬─────────┘
                       │
                       ▼
                      PWM
                       │
                       ▼
              ESP32 PWM / LEDC
                       │
                       ▼
                     GPIO
                       │
              ┌────────┴────────┐
              ▼                 ▼
             LED              Servo
```

The important architectural idea is that **LEDs and servos can share the same underlying PWM abstraction even though they interpret PWM differently**.

---

# Key Concepts Established in Phase 4

By the end of Phase 4, we established an understanding of:

### PWM

PWM rapidly switches a digital signal to create controllable average output behavior.

### Frequency

Frequency controls how frequently PWM cycles repeat.

```text
Higher frequency → faster PWM cycles
Lower frequency  → slower PWM cycles
```

### Duty Cycle

Duty cycle controls how much of each cycle the signal remains HIGH.

```text
Higher duty → more average output
Lower duty  → less average output
```

### Resolution

Resolution determines how many discrete duty-cycle values are available.

Current configuration:

```text
8-bit → 0–255
```

### LED PWM

LED brightness can be controlled by changing PWM duty cycle.

### Servo PWM

Servo position can be controlled by changing pulse width.

### Abstraction

Different hardware can use the same low-level PWM system while maintaining hardware-specific behavior in their own abstractions.

---

# Architectural Lessons

Phase 4 reinforced several important software/hardware design principles.

## 1. Separate "What" From "How"

The application should describe what it wants:

```text
LED brightness = 50%
Servo position = 90°
```

rather than how the ESP32 should accomplish it:

```text
PWM duty = 128
PWM duty = 21
```

---

## 2. Hardware-Specific Logic Should Be Isolated

ESP32-specific PWM calls should remain inside the PWM abstraction rather than being repeated throughout the firmware.

This makes the rest of the project easier to understand and potentially easier to port.

---

## 3. Hardware Abstractions Can Build on Each Other

The architecture now resembles:

```text
Application
    ↓
Device Abstraction
    ↓
PWM Abstraction
    ↓
ESP32 Hardware
```

For example:

```text
Servo
  ↓
PWM
  ↓
ESP32
```

and:

```text
LED
  ↓
PWM
  ↓
ESP32
```

This gives the project a reusable foundation for future actuators.

---

# Phase 4 Accomplishments

By completing Phase 4, Candy Cadet firmware now has:

* A working ESP32 PWM configuration
* An understanding of PWM frequency
* An understanding of duty cycle
* An understanding of PWM resolution
* 8-bit PWM control
* A reusable `PWM` abstraction
* Percentage-based PWM control
* PWM-controlled LED brightness
* PWM configuration separated from LED behavior
* A designed and partially implemented `Servo` abstraction
* Servo configuration through `ServoConfig`
* Servo angle validation
* Servo default-position support
* Angle → pulse-width conversion
* Pulse-width → duty-cycle conversion
* Servo → PWM abstraction integration
* Successful servo position testing at `0°`, `90°`, and `180°`
* A clearer layered hardware architecture
* A foundation for future motors, servos, actuators, and gradual transitions

---

# Phase 4 → Phase 5

Phase 4 established PWM as a reusable low-level capability.

Future phases can build on this foundation rather than implementing PWM independently for every device.

Potential future work includes:

* More robust PWM resource/channel management
* Improved servo calibration
* Multiple simultaneous servos
* Motor speed control
* Reusable gradual transitions
* LED fading
* Servo movement transitions
* Motor acceleration/deceleration
* Automated hardware testing
* Error handling and diagnostics
* Firmware logging
* Integration with the Raspberry Pi dashboard

A particularly important future architectural goal is to create a **generic gradual-transition system** that can eventually operate on values such as:

```text
LED brightness
Motor speed
Servo position
```

rather than creating a separate transition system for each individual hardware type.

This should remain a reusable system unless future requirements demonstrate that a hardware-specific implementation is necessary.
