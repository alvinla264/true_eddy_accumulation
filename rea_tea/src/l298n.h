#ifndef L298N_H_
#define L298N_H_

#include "Arduino.h"

enum direction{
    forward = 0,
    backward,
    Off,
    no_change
};
class L298N{
    private:
        int in1_pin;
        int in2_pin;
        int en_pin;
        direction curr_dir;
        int en_output;
        void forward_dir();
        void backwards_dir();
    public:
        L298N(int in1, int in2, int en = -1);
        void change_dir(direction dir);
        void change_speed(int output);
        void turn_off();
        void update_motor(direction dir, int output);
};

#endif //L289N_H_ file