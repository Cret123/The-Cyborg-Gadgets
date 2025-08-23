*SPDX-License-Identifier: CC-BY-NC-SA-4.0*

# Generation 4

Fourth generation watches make a significant leap forward, being the first to incorporate an MCU (microcontroller unit). The most common MCU used is the Arduino Pro Mini for its compact size and price. However, certain specialized models may utilize alternative Arduino variants.

## Devices

- [Watch 4.0](./Watch4.0/Watch4.0.ino) was the first 4th generation watch to be built. It uses the inbuilt LED to flash sequences at the user.
- [Watch 4.1](./Watch4.1/Watch4.1.ino) uses a neopixel for communication, flashing various colours and sequences for various meanings. 
- [Watch 4.2](./Watch4.2/Watch4.2.ino) shifted to a more advanced display, an SSD1306 12C OLED screen.
- [Watch 4.3](./Watch4.3/Watch4.3.ino) is the first fully independent gadget, featuring an RTC chip to become a fully functional watch.
- [Watch 4.4](./Watch4.4/) utilizes an nRF24L01 transiever module to enable remote control, which helps increase pins, proccessing power and reduces the risk of potential theft.
- [Watch 4.5](./Watch4.5/Watch4.5.ino) is packed with sensors for data collection but does not include outputs.
- [Watch 4.6](./Watch4.6/Watch4.6.ino) is designed to be inconspicuous, with features such as a microphone enabling its use as a spy watch.
- [Watch 4.7](./Watch4.7/Watch4.7.ino) is designed for expeditions, equipped many useful navigation features, most notably a GPS module.

## Installation

1. Gather the required hardware.
2. Download or clone this repository.
3. Install the Arduino IDE.
4. In the Arduino IDE, install any required libraries for your watch model.
5. Open the .ino file for your chosen watch model from the Generation_4 directory.
6. Connect your hardware according to the pin definitions at the top of the chosen file.
7. Upload the code to the arduino.