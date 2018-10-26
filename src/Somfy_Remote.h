#ifndef SOMFY_REMOTE
#define SOMFY_REMOTE

#include <EEPROM.h>
#include <ELECHOUSE_CC1101_RCS_DRV.h>

class SomfyRemote {
  private:
    String _name;
    uint _rollingCode;
    byte _remoteCode;
    uint _module;
    uint _eepromAdress;

    void BuildFrame(byte *frame, byte button);
    void SendCommand(byte *frame, byte sync);
    void send_bitOne();
    void send_bitZero();
    uint getNextEepromAdress();

  public:
    SomfyRemote(String name, uint rollingCode, byte remoteCode, uint module); // Constructor requires name, rolling code, remote code and module
    String getName();
    void move(char button); // Method to send a command (Possible inputs: U, D, M, P)
};
#endif
