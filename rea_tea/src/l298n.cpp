#include "l298n.h"

L298N::L298N(int in1, int in2, int en = -1) : in1_pin(in1), in2_pin(in2), en_pin(en){
    pinMode(in1_pin, OUTPUT);
    pinMode(in2_pin, OUTPUT);
    pinMode(en_pin, OUTPUT);
    digitalWrite(in1_pin, LOW);
    digitalWrite(in2_pin, LOW);
    analogWrite(en_pin, 0);
    en_output = 0;
}

void L298N::forward_dir(){
    digitalWrite(in1_pin, HIGH);
    digitalWrite(in2_pin, LOW);
    curr_dir = forward;
}
void L298N::backwards_dir(){
    digitalWrite(in1_pin, LOW);
    digitalWrite(in2_pin, HIGH);
    curr_dir = backward;
}

void L298N::change_dir(direction dir){
    switch(dir){
        case forward:
            forward_dir();
            break;
        case backward:
            backwards_dir();
            break;
        case Off:
            turn_off();
            break;
    }
}

void L298N::change_speed(int output){
    analogWrite(en_pin, output);
    en_output = output;
}

void L298N::turn_off(){
    analogWrite(en_pin, 0);
    digitalWrite(in1_pin, LOW);
    digitalWrite(in2_pin, LOW);
    en_output = 0;
    curr_dir = Off;
}

void L298N::update_motor(direction dir, int output){
    if(dir == Off || output == 0){
        turn_off();
    }
    else{
        change_speed(output);
        if(dir != no_change) {
            change_dir(dir);
        }
    }
}

