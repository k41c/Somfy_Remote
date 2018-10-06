#ifndef SOMFY_REMOTE
#define SOMFY_REMOTE

#include <EEPROM.h>
#include <ELECHOUSE_CC1101_RCS_DRV.h>

class SomfyRemote {
  private:
    uint8_t _rollingCode;
    byte _remoteCode;
    uint8_t _module;

    void BuildFrame(byte *frame, byte button);
    void SendCommand(byte *frame, byte sync);
    void send_bitOne();
    void send_bitZero();

  public:
    SomfyRemote(uint8_t rollingCode, byte remoteCode, uint8_t module); // Constructor requires rolling code, remote code and module
    void move(char button); // Method to send a command (Possible inputs: U, D, M, P)
};
#endif
