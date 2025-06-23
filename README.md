# Arduino Multitool Watch Extension

This repository contains a series of Arduino programs designed for a customizable multitool watch extension. Each program provides different functions and user interfaces, tailored for various hardware configurations and experimental builds of the watch.

---

## Overview

**What is this project?**

This project is a collection of firmware sketches for a modular watch extension that transforms a basic watch into a smart, multifunctional device. The features and available functions depend on the specific hardware setup and which program you upload to your device. The project demonstrates several embedded systems concepts and offers a platform for experimentation with interactive wearable electronics.

---

## Features

Depending on the design and the `.ino` file you use, your multitool watch extension may include:

- **LED and Laser Control:** Turn on/off, blink, or toggle white and UV LEDs, lasers, and fans.
- **Random Number Generator:** Generate random numbers using physical buttons to set the range and display the output on an OLED screen.
- **Simple Calculator:** Perform basic arithmetic operations with results displayed or indicated via LEDs.
- **Power Management:** Enter low-power or sleep mode to save battery.
- **OLED Display Menus:** Navigate functions and view information on a compact OLED screen.

---

## Hardware Requirements

- **Microcontroller:** Arduino Pro Mini or similar
- **Display:** SSD1306 128x64 OLED (I2C)
- **Inputs:** 3–4 push buttons (varied pinout per design)
- **Outputs:** White LED, UV LED, Laser, Fan (driven via digital output pins)

**Pin assignments and features may vary between files.**  
Always check the `.ino` file comments and pin definitions before connecting your hardware!

---

## Getting Started

1. **Clone or download this repository.**
2. **Open the relevant `.ino` file** (e.g., `MasterWatch.ino`) in the Arduino IDE.
3. **Connect your hardware** according to the pin definitions at the top of the file.
4. **Install required libraries** via the Arduino Library Manager:
   - `U8g2lib` (for OLED display)
   - `LowPower` (for power-saving features, some versions)
5. **Upload the sketch** to your Arduino board.
6. **Interact with your watch extension** using the physical buttons and observe feedback on the OLED and outputs.

---

## Example Use Cases

- **Gadget Watch:** Quickly toggle built-in flashlight, laser pointer, UV LED, or fan from your wrist.
- **Random Number Watch:** Roll random numbers for games or decisions, with range settable via hardware buttons.
- **Wearable Calculator:** Perform simple arithmetic without taking out your phone.
- **Learning Platform:** Experiment with Arduino programming, buttons, displays, and power management on a compact, wearable device.

