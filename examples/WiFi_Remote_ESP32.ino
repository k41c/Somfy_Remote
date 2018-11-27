/*   This sketch allows you to emulate a Somfy RTS or Simu HZ remote.
   If you want to learn more about the Somfy RTS protocol, check out https://pushstack.wordpress.com/somfy-rts-protocol/
   
   The rolling code will be stored in EEPROM, so that you can power the ESP off.
   
   Easiest way to make it work for you:
    - Choose a remote name
    - Choose a remote number
    - Upload the sketch
    - Long-press the program button of YOUR ACTUAL REMOTE until your blind goes up and down slightly
    - Send 'remoteName/p' to the MQTT topic room/sender/iot.hostname/command (replace iot.hostname by selected hostname)
  To make a group command, just repeat the last two steps with another blind (one by one)
  
  Then:
    - U will make it go up
    - D will make it go down
    - M for MY command
    - P for PROGRAM command
*/
#define DEBUG 1
#define EEPROM_SIZE 64

#include <Arduino.h>
#include <Basecamp.hpp>
#include <Somfy_Remote.h>
#include <EEPROM.h>

// Object for MQTT communication
Basecamp iot;

// Object for blind control
SomfyRemote somfy("remote1", 0x149739); // <- Change remote name and remote code here;

// Variables for the mqtt topics
String controlTopic;

void setup()
{
  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);

  // Initialize Basecamp
  iot.begin();

  //Configure the MQTT topics
  controlTopic = "room/sender/" + iot.hostname + "/command";

  //Set up the Callbacks for the MQTT instance. Refer to the Async MQTT Client documentation
  iot.mqtt.onConnect(onMqttConnect);
  iot.mqtt.onMessage(onMqttMessage);
}

void loop()
{
  // Do nothing
}

//This function is called when the MQTT-Server is connected
void onMqttConnect(bool sessionPresent)
{
  DEBUG_PRINTLN(__func__);
  //Subscribe to the control topic
  iot.mqtt.subscribe(controlTopic.c_str(), 0);
}

//This function is called if an MQTT message is received
void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  DEBUG_PRINTLN(__func__);

  // Check the message topic
  if (strcmp(topic, controlTopic.c_str()) == 0)
  {
    somfy.move(payload[0]);
    EEPROM.commit();
  }
}