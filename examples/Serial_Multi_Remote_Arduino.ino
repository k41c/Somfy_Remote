/*   This sketch allows you to emulate multiple Somfy RTS or Simu HZ remotes.
   If you want to learn more about the Somfy RTS protocol, check out https://pushstack.wordpress.com/somfy-rts-protocol/
   
   The rolling code will be stored in EEPROM, so that you can power the Arduino off.
   
   Easiest way to make it work for you:
    - Choose a remote name for each remote
    - Choose a remote number for each remote
    - Choose the used module
    - Upload the sketch
    - Long-press the program button of YOUR ACTUAL REMOTE until your blind goes up and down slightly
    - send 'remoteName/p' to the serial terminal
  To make a group command, just repeat the last two steps with another blind (one by one)
  
  Then:
    - U will make it go up
    - D will make it go down
    - M for MY command
    - P for PROGRAM command
*/

#include <Arduino.h>
#include <Somfy_Remote.h>

// Array storing the multiple remotes
SomfyRemote remotes[] = {
    SomfyRemote("remote1", 0x102938), // <- Change remote name and remote code here!
    SomfyRemote("remote2", 0x654783)  // <- Change remote name and remote code here!
};

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
    // Get string from serial input and divide it into remote name and command
    String serialInput = Serial.readString();
    uint8_t divider = serialInput.indexOf("/");
    String remoteName = serialInput.substring(0, divider);
    char command = ((serialInput.substring(divider + 1)).c_str())[0];

    // Send the command via the corresponding remote
    for (uint8_t i = 0; i < sizeof(remotes)/sizeof(remotes[0]); i++)
    {
      if (remotes[i].getName() == remoteName)
      {
        remotes[i].move(command);
      }
    }
  }
}