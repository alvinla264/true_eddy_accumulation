#ifndef DURATION_H_
#define DURATION_H_
#include <RTClib.h>
enum TemporalUnit{
    second,
    minute,
    hour,
    day
};
class Duration{
    private:
        RTC_DS3231 rtc;
        DateTime target_time;
        bool active;
        TimeSpan target_timespan;
        DateTime has_seconds_passed_timer;
    public:
        Duration();
        bool BeginRTC();
        bool StartDuration(int dur_time, TemporalUnit unit);
        bool CheckDuration();
        bool HasSecondsPassed(int num_of_sec = 1);
        void GetRemainingTime(char *buff);
        DateTime now();
};

#endif //DURATION_H_ 