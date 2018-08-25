#ifndef SOMFY_REMOTE
#define SOMFY_REMOTE

#include <Arduino.h>
#include <EEPROM.h>
#include <ELECHOUSE_CC1101_RCS_DRV.h>


class SomfyRemote {
  private:
    uint8_t _rollingCode;
    byte _remoteCode;

    void BuildFrame(byte *frame, byte button);
    void SendCommand(byte *frame, byte sync);
    void send_bitOne();
    void send_bitZero();

  public:
    SomfyRemote(uint8_t rollingCode, byte remoteCode); // Constructor requires rolling code and remote code
    void move(char button); // Method to send a command (Possible inputs: u, d, m, p)
};
#endif
