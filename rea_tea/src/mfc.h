#ifndef MFC_H_
#define MFC_H_
#define BAUDRATE 19200
#include "Arduino.h"
class Mfc{
    private:
        HardwareSerial *alicatSerial;
        String unitID;
        String raw_data;
        float setpoint;
        float absolute_pressure;
        float temperature;
        float volumetric_flow;
        float standard_flow;

    public:
        Mfc(HardwareSerial &ser, String id);
        Mfc();
        void SetFlowRate(float desired_flow);
        void ReadData();
        float GetAbsolutePressure(){return absolute_pressure;}
        float GetTemperature(){return temperature;}
        float GetVolumetricFlow(){return volumetric_flow;}
        float GetStandardFlow(){return standard_flow;}
        String GetStatus();
};

#endif //MFC_H_