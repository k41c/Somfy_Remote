#include "Somfy_Remote.h"

#define SYMBOL 604
#define UP 0x2
#define MY 0x1
#define DOWN 0x4
#define PROG 0x8

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
uint8_t SomfyRemote::getNextEepromAddress()
{
  currentEppromAddress = currentEppromAddress + 4;
  return currentEppromAddress;
}

// Send a command to the blinds
void SomfyRemote::move(char button)
{
  unsigned int currentRollingCode;
  byte frame[7];

  // Set new rolling code if not already set
  if (EEPROM.get(_eepromAddress, currentRollingCode) < _rollingCode)
  {
    EEPROM.put(_eepromAddress, _rollingCode);
  }
  // Build frame according to selected command
  button = toupper(button);

  if (button == 'U')
  {
    BuildFrame(frame, UP);
  }
  else if (button == 'M')
  {
    BuildFrame(frame, MY);
  }
  else if (button == 'D')
  {
    BuildFrame(frame, DOWN);
  }
  else if (button == 'P')
  {
    BuildFrame(frame, PROG);
  }

  // Send the frame according to Somfy RTS protocol
  SendCommand(frame, 2);
  for (int i = 0; i < 2; i++)
  {
    SendCommand(frame, 7);
  }
}

// Build frame according to Somfy RTS protocol
void SomfyRemote::BuildFrame(byte *frame, byte button)
{
  unsigned int code;
  byte checksum;

  EEPROM.get(_eepromAddress, code);
  frame[0] = 0xA7;              // Encryption key.
  frame[1] = button << 4;       // Selected button. The 4 LSB will be the checksum
  frame[2] = code >> 8;         // Rolling code (big endian)
  frame[3] = code;              // Rolling code
  frame[4] = _remoteCode >> 16; // Remote address
  frame[5] = _remoteCode >> 8;  // Remote address
  frame[6] = _remoteCode;       // Remote address

  // Checksum calculation: a XOR of all the nibbles
  checksum = 0;
  for (byte i = 0; i < 7; i++)
  {
    checksum = checksum ^ frame[i] ^ (frame[i] >> 4);
  }
  checksum &= 0b1111; // Keep the last 4 bits only

  //Checksum integration
  frame[1] |= checksum; //  If a XOR of all the nibbles is equal to 0, the blinds will
                        // consider the checksum ok.

  // Obfuscation: a XOR of all the bytes
  for (byte i = 1; i < 7; i++)
  {
    frame[i] ^= frame[i - 1];
  }

  EEPROM.put(_eepromAddress, code + 1); //  Store the value of the rolling code in the
                                        // EEPROM.
}

// Send frame according to Somfy RTS protocol
void SomfyRemote::SendCommand(byte *frame, byte sync)
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
  for (int i = 0; i < sync; i++)
  {
    digitalWrite(gdo2Pin, HIGH); // PIN 2 HIGH
    delayMicroseconds(4 * SYMBOL);
    digitalWrite(gdo2Pin, LOW); // PIN 2 LOW
    delayMicroseconds(4 * SYMBOL);
  }

  // Software sync
  digitalWrite(gdo2Pin, HIGH); // PIN 2 HIGH
  delayMicroseconds(4550);
  digitalWrite(gdo2Pin, LOW); // PIN 2 LOW
  delayMicroseconds(SYMBOL);

  //Data: bits are sent one by one, starting with the MSB.
  for (byte i = 0; i < 56; i++)
  {
    if (((frame[i / 8] >> (7 - (i % 8))) & 1) == 1)
    {
      send_bitOne();
    }
    else
    {
      send_bitZero();
    }
  }
  digitalWrite(gdo2Pin, LOW); // PIN 2 LOW
  delayMicroseconds(30415);   // Inter-frame silence
}

// Send bit zero
void SomfyRemote::send_bitZero()
{
  // Somfy RTS bits are manchester encoded: 0 = high->low
  digitalWrite(gdo2Pin, HIGH); // PIN 2 HIGH
  delayMicroseconds(SYMBOL);
  digitalWrite(gdo2Pin, LOW); // PIN 2 LOW
  delayMicroseconds(SYMBOL);
}

// Send bit one
void SomfyRemote::send_bitOne()
{
  // Somfy RTS bits are manchester encoded: 1 = low->high
  digitalWrite(gdo2Pin, LOW); // PIN 2 LOW
  delayMicroseconds(SYMBOL);
  digitalWrite(gdo2Pin, HIGH); // PIN 2 HIGH
  delayMicroseconds(SYMBOL);
}
