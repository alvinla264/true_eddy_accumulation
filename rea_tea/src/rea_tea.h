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
#include "rotaryencoder.h"
#include "l298n.h"

#define SPI_SPEED SD_SCK_MHZ(4)

#define K 0.6 //threshold deadband
#define RHO_AIR 1.18
#define CP_AIR 1005

#define INTIAL_RUN_TIME 30 //minutes
#define SEGREGATION_TIME 30 //minutes

#define NUM_OF_RUNS 3
#define ANEM_DATA_SIZE 9

#define NUM_OF_VALVES 6
#define VALVE_PIN 26
#define PUMP_IN1 4
#define PUMP_IN2 5
#define PUMP_EN 9

#define STR_BUFF_SIZE 30

#define DATA_FILE_NAME "wind_data"
#define STATISTIC_FILE_NAME "stats"
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
        String lcd_line[2];
        RotaryEncoder encoder;
        void WriteDataTOSD();
        float w_prime;
        File data_file;
        char data_file_name[STR_BUFF_SIZE];
        double prev_std_dev; 
        double prev_avg;
        float covariance;
        float heat_flux;
        float b_coefficient;
        int N;
        //void ToggleValvesPumps(WindStatus status);
        float temperature_avg[2];
    public:
        REATEASystem();
        bool InitializeSDRTC();
        void InitalRun();
        void REASegregation();
};
const char *WindStatusToString(WindStatus status);
#endif //TEA_H_