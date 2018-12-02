#ifndef SOMFY_REMOTE
#define SOMFY_REMOTE

#include <EEPROM.h>
#include <ELECHOUSE_CC1101_RCS_DRV.h>

class SomfyRemote {
  private:
    String _name;
    byte _remoteCode;
    uint32_t _rollingCode;
    uint16_t _eepromAddress;

    void buildFrame(byte *frame, byte button);
    void sendCommand(byte *frame, byte sync);
    void sendBit(bool value);
    uint16_t getNextEepromAddress();

  public:
    SomfyRemote(String name, byte remoteCode); // Constructor requires name, remote code and used module
    String getName(); // Getter for name
    void move(char button); // Method to send a command (Possible inputs: U, D, M, P)
};
#endif
