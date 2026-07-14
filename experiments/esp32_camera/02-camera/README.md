# Experiment 02 – Camera Initialization

## Objective

Initialize the OV2640 camera and successfully capture image frames.

This experiment focuses entirely on understanding how the ESP32 camera hardware works. No networking or AI is introduced yet.

---

## Learning Objectives

- Understand camera initialization
- Learn how frame buffers work
- Capture JPEG images
- Understand memory usage (PSRAM)
- Verify camera hardware

---

## Hardware

- Freenove ESP32-WROVER
- OV2640 Camera

---

## Tasks

- [ ] Initialize camera
- [ ] Capture one frame
- [ ] Print image size
- [ ] Release frame buffer
- [ ] Repeat capture

---

## Success Criteria

The serial monitor displays something similar to:

```
Camera initialized.

Captured frame:
Size: 153,241 bytes
Resolution: 640x480
```

---

## Concepts Learned

- esp_camera library
- Camera configuration
- JPEG encoding
- PSRAM
- Frame buffers

---

## Next Experiment

Experiment 03 — Camera Web Server