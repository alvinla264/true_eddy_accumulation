/*
File: relay.h
Author: Alvin La
Date: 05/30/2024
Description:
This file contains a class to help simplify relay usage. Allows for Turning on and off the relay as well as 
switching the state of the relay.
*/

#ifndef VALVE_H_
#define VALVE_H_

#include "Arduino.h"


typedef enum RelayState{
    off,
    on
} RelayState;
class Relay{
    private:
        int relay_pin;
        RelayState relay_status;
    public:
        Relay();
        /**
         * @brief Construct a new Relay object
         * 
         * Sets the pinMode to output add sets status to LOW
         * 
         * @param pin valve pin
         */
        Relay(int pin);

        /**
         * @brief Get the Relay Status 
         * 
         * @return returns the valve's current status
         */
        int GetRelayStatus();

        /**
         * @brief Get the Relay Pin number
         * 
         * @return returns the valve pin
         */
        int GetRelayPin();
        /**
         * @brief turns the valve on no matter the current state
         * 
         */
        void TurnRelayOn();
        /**
         * @brief turns the valve off no matter the current state
         * 
         */
        void TurnRelayOff();

        /**
         * @brief turns the valve to the state is currently not in
         * 
         */
        void SwitchRelayState();

        /**
         * @brief Prints the pin number the relay is on and the state
         * 
         * @param serial takes in serial to print to that serial port
         */
        void PrintRelayState(Stream &serial);

        //Relay& operator=(const Relay &other_relay);
};

#endif //VALVE_H_