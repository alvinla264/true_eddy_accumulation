#ifndef TEA_H_
#define TEA_H_

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

#define SPI_SPEED SD_SCK_MHZ(4)

#define K 0.6 //threshold deadband
#define RHO_AIR 1.18
#define CP_AIR 1005

#define INTIAL_RUN_TIME 30 //minutes
#define SEGREGATION_TIME 30 //minutes

#define NUM_OF_RUNS 3
#define ANEM_DATA_SIZE 6

#define NUM_OF_VALVES 6
#define VALVE_PIN 27
#define PUMP_PIN 26

#define STR_BUFF_SIZE 30

#define DATA_FILE_NAME "wind_data"
#define STATISTIC_FILE_NAME "stats"
typedef enum WindStatus{
    up = 0,
    down = 3,
    neutral = 6,
    bypass = 7,
    none = 8
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
        Relay pump;
        LCDScreen lcd;
        TimerRTC rtc;
        Anemometer anem;
        String folder_name;
        SdFat sd;
        String lcd_line[2];
        RotaryEncoder encoder;
        float anem_data[ANEM_DATA_SIZE]; //u, v, w, temp, w_prime, wind speed, wind direction
        void WriteDataTOSD();
        float w_prime;
        bool in_initial_run;
        File data_file;
        String file_read_str;
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