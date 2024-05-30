#include "mfc.h"

Mfc::Mfc(){
  alicatSerial = NULL;
  unitID = "";
  setpoint = 0;
  standard_flow = 0.0;
  volumetric_flow = 0.0;
  absolute_pressure = 0.0;
  temperature = 0.0;
  raw_data = "";
}

Mfc::Mfc(HardwareSerial &ser, String id){
  alicatSerial = &ser;
  unitID = id;
  alicatSerial->begin(BAUDRATE);
  alicatSerial->print("\r\r");
  delay(100);
  alicatSerial->print("\r\r" + unitID + "0\r"); //put setpoint at 0
  delay(100);
  raw_data = "";
  setpoint = 0;
  standard_flow = 0.0;
  volumetric_flow = 0.0;
  absolute_pressure = 0.0;  
  temperature = 0.0;
}

void Mfc::SetFlowRate(float desired_flow){
    setpoint = desired_flow;
    float maxFlow = 500.0;
    float adjustedFlowSet = (setpoint / maxFlow) * 64000.0;
    alicatSerial->print("\r\r" + unitID + (String)adjustedFlowSet + "\r");
}

void Mfc::ReadData(){
    //Serial.println("Start of Get flow");
    alicatSerial->print(unitID + "\r");
    while(!alicatSerial->available()){
      // Serial.println("no data");
    }
    raw_data = alicatSerial->readStringUntil('\r');
    //Serial.println(raw_data);
    String parsed_data[8]; // Array to store the parsed data
    int dataIndex = 0; // Index for storing data in the parsedData array
    // Parse the received data
    for (int i = 0; i < raw_data.length(); i++) {
      if (raw_data[i] != ' ') {
        parsed_data[dataIndex] += raw_data[i];
      } else {
        dataIndex++;
      }
    }
    absolute_pressure = parsed_data[1].toFloat();
    temperature = parsed_data[2].toFloat();
    volumetric_flow = parsed_data[3].toFloat();
    standard_flow = parsed_data[4].toFloat();
}

String Mfc::GetStatus(){
  alicatSerial -> print(unitID + "\r");
  while(!alicatSerial->available());
  String str = alicatSerial->readStringUntil('\n');
  return str;
}