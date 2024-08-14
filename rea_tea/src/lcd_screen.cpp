#include "lcd_screen.h"

LCDScreen::LCDScreen(): lcd(ADDR, 16, 2){
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.clear();
}


void LCDScreen::PrintFirstLine(char str[17], bool print_second_line){
    strcpy(first_line, str);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(str);
    if(print_second_line){
        lcd.setCursor(0,1);
        lcd.print(second_line);
    }
}
void LCDScreen::PrintSecondLine(char str[17], bool print_first_line){
    strcpy(second_line, str);
    lcd.clear();
    if(print_first_line){
        lcd.setCursor(0,0);
        lcd.print(first_line);
    }
    lcd.setCursor(0,1);
    lcd.print(str);
}

void LCDScreen::PrintBothLine(char firstLine[17], char secondLine[17]){
    strcpy(first_line, firstLine);
    strcpy(second_line, secondLine);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(firstLine);
    lcd.setCursor(0,1);
    lcd.print(secondLine);
}

void LCDScreen::ReprintLines(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(first_line);
    lcd.setCursor(0,1);
    lcd.print(second_line);
}

