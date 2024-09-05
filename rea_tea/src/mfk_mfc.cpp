#include "mks_mfc.h"

MksMfc::MksMfc(HardwareSerial &mySerial = Serial3, int address = 254, int baudrate = 9600) : serial(&mySerial), addr(address), baud(baudrate){
    serial->begin(baud);
    command.reserve(25);
    response.reserve(25);
}

void MksMfc::GetFullScaleAndUnits(){
    serial->print("@@@" + String(addr) + "U?;FF");
    if(ReadResponse()) flow_units = response;
    serial->print("@@@" + String(addr) + "FS?;FF");
    if(ReadResponse()) full_scale_range = response.toFloat();
    Serial.print("Flow Units: ");
    Serial.println(flow_units);
    Serial.print("Full scale range: ");
    Serial.println(full_scale_range);
}

bool MksMfc::ReadResponse(){
    response = serial->readStringUntil(';');
    serial->readBytes(checksum, 2);
    if(response[6] == 'A'){
        response.remove(0,9);
        return true;
    }
    else return false;
}
bool MksMfc::SetFlow(float flowrate){
    command = "@@@" + String(addr) + "SX!" + String(flowrate) + ";FF";
    serial->print(command);
    return ReadResponse();
}

float MksMfc::GetFlow(){
    command = "@@@" + String(addr) + "FX?;FF";
    serial->print(command);
    if(!ReadResponse()){
        return -0.2 * full_scale_range - 1;
    }
    return response.toFloat();
}