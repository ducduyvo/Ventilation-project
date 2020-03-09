
/*
 * MenuItem.cpp
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#include "HomeScreen.h"
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include "Printer.h"
static Printer printer;

HomeScreen::HomeScreen(LiquidCrystal *lcd_, IntegerEdit *Speed_, IntegerEdit *Pressure_, ModeEdit *mode_) :
    lcd(lcd_), speed(Speed_), pressure(Pressure_), mode(mode_)
{
}

void HomeScreen::display() {
    lcd->clear();
    // First row
    lcd->setCursor(0,0);
    char s[17];
    snprintf(s, 17, "%-6s%-5s%-5s", getModeTitle(), getSpeedTitle(), getPressureTitle());
    lcd->print(s);

    // Second row
    char speedString[16] = "";
    char pressureString[16] = "";
    sprintf(speedString, "%u", speed->getValue());
    sprintf(pressureString, "%d", pressure->getValue());
    strcat(speedString, "%%");
    strcat(pressureString, "pa");

    lcd->setCursor(0,1);
    // second value has to one greater than in the line above because escaping % with "%%" eats
    // one character from the padding
    snprintf(s, 17, "%-6s%-6s%-5s",
             mode->toString(mode->getValue()),
             speedString,
             pressureString
            );
    lcd->print(s);
}

void HomeScreen::event(menuEvent e) {
    switch (e) {
    case ok:
        mode->changeState();
        display();
        break;
    case show:
        display();
        break;
    default:
        break;
    }
}
