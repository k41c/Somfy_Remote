# Somfy Remote

This is a library to emulate one or multiple Somfy remote controls via a CC1101 module by Texas Instruments.

If you want to learn more about the Somfy RTS protocol, check out [Pushtack](https://pushstack.wordpress.com/somfy-rts-protocol/).

All credit goes to [Nickduino and his Arduino sketch](https://github.com/Nickduino/Somfy_Remote) that built the foundation of my library.

## Hardware

You need:

- CC1101 433Mhz module by Texas Instruments
- Arduino, ESP8266 or ESP32

Connect your CC1101 module according to the wiring instructions:

- [Arduino](https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib/blob/master/WIRING%20NANO_UNO.jpg)
- [ESP8266](https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib/blob/master/WIRING%20ESP8266.jpg)
- [ESP32](https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib/blob/master/WIRING%20ESP32.jpg)

## Software

This library is based on a driver library for the CC1101 module: https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib

Directly [download this](https://github.com/EinfachArne/Somfy_Remote/archive/master.zip) and the [driver library](https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib/archive/2bff72edbee1ee29cbd134d8b2fc47ba0dc0e5f0.zip) or use [this library via PlatformIO](https://platformio.org/lib/show/6414/Somfy_Remote/installation) where it is defined as dependency.

## How To

For each blind you want to control individually:

- Choose a remote name (choose any name you like as it only serves as your personal identifier)
- Choose a remote code (make sure that you use each code only once across all remotes as it serves as identifier for the motors)
- Upload the sketch
- Long-press the program button of <b>your actual remote</b> until your blind goes up and down slightly
- Send 'PROGRAM' to the <b>simulated remote</b>

To control multiple blinds together:

- Repeat the last two steps with another blind (one by one)
