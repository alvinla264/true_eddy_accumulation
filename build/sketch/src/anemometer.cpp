#line 1 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\src\\anemometer.cpp"
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
}

// String Anemometer::getRawData(){
//   String data;
//   //reads data and will keep reading data until it gets a valid rawData
//   return serial->readStringUntil('\r');
// }
void Anemometer::getData(){
  int num_of_retries = 0;
  num_of_cycles = 0;
  if(!get_cycle_limit)
    cycle_limit = num_of_cycles + 1;
  float data[7] = {0};
  bool validSpeedOfSound = false;
  //&& num_of_cycles < cycle_limit
    //char *buff = (char*) malloc(sizeof(char*) * ANEM_BUFFER_SIZE);
  while(!validSpeedOfSound && num_of_cycles < cycle_limit ){
    if(!get_cycle_limit)
      cycle_limit = num_of_cycles + 10;
    delay(DELAY_TIME);
    // Serial.print("waiting time: ");
    // Serial.println(num_of_cycles);
    if(serial->available()){
      
      serial->readBytesUntil('\r', raw_data, ANEM_BUFFER_SIZE);
      raw_data[ANEM_BUFFER_SIZE - 1] = '\0';
      //Serial.print("Raw: ");
      //Serial.println(raw_data);
      //Serial.print("buff: ");
      //Serial.println(buff);
      // if(!strcmp(buff, "\0")) {
      //   //Serial.println("in strcmp");
      //   continue;
      // }
      char *token;
      char *rest = raw_data;
      int count = 0;
      while(token = strtok_r(rest, " ", &rest)){
        if((token && count > 6) || (!token && count < 7)){
          // Serial.println("In if statement");
          data[6] = 0;
          token = NULL;
          num_of_cycles = 0;
        }
        data[count++] = atof(token);
        // Serial.print("Data Count: ");
        // Serial.println(count - 1);
        // Serial.print("Data Value: ");
        // Serial.println(data[count - 1]);
        
      }
      validSpeedOfSound = data[6] > 300 && data[6] < 400;
    }
    else{
      num_of_cycles++;
    }
    if(num_of_cycles >= cycle_limit){
      for(int i = 0; i < 7;i++){
        data[i] = 0;
      }
    } 
  }
  //free(buff);
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

void Anemometer::calculateWaitingTime(){
  Serial.println("Calculating Waiting Time");
  float average_cycles = 0;
  int number_of_data = 100;
  for(int i = 0; i < number_of_data; i++){
    getData();
    Serial.print("Cycle #: ");
    Serial.println(num_of_cycles);
    average_cycles += num_of_cycles;
    //printData(Serial);
  }
  average_cycles = average_cycles / (float) number_of_data;
  average_cycles += 2;
  Serial.print("Average Cycles: ");
  Serial.println(average_cycles);
  cycle_limit = (average_cycles * (float) WAIT_TIME) / (float) DELAY_TIME;
  Serial.print("Cycle Limit: ");
  Serial.println(cycle_limit);
  get_cycle_limit = true;
}

void Anemometer::InitializeAnem(){
  serial->end();
  serial->begin(BAUDRATE);
}
