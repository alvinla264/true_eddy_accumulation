/*
File mfc.h
Author: Alvin La
Date: 05/30/2024
Description:
This file uses RS-232 to communicate with an ALICAT MFC to set setpoint and retrieve data.
Uses a RS-232 to TTL converter to communicate with an Arduino
*/
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