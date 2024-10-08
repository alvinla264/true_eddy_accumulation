/*
File: lcd_screen.h
Author: Alvin La
Date: 05/30/2024
Description:
this file is an extension of the LiquidCrystal library to allow for easier printing to the LCD screen.
This file was created for a 16x2 LCD shield for Arduino.
*/
#ifndef LCD_SCREEN_H_
#define LCD_SCREEN_H_
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"

#define ADDR 0x26


class LCDScreen{
    private:
        char first_line[17];
        char second_line[17];
        LiquidCrystal_I2C lcd;
    public:
        
        LCDScreen();

        /**
         * @brief This function prints the string passed in the param 
         * on the first line of the lcd and depending on the boolean in the 
         * second parameter it will print the second_line string saved in the class 
         * on the second line.Also saves the 
         * the string to the respective string field of the class
         * 
         * @param str String to print on the first line of the lcd
         * @param print_second_line if true print second_line string saved in class, else do nothing
         */
        void PrintFirstLine(char str[17], bool print_second_line);
        /**
         * @brief This function prints the string passed in the param 
         * on the second line of the lcd and depending on the boolean in the 
         * second parameter it will print the first_line string saved in the class 
         * on the first line. Also saves the 
         * the string to the respective string field of the class
         * 
         * @param str String to print on the second line of the lcd
         * @param print_first_line if true print first_line string saved in class, else do nothing
         */
        void PrintSecondLine(char str[17], bool print_first_line);
        /**
         * @brief This function prints the two string pass in on the lcd screen. Also saves the 
         * the string to the respective string field of the class
         * 
         * @param firstLine String that is printed in the first line of lcd
         * @param secondLine String that is printed in the second line of lcd
         */
        void PrintBothLine(char firstLine[17], char secondLine[17]);
        /**
         * @brief Takes the Two string fields in the class and prints the string out
         * on their respective lines on the LCD.
         * 
         */
        void ReprintLines();
};

#endif //LCD_REA_H_