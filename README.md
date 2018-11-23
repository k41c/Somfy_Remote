# Somfy Remote
This is a library to emulate one or multiple Somfy remote controls via a CC1101 module by Texas Instruments.

If you want to learn more about the Somfy RTS protocol, check out [Pushtack](https://pushstack.wordpress.com/somfy-rts-protocol/).


## How the hardware works
This library is based on a driver library for the CC1101 module: https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib

Connect your CC1101 module according to the wiring instructions for Arduino or ESP32 of the driver library.

## How the software works
Easiest way to make it work for you:  
* Choose a remote name
* Choose a remote number
* Choose your module (Arduino, ESP32)
* Upload the sketch
* Long-press the program button of <b>your actual remote</b> until your blind goes up and down slightly  
* Send 'P' to the <b>simulated remote</b> 

To make a group command, just repeat the last two steps with another blind (one by one)

The rolling code value is stored in the EEPROM, so that you don't loose count of your rolling code after a reset.

### MQTT support
For MQTT support the Basecamp library v0.1.8 is required: https://github.com/merlinschumacher/Basecamp/tree/0.1.8

Please have a look at the library instructions to learn how to setup WiFi and MQTT.

Just send U, D, M or P to the corresponding MQTT topic: "room/sender/iot.hostname/command";