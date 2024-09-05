#include "timerrtc.h"

TimerRTC::TimerRTC(): active(false){

}

bool TimerRTC::BeginRTC(){
    bool success = rtc.begin();
    target_time = rtc.now();
    has_seconds_passed_timer = rtc.now();
    return success;
}

int32_t TimerRTC::StartTimer(int dur_time, TemporalUnit unit){
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
    start_time = rtc.now();
    has_seconds_passed_timer = rtc.now();
    return target_timespan.totalseconds();
}

bool TimerRTC::UpdateTimer(){
    if(!active)
        Serial.println("Timer Not Active");
    else 
        if(rtc.now() >= target_time) active = false;
    return active;
}

bool TimerRTC::HasSecondsPassed(int num_of_sec = 1){
    if((rtc.now() - has_seconds_passed_timer).totalseconds() >= num_of_sec){
        has_seconds_passed_timer = rtc.now();
        return true;
    }
    else return false;
}

void TimerRTC::GetRemainingTime(char *buff){
    TimeSpan time = target_time - rtc.now();
    int total_hours = time.hours() + (time.days() * 24);
    if(!total_hours) 
        sprintf(buff, "%02d:%02d", time.minutes(), time.seconds());
    else
        sprintf(buff, "%02d:%02d:%02d", total_hours, time.minutes(), time.seconds());
}
void TimerRTC::GetElapsedTime(char *buff){
    TimeSpan time = rtc.now() - start_time;
    int total_hours = time.hours() + (time.days() * 24);
    if(!total_hours) 
        sprintf(buff, "%02d:%02d", time.minutes(), time.seconds());
    else
        sprintf(buff, "%02d:%02d:%02d", total_hours, time.minutes(), time.seconds());
}
DateTime TimerRTC::now(){
    return rtc.now();
}
