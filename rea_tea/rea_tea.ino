#include "src/rtc_duration.h"
RTCDuration rtc;
void setup(){
    rtc.BeginRTC();
}

void loop(){
    DateTime test1 = rtc.GetCurrentTime();
    delay(500);
    DateTime test2 = rtc.GetCurrentTime();
    
}