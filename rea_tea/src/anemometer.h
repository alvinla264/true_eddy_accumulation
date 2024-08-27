/*
File: anemometer.h
Author: Alvin La
Date: 05/30/2024
Description:
This file is to obtain data from the Young81000 anemometer wind sensor via RS232. Using a RS232 to TTL converter to
communicate to the Arduino.
*/
#ifndef ANEMOMETER_H_
#define ANEMOMETER_H_
#include "Arduino.h"
#include "SdFat.h"
#define BAUDRATE 38400
#define ANEM_BUFFER_SIZE 70 //49
#define WAIT_TIME 125 //put delay and wait time in ms
#define DELAY_TIME 5
#define DATA_FREQ 10
class Anemometer{
private:
  String rawString;
  float windSpeed;
  float windDir;
  float northSouth;
  float westEast;
  float upDown;
  float temp;
  float speed_of_sound;
  bool isValid;
  int cycle_limit;
  bool get_cycle_limit;
  char raw_data[ANEM_BUFFER_SIZE];
  HardwareSerial *serial;
public:
  int num_of_cycles;
  Anemometer(HardwareSerial &mySerial);
  /**
   * @brief Get the Raw Data object
   * 
   * will keep calling the readStringUntil func until it gets 
   * valid data
   * 
   * @return raw data as String 
   */
  //String getRawData();
  /**
   * @brief Set the Data object
   * 
   * Sets all the members of the class to param values
   * 
   * @param spd - windSpeed
   * @param dir - windDirection
   * @param nS - northSouth
   * @param wE - westEast
   * @param uD -upDown
   * @param tmp -temp
   */
  void setData(float spd, float dir, float nS, float wE, float uD, float tmp, float soss);
  /**
   * @brief parses the raw data and sets the values with setData func
   * 
   */
  void getData(File log_file, int num_of_retries = 0);
  //void getData_test();
  /**
   * @brief prints the members of the class to param Serial
   * 
   * @param serial - Serial to print the values
   */
  void printData(); 
  bool isValidData(float spd, float dir, float nS, float wE, float uD, float tmp);
  //getter methods
  float getWindSpeed();
  float getWindDir();
  float getNorthSouth();
  float getWestEast();
  float getUpDown();
  float getTemp();
  float getTempK();
  float getSpeedOfSound();
  bool getIsValid();
  //void calculateWaitingTime();
  void InitializeAnem();
};

#endif //ANEMOMETER_H_
