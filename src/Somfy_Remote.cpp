#include "Somfy_Remote.h"

const uint8_t symbol = 604;

uint8_t currentEppromAddress = 0;
uint8_t gdo0Pin;
uint8_t gdo2Pin;

// Constructor
SomfyRemote::SomfyRemote(String name, byte remoteCode)
{
  _name = name;
  _remoteCode = remoteCode;
  _rollingCode = 1;
  _eepromAddress = getNextEepromAddress();
}

// Getter for name
String SomfyRemote::getName()
{
  return _name;
}

// Generates the next available EEPROM address
uint16_t SomfyRemote::getNextEepromAddress()
{
  // Every address gets 4 bytes of space to save the rolling code
  currentEppromAddress = currentEppromAddress + 4;
  return currentEppromAddress;
}

// Send a command to the blinds
void SomfyRemote::move(char button)
{
  const uint8_t up = 0x2;
  const uint8_t down = 0x4;
  const uint8_t my = 0x1;
  const uint8_t prog = 0x8;

  uint16_t currentRollingCode;
  byte frame[7];

  // Set new rolling code if not already set
  if (EEPROM.get(_eepromAddress, currentRollingCode) < _rollingCode)
  {
    EEPROM.put(_eepromAddress, _rollingCode);
  }
  // Build frame according to selected command
  button = toupper(button);

  switch (button)
  {
  case 'U':
    buildFrame(frame, up);
    break;
  case 'D':
    buildFrame(frame, down);
    break;
  case 'M':
    buildFrame(frame, my);
    break;
  case 'P':
    buildFrame(frame, prog);
    break;
  default:
    buildFrame(frame, my);
    break;
  }

  // Send the frame according to Somfy RTS protocol
  sendCommand(frame, 2);
  for (int i = 0; i < 2; i = i + 1)
  {
    sendCommand(frame, 7);
  }
}

// Build frame according to Somfy RTS protocol
void SomfyRemote::buildFrame(byte *frame, byte button)
{
  unsigned int code;
  byte checksum;

  EEPROM.get(_eepromAddress, code);
  frame[0] = 0xA7;              // Encryption key.
  frame[1] = button << 4;       // Selected button. The 4 LSB are the checksum
  frame[2] = code >> 8;         // Rolling code (big endian)
  frame[3] = code;              // Rolling code
  frame[4] = _remoteCode >> 16; // Remote address
  frame[5] = _remoteCode >> 8;  // Remote address
  frame[6] = _remoteCode;       // Remote address

  // Checksum calculation (XOR of all nibbles)
  checksum = 0;
  for (byte i = 0; i < 7; i = i + 1)
  {
    checksum = checksum ^ frame[i] ^ (frame[i] >> 4);
  }
  checksum &= 0b1111; // Keep the last 4 bits only

  //Checksum integration
  frame[1] |= checksum; //  If a XOR of all the nibbles is equal to 0, the blinds will
                        // consider the checksum ok.

  // Obfuscation (XOR of all bytes)
  for (byte i = 1; i < 7; i = i + 1)
  {
    frame[i] ^= frame[i - 1];
  }

  EEPROM.put(_eepromAddress, code + 1); //  Store the value of the rolling code in the
                                        // EEPROM.
}

// Send frame according to Somfy RTS protocol
void SomfyRemote::sendCommand(byte *frame, byte sync)
{
  if (sync == 2)
  { // Only with the first frame.

// Set pins according to module
#ifdef __AVR_ATmega168__ || __AVR_ATmega328P__
    gdo0Pin = 2;
    gdo2Pin = 3;
#elif ESP32 || ESP8266
    gdo0Pin = 2;
    gdo2Pin = 4;
#endif

    ELECHOUSE_cc1101.setGDO(gdo0Pin, gdo2Pin);

    // Initialize radio chip
    ELECHOUSE_cc1101.Init(PA10);

    // Enable transmission at 433.42 MHz
    ELECHOUSE_cc1101.SetTx(433.42);

    // Wake-up pulse & Silence
    digitalWrite(gdo2Pin, HIGH); // High
    delayMicroseconds(9415);
    digitalWrite(gdo2Pin, LOW); // Low
    delayMicroseconds(89565);
  }

  // Hardware sync: two sync for the first frame, seven for the following ones.
  for (int i = 0; i < sync; i = i + 1)
  {
    digitalWrite(gdo2Pin, HIGH); // PIN 2 HIGH
    delayMicroseconds(4 * symbol);
    digitalWrite(gdo2Pin, LOW); // PIN 2 LOW
    delayMicroseconds(4 * symbol);
  }

  // Software sync
  digitalWrite(gdo2Pin, HIGH); // PIN 2 HIGH
  delayMicroseconds(4550);
  digitalWrite(gdo2Pin, LOW); // PIN 2 LOW
  delayMicroseconds(symbol);

  //Data: bits are sent one by one, starting with the MSB.
  for (byte i = 0; i < 56; i = i + 1)
  {
    if (((frame[i / 8] >> (7 - (i % 8))) & 1) == 1)
    {
      sendBit(true);
    }
    else
    {
      sendBit(false);
    }
  }
  digitalWrite(gdo2Pin, LOW); // PIN 2 LOW
  delayMicroseconds(30415);   // Inter-frame silence
}

// Send one bit
void SomfyRemote::sendBit(bool value)
{
    uint8_t firstState;
    uint8_t secondState;

// Decide which bit to send (Somfy RTS bits are manchester encoded: 0 = high->low 1 = low->high)
  if (value == true) {
    firstState = LOW;
    secondState = HIGH;
  }
  else if(value == false) {
    firstState = HIGH;
    secondState = LOW;
  }

  // Send the bit
  digitalWrite(gdo2Pin, firstState);
  delayMicroseconds(symbol);
  digitalWrite(gdo2Pin, secondState);
  delayMicroseconds(symbol);
}