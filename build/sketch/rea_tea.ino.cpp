#include <Arduino.h>
#line 1 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\rea_tea.ino"
#include "src/rtc_duration.h"
RTCDuration rtc;
#line 3 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\rea_tea.ino"
void setup();
#line 7 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\rea_tea.ino"
void loop();
#line 3 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\rea_tea.ino"
void setup(){
    rtc.BeginRTC();
}

void loop(){
    DateTime test1 = rtc.GetCurrentTime();
    delay(500);
    DateTime test2 = rtc.GetCurrentTime();
    
}
