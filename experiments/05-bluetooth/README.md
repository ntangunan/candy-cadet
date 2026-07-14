# Experiment 05 – Bluetooth Communication

## Objective

Establish reliable two-way Bluetooth communication between the MacBook and the ESP32.

The camera and Bluetooth are intentionally developed as separate systems before being integrated into the robot.

---

## Learning Objectives

- Learn Bluetooth communication
- Design communication protocols
- Send commands
- Receive telemetry
- Debug wireless communication

---

## Hardware

- ESP32-WROVER
- MacBook

---

## Tasks

### Phase 1

- [ ] Pair ESP32 with MacBook
- [ ] Send text from MacBook
- [ ] Print received text

### Phase 2

- [ ] Send data from ESP32
- [ ] Display data on MacBook

### Phase 3

- [ ] Implement command protocol

Commands:

```
MOVE_FORWARD
MOVE_BACKWARD
TURN_LEFT
TURN_RIGHT
STOP
STATUS
```

---

## Success Criteria

The MacBook sends commands that the ESP32 correctly receives and acknowledges.

Example:

```
MacBook:
MOVE_FORWARD

↓

ESP32:

Executing MOVE_FORWARD
```

---

## Concepts Learned

- Bluetooth Classic
- BLE (future)
- Serial communication
- Packet protocols
- Command parsing

---

## Future Integration

Eventually this communication channel will connect the robot's AI running on the MacBook to the embedded controllers responsible for motion and hardware.