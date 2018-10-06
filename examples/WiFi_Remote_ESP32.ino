/*   This sketch allows you to emulate a Somfy RTS or Simu HZ remote.
   If you want to learn more about the Somfy RTS protocol, check out https://pushstack.wordpress.com/somfy-rts-protocol/
   
   The rolling code will be stored in EEPROM, so that you can power the ESP off.
   
   Easiest way to make it work for you:
    - Choose a remote number
    - Choose a starting point for the rolling code. Any unsigned int works, 1 is a good start
    - Choose the used module
    - Upload the sketch
    - Long-press the program button of YOUR ACTUAL REMOTE until your blind goes up and down slightly
    - send 'P' to the MQTT topic room/sender/iot.hostname/command (replace iot.hostname by selected hostname)
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
SomfyRemote somfy(0x149739, 77, 2); // <- Change remote, rolling code and module here;

//Variables for the mqtt packages and topics
String controlTopic;
String delaySleepTopic;

//This is used to control if the ESP should enter sleep mode or not
bool delaySleep = false;

void setup() {
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

void loop() {
// Do nothing
}

//This function is called when the MQTT-Server is connected
void onMqttConnect(bool sessionPresent) {
  DEBUG_PRINTLN(__func__);
  //Subscribe to the delay topic
  iot.mqtt.subscribe(delaySleepTopic.c_str(), 0);
  //Subscribe to the control topic
  iot.mqtt.subscribe(controlTopic.c_str(), 0);
}

//This topic is called if an MQTT message is received
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
DEBUG_PRINTLN(__func__);

// Check the message topic
  if (strcmp(topic, controlTopic.c_str()) == 0)  {
    somfy.move(payload[0]);
    EEPROM.commit();
  } 
}