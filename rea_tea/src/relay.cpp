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
        digitalWrite(relay_pin, HIGH);
        relay_status = on;
    }
    
}
void Relay::TurnRelayOff(){
    if(relay_status == on){
        digitalWrite(relay_pin, LOW);
        relay_status = off;
    }
    
}
void Relay::SwitchRelayState(){
    if(relay_status){
        relay_status = off;
        digitalWrite(relay_pin, LOW);
    }
    else{
        relay_status = on;
        digitalWrite(relay_pin, HIGH);

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
