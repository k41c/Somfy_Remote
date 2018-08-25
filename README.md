# Somfy Remote
This is an Arduino library able to emulate a Somfy remote control via a CC1101 module by Texas Instruments.

If you want to learn more about the Somfy RTS protocol, check out [Pushtack](https://pushstack.wordpress.com/somfy-rts-protocol/).


**How the hardware works:**
This library is based on a driver library for the CC1101 module: https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib

Connect your CC1101 module according to the driver library:

CC1101 = Arduino

SCK_PIN = 13
MISO_PIN = 12
MOSI_PIN = 11
SS_PIN = 10
GD02 = 6
VCC = 3,3V
GND = GND

**How the software works:**
Easiest way to make it work for you:
    - Choose a remote number
    - Choose a starting point for the rolling code. Any unsigned int works, 1 is a good start
    - Upload the sketch
    - Long-press the program button of YOUR ACTUAL REMOTE until your blind goes up and down slightly
    - send 'p' to the serial terminal
To make a group command, just repeat the last two steps with another blind (one by one)

The rolling code value is stored in the EEPROM, so that you don't loose count of your rolling code after a reset.

Other examples that use MQTT are coming soon.