/*
    Author: Alvin La
    Date: 09/19/2024
    Description:
        This file contains the REATEASystem class 
        to run the rea and tea system. 
*/
#ifndef REA_TEA_H_
#define REA_TEA_H_

#include <math.h>
#include <SPI.h>

#include "SdFat.h"
#include "anemometer.h"
#include "Arduino.h"
#include "timerrtc.h"
#include "lcd_screen.h"
#include "mks_mfc.h"
#include "relay.h"
#include "l298n.h"
#include "ArduinoJson.h"

#define DEBUG false

#define DEFAULT_FLOW_RATE 200

#define SPI_SPEED SD_SCK_MHZ(4)

#define K 0.6 //threshold deadband
#define RHO_AIR 1.18
#define CP_AIR 1005

#define DEFAULT_DATA_COLLECT_TIME 30 //minutes
#define DEFAULT_SEGREGATION_TIME 30 //minutes

#define NUM_OF_RUNS 3

#define ANEM_DATA_SIZE 9
#define ANEM_DATA_BUFFER_SIZE 50

#define NUM_OF_VALVES 8
#define VALVE_PIN 26
#define PUMP_IN1 4
#define PUMP_IN2 5
#define PUMP_EN 9

#define STR_BUFF_SIZE 30

#define DATA_FILE_NAME "wind_data"
#define STATISTIC_FILE_NAME "stats.csv"
#define LOG_FILE_NAME "log.txt"
typedef enum WindStatus{
    up = 0,
    down = NUM_OF_RUNS,
    neutral = NUM_OF_RUNS * 2,
    bypass = NUM_OF_RUNS * 2 + 1,
    none = NUM_OF_RUNS * 2 + 2
} WindStatus;

typedef enum DataEnum{
    U,
    V,
    W,
    Temp,
    wPrime,
    windSpd,
    windDir     
}DataEnum;

typedef enum SampleType{
    rea,
    tea
}SampleType;

struct REATEAData{
    float w_prime;
    double prev_std_dev;
    double prev_avg;
    double prev_min_w;
    double prev_max_w;
    float heat_flux;
    float b_coefficient;
    int N;
    float temperature_avg[2]; //[0] - up, [1] - down
};

struct AnemDataBuffer{
    char date_time[ANEM_DATA_BUFFER_SIZE][21];
    float u[ANEM_DATA_BUFFER_SIZE];
    float v[ANEM_DATA_BUFFER_SIZE];
    float w[ANEM_DATA_BUFFER_SIZE];
    float temp[ANEM_DATA_BUFFER_SIZE];
    float w_prime[ANEM_DATA_BUFFER_SIZE];
    float wind_speed[ANEM_DATA_BUFFER_SIZE];
    float wind_direction[ANEM_DATA_BUFFER_SIZE];
    char wind_status[ANEM_DATA_BUFFER_SIZE][10];
    int buffer_count;
};

class REATEASystem{
    private:
        MksMfc mfc;
        WindStatus wind_status = none;
        Relay valves[NUM_OF_VALVES]; //up1, up2, up3, down1, down2, down3, neutral, bypass
        L298N pump;
        LCDScreen lcd;
        TimerRTC rtc;
        Anemometer anem;
        SdFat sd;
        //RotaryEncoder encoder;
        void WriteDataTOSD();
        File data_file;
        File log_file;
        char data_file_name[STR_BUFF_SIZE];
        struct REATEAData data_var;
        struct AnemDataBuffer anem_buffer;
        struct Time data_collect_time;
        struct Time segreation_time;
        SampleType sample_type;
        void REASampling();
        void LoadSettings();
        void StoreAnemBuffer();
    public:
        REATEASystem();
        void InitializeSDRTC();
        void InitialDataCollection();
        void StartSampling();
};
const char *WindStatusToString(WindStatus status);
#endif //TEA_H_