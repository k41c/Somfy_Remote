# Somfy Remote

This is a library to emulate one or multiple Somfy remote controls via a CC1101 module by Texas Instruments.

If you want to learn more about the Somfy RTS protocol, check out [Pushtack](https://pushstack.wordpress.com/somfy-rts-protocol/).

All credit goes to [Nickduino and his Arduino sketch](https://github.com/Nickduino/Somfy_Remote) that built the foundation of my library.

## Hardware

You need:

- CC1101 433Mhz module by Texas Instruments
- Arduino, ESP8266 or ESP32

Connect your CC1101 module according to the wiring instructions:

- [Arduino](https://github.com/LSatan/SmartRC-CC1101-Driver-Lib/blob/master/img/Nano_CC1101.png)
- [ESP8266](https://github.com/LSatan/SmartRC-CC1101-Driver-Lib/blob/master/img/Esp8266_CC1101.png)
- [ESP32](https://github.com/LSatan/SmartRC-CC1101-Driver-Lib/blob/master/img/Esp32_CC1101.png)

## Software

This library is based on a driver library for the CC1101 module: https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib

Directly [download it](https://github.com/EinfachArne/Somfy_Remote/archive/master.zip) and the [driver library](https://github.com/LSatan/SmartRC-CC1101-Driver-Lib/archive/933020a9f8788ef11173278fd5396c16f8939785.zip) or use [this library via PlatformIO](https://platformio.org/lib/show/6414/Somfy_Remote/installation).

## How To

For each blind you want to control individually:

- Choose a remote name (choose any name you like as it only serves as your personal identifier)
- Choose a remote code (make sure that you use each code only once across all remotes as it serves as identifier for the motors)
- Upload the sketch
- Long-press the program button of <b>your actual remote</b> until your blind goes up and down slightly
- Send 'PROGRAM' to the <b>simulated remote</b>

To control multiple blinds together:

- Repeat the last two steps with another blind (one by one)
