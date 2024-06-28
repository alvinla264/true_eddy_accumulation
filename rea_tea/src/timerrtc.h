#ifndef TIMERRTC_H_
#define TIMERRTC_H_
#include <RTClib.h>
enum TemporalUnit{
    second,
    minute,
    hour,
    day
};
class TimerRTC{
    private:
        RTC_DS3231 rtc;
        DateTime target_time;
        bool active;
        DateTime start_time;
        TimeSpan target_timespan;
        DateTime has_seconds_passed_timer;
    public:
        TimerRTC();
        bool BeginRTC();
        bool StartTimer(int dur_time, TemporalUnit unit);
        bool UpdateTimer();
        bool HasSecondsPassed(int num_of_sec = 1);
        void GetRemainingTime(char *buff);
        void GetElapsedTime(char * buff);
        String GetRemainingTime();
        String GetElapsedTime();
        DateTime now();
};

#endif //TIMERRTC_H_ 