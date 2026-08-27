# Phase 3 — Firmware Task Scheduler

**Status:** ✅ Complete

---
# Overview

Phase 3 focused on evolving the Candy Cadet firmware from individual timing checks inside `Application::update()` into a reusable **cooperative task scheduler**.

The main architectural goal was to separate:

- **What a behavior does** → Task callback
- **How often it should run** → Task interval
- **When it last ran** → Task `lastRun`
- **When it should execute** → Scheduler

The final architecture is:

```text
Application
    |
    v
Scheduler
    |
    +-- Task
    |    +-- callback
    |    +-- interval
    |    +-- lastRun
    |
    +-- Task
    |    +-- callback
    |    +-- interval
    |    +-- lastRun
    |
    +-- Task
         +-- callback
         +-- interval
         +-- lastRun


### Milestone 1 — Basic `millis()` Timing

The first step was learning how to create periodic firmware behavior using `millis()` without blocking the processor with `delay()`.

A basic timing pattern was:

```cpp
unsigned long heartbeatLastRun = 0;
const unsigned long heartbeatInterval = 500;

unsigned long currMillis = millis();

if (currMillis - heartbeatLastRun >= heartbeatInterval)
{
    heartbeatLastRun = currMillis;
    heartbeatLed.toggle();
}

### Milestone 2 — Multiple Independent Timers

```md
# Milestone 2 — Multiple Independent Timers

The next step was creating multiple independent timing systems.

For example:

```cpp
unsigned long heartbeatLastRun = 0;
const unsigned long heartbeatInterval = 500;

unsigned long statusLastRun = 0;
const unsigned long statusInterval = 1000;