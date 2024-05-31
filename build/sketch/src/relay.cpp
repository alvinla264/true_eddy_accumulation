#line 1 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\src\\relay.cpp"
#include "relay.h"

Relay::Relay(){
    relay_pin = 0;
    relay_status = off;
}

Relay::Relay(int pin){
    relay_pin = pin;
    relay_status = on;
    pinMode(relay_pin, OUTPUT);
    TurnRelayOff();
}

int Relay::GetRelayPin(){
    return relay_pin;
}

int Relay::GetRelayStatus(){
    return relay_status;
}

void Relay::TurnRelayOn(){
    if(relay_status == off){
        digitalWrite(relay_pin, LOW);
        relay_status = on;
    }
    
}
void Relay::TurnRelayOff(){
    if(relay_status == on){
        digitalWrite(relay_pin, HIGH);
        relay_status = off;
    }
    
}
void Relay::SwitchRelayState(){
    if(relay_status){
        relay_status = off;
        digitalWrite(relay_pin, HIGH);
    }
    else{
        relay_status = on;
        digitalWrite(relay_pin, LOW);

    }
}

void Relay::PrintRelayState(Stream &serial){
    serial.print("Relay Status on pin ");
    serial.print(GetRelayPin());
    serial.print(": ");
    if(relay_status){
        serial.println("On");
    }
    else{
        serial.println("Off");
    }
}

// Relay &Relay::operator=(const Relay &other_relay){
//     if(this != &other_relay){
//         this->relay_pin = other_relay.relay_pin;
//         this->relay_status = other_relay.relay_status;
//     }
// }