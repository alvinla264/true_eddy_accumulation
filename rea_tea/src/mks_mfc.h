#ifndef MKS_MFC_H_
#define MKS_MFC_H_
#include "Arduino.h"
 
class MksMfc{
    private:
        int addr;
        int baud;
        float full_scale_range;
        String flow_units;
        HardwareSerial *serial;
        String command;
        String response;
        char checksum[3];
        bool ReadResponse();
    public:
        MksMfc(HardwareSerial &mySerial = Serial3, int address = 254, int baudrate = 9600);
        bool SetFlow(float flowrate);
        float GetFlow();
        void GetFullScaleAndUnits();
};

#endif //MKS_MFC_H_