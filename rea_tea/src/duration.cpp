#include "duration.h"

Duration::Duration(): active(false){

}

bool Duration::BeginRTC(){
    bool success = rtc.begin();
    target_time = rtc.now();
    has_seconds_passed_timer = rtc.now();
    return success;
}

bool Duration::StartDuration(int dur_time, TemporalUnit unit){
    active = true;
    switch(unit){
        case minute:
            target_timespan = TimeSpan(0, 0, dur_time, 0);
            break;
        case hour:
            target_timespan = TimeSpan(0, dur_time, 0, 0);
            break;
        case day:
            target_timespan = TimeSpan(dur_time, 0, 0, 0);
            break;
        case second:
        default:
            target_timespan = TimeSpan(0, 0, 0, dur_time);
            break;
    }
    target_time = DateTime(rtc.now() + target_timespan);
    has_seconds_passed_timer = rtc.now();
}

bool Duration::CheckDuration(){
    if(!active)
        Serial.println("Timer Not Active");
    else 
        if(rtc.now() >= target_time) active = false;
    return active;
}

bool Duration::HasSecondsPassed(int num_of_sec = 1){
    if((rtc.now() - has_seconds_passed_timer).totalseconds() >= num_of_sec){
        has_seconds_passed_timer = rtc.now();
        return true;
    }
    else return false;
}

void Duration::GetRemainingTime(char *buff){
    TimeSpan time = target_time - rtc.now();
    sprintf(buff, "%02d:%02d:%02d", time.hour() + (time.day() * 24), time.minute(), time.second());
}

DateTime Duration::now(){
    return rtc.now();
}