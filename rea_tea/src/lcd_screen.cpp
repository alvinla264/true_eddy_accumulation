#include "lcd_screen.h"

LCDScreen::LCDScreen(): lcd(8,9, 4, 5, 6, 7){
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.clear();
    previousPressed = NotPressed;
}


ButtonPressed LCDScreen:: GetButtonPressed(){
    int button_pressed = analogRead(buttons);
    ButtonPressed pressed;
    if(button_pressed < 60){
        pressed = Right;
    }
    else if(button_pressed < 200){
        pressed = Up_button;
    }
    else if(button_pressed < 400){
        pressed = Down_button;
    }
    else if(button_pressed < 600){
        pressed = Left;
    }
    else if(button_pressed < 800){
        pressed = Select;
    }
    else{
        pressed = NotPressed;
    }
    previousPressed = (pressed == previousPressed) ? NotPressed : pressed;
    return previousPressed;

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

void LCDScreen::InitializeLCD(){
    lcd.begin(16,2);
}

void LCDScreen::TurnOnDisplay(){
    lcd.display();
}

void LCDScreen::TurnOffDisplay(){
    lcd.noDisplay();
}