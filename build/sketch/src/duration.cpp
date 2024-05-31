#line 1 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\src\\duration.cpp"
#include "duration.h"

Duration::Duration(): active(false){

}

bool Duration::BeginRTC(){
    bool success = rtc.begin();
    target_time = rtc.now();
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
}