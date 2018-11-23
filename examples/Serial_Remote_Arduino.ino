/*   This sketch allows you to emulate a Somfy RTS or Simu HZ remote.
   If you want to learn more about the Somfy RTS protocol, check out https://pushstack.wordpress.com/somfy-rts-protocol/
   
   The rolling code will be stored in EEPROM, so that you can power the Arduino off.
   
   Easiest way to make it work for you:
    - Choose a remote name
    - Choose a remote number
    - Choose the used module
    - Upload the sketch
    - Long-press the program button of YOUR ACTUAL REMOTE until your blind goes up and down slightly
    - send 'p' to the serial terminal
  To make a group command, just repeat the last two steps with another blind (one by one)
  
  Then:
    - U will make it go up
    - D will make it go down
    - M for MY command
    - P for PROGRAM command
*/

#include <Arduino.h>
#include <Somfy_Remote.h>

SomfyRemote somfy("remote1", 0x131478); // <- Change remote name and remote code here!

void setup()
{
  // Setup Serial
  Serial.begin(115200);

  // Set the used device
  SomfyRemote::setDevice(0); // <- Change the device here -> Arduino(0) || ESP8266(1) || ESP32(2)
}

void loop()
{
  // Check if input is available
  if (Serial.available() > 0)
  {
    char command = (char)Serial.read();
    somfy.move(command);
  }
}