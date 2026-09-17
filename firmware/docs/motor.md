# Motor Abstraction

## Overview

The `Motor` abstraction provides a high-level interface for controlling a DC motor using two PWM control signals.

The abstraction separates motor behavior from the underlying PWM implementation. The `Motor` class is responsible for interpreting a signed speed value and determining the motor direction, while the `PWM` class is responsible for generating the actual PWM signal.

Each motor uses two PWM objects:

* One PWM for forward motion
* One PWM for backward motion

This design matches the control structure of the L9110H motor driver, where each motor has two control inputs.

---

## Motor Architecture

The motor abstraction is organized into three main components:

```text
Motor
├── MotorConfig
├── Forward PWM
├── Backward PWM
└── PWMResourceManager
```

The responsibilities are separated as follows:

| Component            | Responsibility                                       |
| -------------------- | ---------------------------------------------------- |
| `MotorConfig`        | Stores motor-specific hardware and PWM configuration |
| `Motor`              | Controls motor direction and speed                   |
| `PWM`                | Generates PWM signals on the motor control pins      |
| `PWMResourceManager` | Allocates and releases PWM resources                 |

The `Motor` does not directly implement PWM generation. Instead, it receives PWM objects that have already been allocated by the application.

---

# MotorConfig

`MotorConfig` stores the configuration required by a motor.

```cpp
struct MotorConfig
{
    uint8_t controlPin1;
    uint8_t controlPin2;

    uint32_t pwmFrequency;
    uint8_t pwmResolution;

    bool directionInverted;
};
```

### Configuration Fields

| Field               | Description                               |
| ------------------- | ----------------------------------------- |
| `controlPin1`       | GPIO used for the forward control signal  |
| `controlPin2`       | GPIO used for the backward control signal |
| `pwmFrequency`      | PWM operating frequency                   |
| `pwmResolution`     | PWM resolution in bits                    |
| `directionInverted` | Negates speed to flip motor direction     |

For the current Candy Cadet motor configuration:

```cpp
Devices::MotorConfig leftMotorConfig
{
    Board::LEFT_A_MOTOR_FORWARD_PIN,
    Board::LEFT_A_MOTOR_BACKWARD_PIN,
    1000,
    8,
    false
};
```

This configures the motor to use:

* 1000 Hz PWM
* 8-bit PWM resolution
* One GPIO for forward control
* One GPIO for backward control
* left motor direction is fixed

---

# Motor Class

The `Motor` class provides the high-level interface for controlling a DC motor.

```cpp
class Motor
{
public:
    Motor(const MotorConfig& config);

    void initialize();
    void setSpeed(int speed);
    void stop();

    void setPWM(
        PWM& pwmForward,
        PWM& pwmBackward,
        PWMResourceManager& manager,
        PWMAllocationHandle forwardPwmHandle,
        PWMAllocationHandle backwardPwmHandle
    );

    void releasePWM();

private:
    MotorConfig config_;

    PWM* forwardPwm_ = nullptr;
    PWM* backwardPwm_ = nullptr;

    PWMResourceManager* pwmManager_ = nullptr;

    PWMAllocationHandle forwardPwmHandle_;
    PWMAllocationHandle backwardPwmHandle_;

    int speed_ = 0;

    void setForward_(int speed);
    void setBackward_(int speed);
};
```

---

# Signed Speed Interface

The primary motor control interface is:

```cpp
void setSpeed(int speed);
```

The speed uses a signed range:

```text
-100 ───────── 0 ───────── +100
  ↑             ↑             ↑
Reverse        Stop         Forward
```

|  Speed | Behavior        |
| -----: | --------------- |
|  `100` | Forward at 100% |
|   `50` | Forward at 50%  |
|   `25` | Forward at 25%  |
|    `0` | Stop            |
|  `-25` | Reverse at 25%  |
|  `-50` | Reverse at 50%  |
| `-100` | Reverse at 100% |

The signed value allows the caller to specify both direction and magnitude through a single function.

---

# Direction Handling

The `Motor` abstraction interprets the sign of the speed.

```cpp
void Motor::setSpeed(int speed)
{
    if (speed > 0)
    {
        setForward_(speed);
    }
    else if (speed < 0)
    {
        setBackward_(speed);
    }
    else
    {
        stop();
    }

    speed_ = speed;
}
```

The behavior is:

```text
setSpeed(+25)
      ↓
setForward_(25)
      ↓
Forward PWM = 25%
Backward PWM = 0%
```

For reverse:

```text
setSpeed(-25)
      ↓
setBackward_(-25)
      ↓
Backward PWM = 25%
Forward PWM = 0%
```

The negative value is converted to a positive magnitude before being passed to the PWM abstraction.

---

# PWM Direction Control

The motor uses two PWM outputs.

```text
             Motor
               │
       ┌───────┴───────┐
       ↓               ↓
 Forward PWM      Backward PWM
       │               │
 Control Pin 1    Control Pin 2
       │               │
       └────── Motor Driver ──── Motor
```

Only one PWM output is given a non-zero percentage at a time.

### Forward

```cpp
forwardPwm_->setPercentage(speed);
backwardPwm_->setPercentage(0);
```

### Backward

```cpp
backwardPwm_->setPercentage(-speed);
forwardPwm_->setPercentage(0);
```

### Stop

```cpp
forwardPwm_->setPercentage(0);
backwardPwm_->setPercentage(0);
```

This ensures that the two direction signals are not simultaneously commanding the motor.

---

# Why PWM Does Not Accept Negative Values

The `PWM` abstraction uses:

```cpp
setPercentage(int percentage);
```

with a valid range of:

```text
0% ───────────── 100%
```

Negative values are intentionally not handled by `PWM`.

Direction is a responsibility of the `Motor` abstraction.

This creates a clean separation:

```text
Motor
  │
  │ understands direction
  │ understands signed speed
  ↓
PWM
  │
  │ understands duty cycle
  │ understands 0–100%
  ↓
Hardware
```

For example:

```cpp
motor.setSpeed(-25);
```

becomes:

```text
Motor
  ↓
-25 = reverse, magnitude 25
  ↓
Backward PWM = 25%
Forward PWM = 0%
```

The PWM class itself never needs to understand what "reverse" means.

---

# PWM Resource Management

Each motor requires two PWM resources:

```text
Motor
├── Forward PWM resource
└── Backward PWM resource
```

The application allocates these resources using `PWMResourceManager`.

The resulting PWM objects and allocation handles are then passed into the motor through:

```cpp
setPWM(...)
```

This allows the motor to retain references to the resources it uses.

---

# setPWM()

`setPWM()` connects the motor to its PWM resources.

```cpp
void Motor::setPWM(
    PWM& forwardPwm,
    PWM& backwardPwm,
    PWMResourceManager& manager,
    PWMAllocationHandle forwardPwmHandle,
    PWMAllocationHandle backwardPwmHandle
)
```

The function performs validation before storing the resources:

```cpp
if (forwardPwm_ != nullptr ||
    backwardPwm_ != nullptr ||
    !manager.validate(forwardPwmHandle) ||
    !manager.validate(backwardPwmHandle))
{
    return;
}
```

If valid, the motor stores:

```cpp
forwardPwm_ = &forwardPwm;
backwardPwm_ = &backwardPwm;
pwmManager_ = &manager;

forwardPwmHandle_ = forwardPwmHandle;
backwardPwmHandle_ = backwardPwmHandle;
```

The motor therefore keeps references to both PWM objects and their allocation handles.

---

# initialize()

`initialize()` configures the motor's PWM outputs and establishes its initial state.

```cpp
void Motor::initialize()
{
    if (forwardPwm_ == nullptr || backwardPwm_ == nullptr)
    {
        return;
    }

    forwardPwm_->configure(
        config_.controlPin1,
        config_.pwmFrequency
    );

    backwardPwm_->configure(
        config_.controlPin2,
        config_.pwmFrequency
    );

    stop();

    forwardPwm_->start();
    backwardPwm_->start();
}
```

The initialization sequence is:

```text
1. Verify PWM objects exist
        ↓
2. Configure forward PWM
        ↓
3. Configure backward PWM
        ↓
4. Set motor state to stopped
        ↓
5. Start forward PWM
        ↓
6. Start backward PWM
```

The motor starts in a stopped state rather than immediately driving the wheels.

---

# stop()

`stop()` sets both PWM outputs to zero.

```cpp
void Motor::stop()
{
    forwardPwm_->setPercentage(0);
    backwardPwm_->setPercentage(0);

    speed_ = 0;
}
```

The resulting state is:

```text
Forward PWM  = 0%
Backward PWM = 0%
Motor speed  = 0
```

---

# releasePWM()

`releasePWM()` stops the motor and returns its PWM resources to the resource manager.

```cpp
void Motor::releasePWM()
{
    if (forwardPwm_ == nullptr ||
        backwardPwm_ == nullptr ||
        pwmManager_ == nullptr)
    {
        return;
    }

    forwardPwm_->stop();
    backwardPwm_->stop();

    pwmManager_->release(forwardPwmHandle_);
    pwmManager_->release(backwardPwmHandle_);

    forwardPwm_ = nullptr;
    backwardPwm_ = nullptr;
    pwmManager_ = nullptr;

    forwardPwmHandle_ = {-1, -1};
    backwardPwmHandle_ = {-1, -1};
}
```

The release sequence is:

```text
Stop PWM outputs
      ↓
Release forward PWM resource
      ↓
Release backward PWM resource
      ↓
Clear stored PWM pointers
      ↓
Clear allocation handles
```

This prevents the motor from retaining references to resources that are no longer allocated to it.

---

# Private Direction Functions

The public `setSpeed()` function delegates direction-specific behavior to private functions.

### Forward

```cpp
void Motor::setForward_(int speed)
{
    forwardPwm_->setPercentage(speed);
    backwardPwm_->setPercentage(0);
}
```

### Backward

```cpp
void Motor::setBackward_(int speed)
{
    backwardPwm_->setPercentage(-speed);
    forwardPwm_->setPercentage(0);
}
```

The private functions keep the direction logic out of the public interface.

---

# Motor State

The motor maintains its most recently requested speed:

```cpp
int speed_ = 0;
```

Examples:

```cpp
motor.setSpeed(50);
```

results in:

```text
speed_ = 50
```

while:

```cpp
motor.setSpeed(-50);
```

results in:

```text
speed_ = -50
```

and:

```cpp
motor.stop();
```

results in:

```text
speed_ = 0
```

The sign of `speed_` represents the requested direction.

---

# Application Integration

The application is responsible for creating and allocating the PWM resources before giving them to the motor.

The general flow is:

```text
Application
    │
    ├── Create PWM requirements
    │
    ├── Allocate forward PWM
    │
    ├── Allocate backward PWM
    │
    ├── Create PWM objects
    │
    ├── Configure PWM objects
    │
    ├── Give PWM objects + handles to Motor
    │
    └── Initialize Motor
```

The motor itself then handles its runtime behavior:

```text
Scheduler
    │
    ↓
Motor::setSpeed()
    │
    ├── Forward → forward PWM
    │
    ├── Reverse → backward PWM
    │
    └── Stop → both PWM = 0
```

---

# Current Candy Cadet Motor Architecture

Candy Cadet currently uses two DC motors:

```text
                 Candy Cadet
                     │
          ┌──────────┴──────────┐
          │                     │
     Left Motor             Right Motor
          │                     │
     ┌────┴────┐           ┌────┴────┐
     ↓         ↓           ↓         ↓
 Forward   Backward      Forward   Backward
   PWM       PWM           PWM       PWM
     │         │           │         │
     └────┬────┘           └────┬────┘
          ↓                     ↓
       L9110H                L9110H
          │                     │
          ↓                     ↓
     Left Wheel            Right Wheel
```

Each motor therefore has independent control over:

* Direction
* Speed
* Forward PWM
* Backward PWM
* PWM resource allocation

---

# Design Rationale

The motor abstraction is designed to keep responsibilities separated.

### Application

Responsible for:

* Hardware configuration
* PWM resource allocation
* Connecting resources to devices
* Scheduler task registration

### Motor

Responsible for:

* Motor direction
* Motor speed
* Starting/stopping motor control
* Managing its assigned PWM resources

### PWM

Responsible for:

* PWM configuration
* Duty cycle
* Percentage conversion
* Starting/stopping PWM output

### PWMResourceManager

Responsible for:

* PWM resource allocation
* Resource validation
* Resource release

This separation allows the motor implementation to remain independent of the details of PWM resource allocation.

---

# Future Extensions

The current abstraction provides a foundation for future Candy Cadet motor control.

Potential future additions include:

* Encoder feedback
* Closed-loop speed control
* PID control
* Acceleration/deceleration ramps
* Smooth motor transitions
* Differential-drive control
* Self-balancing control
* IMU-based stabilization

For example, future PID control could calculate signed motor speeds:

```text
                 MPU-6050
                    │
                    ↓
               PID Controller
                    │
             ┌──────┴──────┐
             ↓             ↓
        Left Motor     Right Motor
        setSpeed()     setSpeed()
```

The existing signed-speed interface makes the `Motor` abstraction suitable for this future control architecture because a controller can produce positive or negative outputs directly.

---

# Summary

The `Motor` abstraction provides a high-level interface for controlling a DC motor while keeping PWM implementation details separate.

The key design principle is:

```text
Motor = direction + speed
PWM   = duty cycle
```

A signed motor command determines direction:

```text
Positive → Forward
Zero     → Stop
Negative → Backward
```

The magnitude of the command determines the PWM percentage:

```text
setSpeed(25)  → Forward PWM 25%
setSpeed(0)   → Both PWM 0%
setSpeed(-25) → Backward PWM 25%
```
