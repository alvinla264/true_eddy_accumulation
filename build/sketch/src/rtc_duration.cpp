#line 1 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\src\\rtc_duration.cpp"
#include "rtc_duration.h"

RTCDuration::RTCDuration(){
    duration_time = 0;
    active = false;
    start.min = 0;
    start.sec = 0;
    end.min = 0;
    end.sec = 0;
    adjusted_end.min = 0;
    adjusted_end.sec = 0;
    sec_start.min = 0;
    sec_start.sec = 0;
}

bool RTCDuration::BeginRTC(){
    bool temp = rtc.begin();
    time = rtc.now();
    return temp;
}
bool RTCDuration::Duration(int dur_time){
    //gets current time on the rtc
    DateTime now = rtc.now();
    time = now;
    //checks if a new timer is being started or if there is already an active timer
    //if new timer set all Time struct var to current time
    if(duration_time == 0 && !active){
        duration_time = dur_time;
        start.min = now.minute();
        end.min = now.minute();
        start.sec = now.second();
        end.sec = now.second();
        sec_start.min = now.minute();
        sec_start.sec = now.second();
        active = true; 
    }
    //since we dont want end time to be smaller which will happen
    //if it passes the hour line(ex started at 1:53, end is at 2:03 so end min would be 63
    //since 10 min has passed). 
    adjusted_end.min = (end.min < start.min) ? end.min + 60 : end.min;
    //basically the same concept as the previous line but with seconds. Also need to subtract from
    //the min since we are basically just adding the seconds back in
    if(end.sec < start.sec){
        adjusted_end.min --;
        adjusted_end.sec = end.sec + 60;
    }
    else{
        adjusted_end.sec = end.sec;
    }
    //gets new end minute and seconds
    //start should always be the same since its when the timer started
    end.min = now.minute();
    end.sec = now.second();
    //checks if the timer has ended
    active = (adjusted_end.min - start.min) < duration_time;
    //resets the timer variable
    if(!active){
        duration_time = 0;
        start.min = 0;
        start.sec = 0;
        end.min = 0;
        end.sec = 0;
        adjusted_end.min = 0;
        adjusted_end.sec = 0;
        sec_start.min = 0;
        sec_start.sec = 0;
    }
    return active;
}
//checks if a certain amount of seconds has passed since the timer started or
//since the last time it checked for seconds
bool RTCDuration::SecondHasPassed(int num_of_sec = 1){
    //if the end sec is 0 it means 1 min has passed so we need to add the minute back in
    //terms of seconds
    int end_sec_new = (end.sec == 0) ? end.sec + 60 : end.sec;
    //if the difference is bigger than the number of sec - 1 than that much time has passed
    if(end_sec_new - sec_start.sec > num_of_sec - 1){
        DateTime now = rtc.now();
        sec_start.min = now.minute();
        sec_start.sec = now.second();
        return true;
    }
    return false;
}

void RTCDuration::GetTimeElapsed(char *buff){
    //char *buff = (char*)malloc(sizeof(char) * 49);
    //the difference between the adjusted end and start will give us how long the timer 
    //has been up for
    sprintf(buff, "%d:%02d", adjusted_end.min - start.min, adjusted_end.sec - start.sec);
}

DateTime RTCDuration::GetCurrentTime(){
    return rtc.now();
}

void RTCDuration::TurnOffDuration(){
    duration_time = 0;
    start.min = 0;
    start.sec = 0;
    end.min = 0;
    end.sec = 0;
    adjusted_end.min = 0;
    adjusted_end.sec = 0;
    sec_start.min = 0;
    sec_start.sec = 0;
    active = false;
}

void RTCDuration::AdjustRTC(int time[5]){
    rtc.adjust(DateTime(time[2], time[0], time[1], time[3], time[4], 0));
}

void RTCDuration::AdjustRTC(DateTime new_time){
    Serial.println("in rtc adjust");
    rtc.adjust(new_time);
}