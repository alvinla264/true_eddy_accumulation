#ifndef RTC_DURATION_H_
#define RTC_DURATION_H_

#include <RTClib.h>
//struct to keep track of the minute and seconds
typedef struct Time{
    unsigned int min;
    unsigned int sec;
} Time;

class RTCDuration{
    private:
        RTC_DS3231 rtc;
        int duration_time;
        Time start;
        Time end;
        Time adjusted_end;
        Time sec_start;
        bool active;
        DateTime time;
    public:
    /**
     * @brief Construct a new Rea Time object
     * Sets variables to default values
     * 
     */
        RTCDuration();
        /**
         * @brief basically just uses rtc.begin() and returns the values that the methods
         * returns
         * 
         * @return true if sucessfully initilizes rtc
         * @return false if not successful
         */
        bool BeginRTC();
        /**
         * @brief This method uses the rtc to start a non blocking timer for a given amount
         * of minutes that is passed in the parameter. 
         * 
         * @param dur_time minutes you want the timer to run for
         * @return true if the timer is still going on
         * @return false if the timer has ended
         */
        bool Duration(int dur_time);
        /**
         * @brief Will the check the timer to see if a certain amount of time has passed
         * For example if you need to do something every 3 seconds set param to 3 and have
         * an active timer running and every 3 seconds it will return true
         * 
         * @param num_of_sec Number of seconds you want 
         * @return true if that amount of seconds has passed
         * @return false if that amount of seconds has not passed
         */
        bool SecondHasPassed(int num_of_sec = 1);
        /**
         * @brief Gets the time since the timer started starting from 00:00
         * 
         * @return String of the time
         */
        void GetTimeElapsed(char *buff);
        /**
         * @brief allows you to use rtc.now()
         * 
         * @return DateTime 
         */
        DateTime GetCurrentTime();
        /**
         * @brief Turns off the timer in the case that you dont need 
         * to use the full timer
         * 
         */
        void TurnOffDuration();

        void AdjustRTC(int time[5]);

        void AdjustRTC(DateTime new_time);
};

#endif //RTC_DURATION