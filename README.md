# Somfy Remote
This is a library to emulate one or multiple Somfy remote controls via a CC1101 module by Texas Instruments.

If you want to learn more about the Somfy RTS protocol, check out [Pushtack](https://pushstack.wordpress.com/somfy-rts-protocol/).


## How the hardware works
This library is based on a driver library for the CC1101 module: https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib

Connect your CC1101 module according to the wiring instructions for Arduino or ESP32 of the driver library.

## How the software works
Easiest way to make it work for you:  
* Choose a remote name (choose any name you like as it only serves as your personal identifier)
* Choose a remote code (make sure that you use each code only once across all remotes as it serves as identifier for the motors)
* Upload the sketch
* Long-press the program button of <b>your actual remote</b> until your blind goes up and down slightly  
* Send 'PROGRAM' to the <b>simulated remote</b> 

To make a group command, just repeat the last two steps with another blind (one by one)

The rolling code value is stored in the EEPROM, so that you don't loose count of your rolling code after a reset.