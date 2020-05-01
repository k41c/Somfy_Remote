/* This sketch allows you to emulate a Somfy RTS or Simu HZ remote.
   If you want to learn more about the Somfy RTS protocol, check out https://pushstack.wordpress.com/somfy-rts-protocol/
   
   The rolling code will be stored in EEPROM, so that you can power the ESP off.
   
   Easiest way to make it work for you:
    - Choose a remote name (choose any name you like as it only serves as your personal identifier)
    - Choose a remote code (make sure that you use each code only once across all remotes as it serves as identifier for the motors)
    - Upload the sketch
    - Long-press the program button of YOUR ACTUAL REMOTE until your blind goes up and down slightly
    - Send 'remoteName/PROGRAM' to the serial terminal
  To make a group command, just repeat the last two steps with another blind (one by one)
  
  Then:
    - UP will make it go up
    - DOWN will make it go down
    - MY for MY/STOP command
    - PROGRAM for PROGRAM command
*/

#include <Arduino.h>
#include <Somfy_Remote.h>
#include <EEPROM.h>

#define EEPROM_SIZE 64

// Array storing the multiple remotes
SomfyRemote remotes[] = {
    SomfyRemote("remote1", 0x131171), // <- Change remote name and remote code here!
    SomfyRemote("remote2", 0x089501)  // <- Change remote name and remote code here!
};

void setup()
{
    // Setup Serial and EEPROM
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);
}

void loop()
{
    // check if input is available
    if (Serial.available() > 0)
    {
        // Get string from serial input and divide it into remote name and command
        String serialInput = Serial.readString();
        uint8_t divider = serialInput.indexOf("/");
        String remoteName = serialInput.substring(0, divider);
        String command = (serialInput.substring(divider + 1)).c_str();

        // Send the command via the corresponding remote
        for (uint8_t i = 0; i < sizeof(remotes) / sizeof(remotes[0]); i++)
        {
            if (remotes[i].getName() == remoteName)
            {
                if (command == "UP" || command == "DOWN" || command == "MY" || command == "PROGRAM")
                {
                    remotes[i].move(command);
                }
            }
        }
    }
}