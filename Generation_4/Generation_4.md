*SPDX-License-Identifier: CC-BY-NC-SA-4.0*

# Generation 4

Fourth generation watches make a significant leap forward, being the first to incorporate an MCU (microcontroller unit). The most common MCU used is the Arduino Pro Mini for its compact size and price. However, specialized models may use other Arduino variants.

## Devices

- [Watch 4.0](./Watch4.0/Watch4.0.ino) was the first 4th generation watch to be built. It uses a neopixel for communication, flashing various colours and sequences for various meanings. 
- [Watch 4.1](./Watch4.1/Watch4.1.ino) made use of technology from its prior generation, utilising an SA56 display. While this improves communication, the amount of pins required to use the display limits other potential features.
- [Watch 4.2](./Watch4.2/Watch4.2.ino) shifted to a more advanced display, an SSD1306 12C OLED screen.
- [Watch 4.3](./Watch4.3/Watch4.3.ino)
- [Watch 4.4](./Watch4.4/)
- [Watch 4.5](./Watch4.5/Watch4.5.ino) is designed for expeditions, designed with many useful navigation features. The most notable amoung them is a GPS module.

## Installation

1. Gather the required hardware.
2. Download or clone this repository.
3. Install the Arduino IDE.
4. In the Arduino IDE, install any required libraries for your watch model.
5. Open the .ino file for your chosen watch model from the Generation_4 directory.
6. Connect your hardware according to the pin definitions at the top of the chosen file.
7. Upload the code to the arduino.