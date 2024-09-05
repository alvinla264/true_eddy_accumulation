#include "anemometer.h"

//constructor
Anemometer::Anemometer(HardwareSerial &mySerial){
  //sets baudrate
  //mySerial.begin(BAUDRATE);
  //sets the serial to passed in serial

  serial = &mySerial;
  serial->begin(BAUDRATE);
  //initialize other fields
  this -> setData(0,0,0,0,0,0, 0);
  get_cycle_limit = false;
  cycle_limit = 0;
  //mySerial.end();
  rawString.reserve(ANEM_BUFFER_SIZE);
}

void Anemometer::getData(const File &log_file, int num_of_retries = 0){
  float data[7] = {0};
  while(!serial->available()){
  }
  rawString = serial->readStringUntil('\r');
  rawString.trim();
  int end_index = 0;
  int data_count = 0;
  for(int i = 0; i < 7; i++){
    end_index = rawString.indexOf(" ");
    if(end_index == -1){
      data[i] = rawString.toFloat();
    }
    else{
      data_count++;
      data[i] = rawString.substring(0, end_index).toFloat();
      rawString = rawString.substring(end_index);
      rawString.trim();
    }
  }
  if(!(data[6] > 300 && data[6] < 400) || data_count < 6){
    log_file.print("Error Parsing Data: ");
    log_file.print(data[6]);
    log_file.print(", ");
    log_file.println(num_of_retries);
    log_file.flush();
    getData(log_file, num_of_retries++);
  }
  else
    setData(data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
}

//sets the value
void Anemometer::setData(float spd, float dir, float nS, float wE, float uD, float tmp, float sos){
  isValid = isValidData(spd, dir, nS, wE, uD, tmp);
  this -> windSpeed =  spd;
  this ->windDir = dir;
  this ->northSouth = nS;
  this ->westEast = wE;
  this ->upDown = uD;
  this ->temp = tmp;
  this ->speed_of_sound = sos;
}
//prints the data
//mainly for debugging
void Anemometer::printData(){
  //if(isValid){
    Serial.print("WindSpeed: ");
    Serial.println(windSpeed);
    Serial.print("Wind Direction: ");
    Serial.println(windDir);
    Serial.print("North-South vector: ");
    Serial.println(northSouth);
    Serial.print("West-East Vector: ");
    Serial.println(westEast);
    Serial.print("Up-Down Vector: ");
    Serial.println(upDown);
    Serial.print("Temperature: ");
    Serial.println(temp);
    Serial.println();
  //}
}

bool Anemometer::isValidData(float spd, float dir, float nS, float wE, float uD, float tmp){
  return !((spd == 0 && dir == 0 && nS == 0 && wE == 0 && uD == 0 && tmp == 0) 
      || (spd == -99 && dir == 0 && nS == -99 && wE == -99 && uD == -99  && tmp == -99));
}

float Anemometer::getWindSpeed(){
  return windSpeed;
}

float Anemometer::getWindDir(){
  return windDir;
}

float Anemometer::getNorthSouth(){
  return northSouth;
}

float Anemometer::getWestEast(){
  return westEast;
}

float Anemometer::getUpDown(){
  return upDown;
}

float Anemometer::getTemp(){
  return temp;
}

float Anemometer::getTempK(){
  return temp + 273.15;
}

float Anemometer::getSpeedOfSound(){
  return speed_of_sound;
}

bool Anemometer::getIsValid(){
  return isValid;
}

// void Anemometer::calculateWaitingTime(){
//   Serial.println("Calculating Waiting Time");
//   float average_cycles = 0;
//   int number_of_data = 100;
//   for(int i = 0; i < number_of_data; i++){
//     getData();
//     Serial.print("Cycle #: ");
//     Serial.println(num_of_cycles);
//     average_cycles += num_of_cycles;
//     //printData(Serial);
//   }
//   average_cycles = average_cycles / (float) number_of_data;
//   average_cycles += 2;
//   Serial.print("Average Cycles: ");
//   Serial.println(average_cycles);
//   cycle_limit = (average_cycles * (float) WAIT_TIME) / (float) DELAY_TIME;
//   Serial.print("Cycle Limit: ");
//   Serial.println(cycle_limit);
//   get_cycle_limit = true;
// }

void Anemometer::InitializeAnem(){
  serial->end();
  serial->begin(BAUDRATE);
}
