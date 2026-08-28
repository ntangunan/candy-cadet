# Phase 3 — Timing & Task Scheduling

**Status:** ✅ Complete

---

# Overview

Phase 3 marks the transition from simple hardware control into structured, time-aware firmware.

The primary objective of this phase was to develop a **non-blocking software timing system** capable of managing multiple independent tasks without relying on `delay()`.

Instead of writing timing logic directly into the application loop, the firmware was progressively redesigned around reusable timers, callbacks, and a lightweight task-scheduling architecture.

This phase established an important foundation for Candy Cadet's future firmware systems, including sensors, motors, servos, communication, battery monitoring, and other periodic tasks.

---

# Phase Goals

The goals of Phase 3 were to:

* Understand non-blocking timing in embedded systems
* Replace blocking delays with elapsed-time logic
* Implement reusable software timers
* Support multiple independent timers
* Separate timing logic from application behavior
* Introduce callbacks and function pointers
* Understand and apply C++ lambdas
* Begin designing a lightweight task scheduler
* Create an architecture that can scale as more firmware subsystems are added
* Maintain independently testable firmware components

---

# Milestone 1 — Basic Non-Blocking Timer

**Status:** ✅ Complete

The first milestone introduced the fundamental concept of non-blocking timing.

A timer was implemented using:

* `millis()`
* A stored previous timestamp
* A configurable time interval
* Elapsed-time comparison

The basic structure was:

```cpp
if (millis() - previousMillis >= interval)
{
    previousMillis = millis();

    // Timed action
}
```

This allowed the firmware to perform an action periodically without stopping execution of the rest of the application.

### What Was Accomplished

* Learned why `delay()` is unsuitable for scalable robotics firmware
* Implemented elapsed-time-based scheduling
* Created the first software timer
* Used `millis()` to measure elapsed time
* Established the foundation for periodic firmware tasks
* Demonstrated a non-blocking LED heartbeat

---

# Milestone 2 — Multiple Independent Timers

**Status:** ✅ Complete

The next milestone expanded the timing system to support multiple independent timers.

Two separate timers were implemented:

* **Heartbeat Timer:** 500 ms
* **Status Timer:** 1000 ms

Each timer maintained its own timing state and interval.

This demonstrated that multiple periodic behaviors can run concurrently within the same main application loop without blocking one another.

### What Was Accomplished

* Implemented multiple software timers
* Gave each timer an independent interval
* Allowed different LEDs to operate at different frequencies
* Demonstrated concurrent periodic behavior
* Separated timing state between different tasks
* Established the concept of independent scheduled tasks

---

# Milestone 3 — Callback-Based Task Scheduling

**Status:** ✅ Complete

The third milestone moved the timing system toward a reusable scheduler architecture.

Instead of having the timer directly contain the behavior it should execute, the timer was designed around the concept of a **callback**.

The scheduler determines **when** a task should execute, while the callback determines **what** the task should do.

Conceptually:

```text
Scheduler
    │
    ├── Determines task is ready
    │
    └── Executes callback
            │
            └── Task-specific behavior
```

This created a separation between timing infrastructure and application behavior.

### What Was Accomplished

* Introduced function pointers/callbacks
* Separated scheduling from task behavior
* Created reusable task execution logic
* Learned how callbacks can decouple firmware components
* Began moving toward an event-driven firmware architecture
* Investigated C++ lambda expressions for task callbacks

---

# Milestone 4 — Lambda-Based Task Definitions

**Status:** ✅ Complete

C++ lambdas were introduced as a convenient way to define task behavior directly when creating scheduled tasks.

This allowed task behavior to be expressed without requiring every small task to have a separate named function.

For example, a scheduled task can conceptually contain:

```cpp
[]()
{
    // Task behavior
}
```

The lambda is then executed by the scheduler when the associated timer expires.

During implementation, a lambda-capture issue was encountered involving a variable with static storage duration.

This provided practical experience with:

* Lambda syntax
* Lambda captures
* Variable lifetime
* Scope
* Static storage duration
* Callback compatibility

### What Was Accomplished

* Implemented lambda-based callbacks
* Learned the difference between captured and non-captured variables
* Debugged a C++ lambda compilation issue
* Better understood variable scope and lifetime
* Used lambdas to keep small task definitions close to their scheduling logic
* Further decoupled task execution from scheduler implementation

---

# Milestone 5 — Lightweight Task Scheduler Architecture

**Status:** ✅ Complete

The final milestone established the conceptual architecture for Candy Cadet's software task scheduler.

The firmware now has a structure where periodic tasks can be represented independently and executed when their scheduled interval expires.

Potential firmware tasks were identified as:

* Heartbeat
* Status LED
* Battery monitoring
* Sensor updates
* Servo control
* Motor control
* Communication

The scheduler provides a common mechanism for managing these tasks instead of requiring separate timing logic to be manually implemented throughout the application.

Conceptually:

```text
Application
     │
     ▼
  Scheduler
     │
     ├── Heartbeat Task
     ├── Status Task
     ├── Battery Task
     ├── Sensor Task
     ├── Servo Task
     └── Communication Task
```

### What Was Accomplished

* Established a reusable scheduling model
* Supported multiple independent timed tasks
* Used callbacks to execute task-specific behavior
* Used lambdas where appropriate
* Separated timing infrastructure from application logic
* Created an architecture capable of supporting future firmware subsystems
* Established the foundation for more advanced real-time behavior

---

# Final Architecture

By the completion of Phase 3, the firmware had progressed from a simple timing implementation into a basic scheduling architecture.

The overall flow is:

```text
Application::update()
        │
        ▼
    Scheduler
        │
        ├── Check Heartbeat Timer
        │       └── Execute callback
        │
        ├── Check Status Timer
        │       └── Execute callback
        │
        ├── Check Sensor Timer
        │       └── Execute callback
        │
        └── Check Other Tasks
                └── Execute callback
```

The key architectural principle established during this phase is:

> **The scheduler decides when something runs; the task decides what happens.**

This separation allows new periodic behaviors to be added without rewriting the core timing system.

---

# Engineering Concepts Learned

Phase 3 introduced several important embedded-systems and C++ concepts.

## Embedded Systems

* Non-blocking firmware
* Cooperative scheduling
* Periodic tasks
* Elapsed-time measurement
* Software timers
* Task scheduling
* Event-driven execution
* Timing independence

## C++

* Function pointers
* Callbacks
* Lambda expressions
* Lambda captures
* Variable scope
* Variable lifetime
* Static storage duration

## Software Architecture

* Separation of concerns
* Modular task design
* Decoupling
* Reusable infrastructure
* Application vs. infrastructure responsibilities
* Scalable firmware architecture

---

# Problems & Debugging

Phase 3 also involved working through several implementation issues.

### Blocking vs. Non-Blocking Timing

The initial timing implementation demonstrated how simple `delay()`-based logic can prevent other firmware operations from running.

The solution was to use elapsed-time comparisons with `millis()`.

### Multiple Timers

Adding additional timers required recognizing that each timed behavior needs independent timing state.

This led to the concept of representing timers/tasks as reusable objects or scheduling entries rather than duplicating timing logic throughout the application.

### Lambda Capture Error

While implementing callback-based behavior, a compilation error occurred when a lambda attempted to capture a variable with static storage duration.

Debugging this issue reinforced the importance of understanding:

* Scope
* Lifetime
* Storage duration
* Lambda capture rules

Rather than treating the compiler error as something to work around blindly, the issue was used as an opportunity to understand how C++ manages callback state.

---

# Phase 3 Accomplishments

By the end of this phase, Candy Cadet's firmware had achieved the following:

* ✅ Implemented non-blocking timing using `millis()`
* ✅ Replaced blocking timing logic with elapsed-time scheduling
* ✅ Implemented multiple independent software timers
* ✅ Created independently timed heartbeat and status behaviors
* ✅ Introduced callback-based task execution
* ✅ Implemented C++ lambda callbacks
* ✅ Debugged lambda capture and storage-duration issues
* ✅ Separated task timing from task behavior
* ✅ Established a lightweight task scheduler architecture
* ✅ Created a scalable foundation for future firmware subsystems

---

# Why This Phase Matters

Phase 3 is important because Candy Cadet is no longer structured around individual hardware demonstrations.

The firmware is beginning to behave like a **real embedded system**.

Future subsystems will need to operate simultaneously. For example:

```text
Motor Control       → every few milliseconds
Sensor Sampling     → periodic
Battery Monitoring  → periodic
Servo Updates       → periodic
Communication       → periodic
Heartbeat LED       → periodic
Status Monitoring   → periodic
```

A blocking architecture would make these behaviors difficult to coordinate.

The scheduler architecture developed during this phase provides a foundation for handling these responsibilities while keeping each subsystem modular.

---

# Phase 3 Result

At the conclusion of Phase 3, Candy Cadet has a working foundation for **non-blocking, multi-task firmware scheduling**.

The project progressed from:

```text
Simple LED Timing
        ↓
Non-Blocking Timer
        ↓
Multiple Timers
        ↓
Callbacks
        ↓
Lambdas
        ↓
Task Scheduler
```

This phase established the timing infrastructure that future firmware phases can build upon.

The next firmware work can therefore focus on adding more complex behaviors—such as PWM, motors, servos, sensors, communication, and eventually real-time control—without requiring the timing architecture to be redesigned from scratch.

---

# Lessons Learned

The most important lesson from Phase 3 was that **embedded firmware should be designed around concurrent responsibilities rather than isolated features**.

A timer is not valuable simply because it can blink an LED. Its real value is providing infrastructure that allows many independent systems to operate predictably within the same firmware.

The phase also reinforced the importance of incremental engineering:

1. Start with one timer.
2. Verify that it works.
3. Add independent timers.
4. Abstract the repeated behavior.
5. Introduce callbacks.
6. Introduce lambdas.
7. Build the scheduler.
8. Test each layer independently.

This approach made it possible to learn increasingly advanced firmware concepts without attempting to build the entire scheduling system at once.

---

# Resume-Relevant Accomplishments

Phase 3 produced several resume-worthy engineering accomplishments:

* Developed a reusable non-blocking software scheduling system for an ESP32-based animatronic robot.
* Implemented multiple independent timed tasks using elapsed-time scheduling.
* Designed callback- and lambda-based task execution to decouple application behavior from scheduling infrastructure.
* Structured firmware around modular, event-driven task execution.

---

# Phase 3 Completion Criteria

**Phase 3 is considered complete when:**

* [x] Non-blocking timing is implemented
* [x] Multiple independent timers are supported
* [x] Timed tasks execute independently
* [x] Callbacks are supported
* [x] Lambda-based callbacks are supported
* [x] Scheduling logic is separated from task behavior
* [x] Scheduler architecture is established
* [x] Firmware remains modular and extensible
* [x] Timing infrastructure is ready for future subsystems

---

# Next Phase

With the timing and scheduling foundation established, the firmware can progress toward more advanced hardware control.

The next phase should build on this scheduler to introduce additional embedded functionality while preserving the same architectural principles:

* Modularity
* Non-blocking execution
* Hardware abstraction
* Separation of concerns
* Reusable components
* Testability
* Scalability
* Maintainability

Phase 3 therefore serves as the bridge between **basic GPIO firmware** and a more complete **embedded robotics control system**.
