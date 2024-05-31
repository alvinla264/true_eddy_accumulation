#line 1 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\src\\duration.h"
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
    public:
        Duration();
        bool BeginRTC();
        bool StartDuration(int dur_time, TemporalUnit unit);
        bool CheckDuration();
};

#endif //DURATION_H_ 