
/*
 * MenuItem.cpp
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#include "HomeScreen.h"
#include <cstdio>
#include <stdio.h>

MenuItem::MenuItem(PropertyEdit *property): pe(property) { }

HomeScreen::HomeScreen(LiquidCrystal *lcd_, IntegerEdit *Speed_, IntegerEdit *Pressure_, ModeEdit *mode_) :
    lcd(lcd_), speed(Speed_), pressure(Pressure_), mode(mode_)
{
}

void HomeScreen::display() {
    lcd->clear();
    // First row
    lcd->setCursor(0,0);
    char s[17];
    snprintf(s, 17, "%5s, %5s, %5s", getModeTitle(), getSpeedTitle(), getPressureTitle());
    lcd->print(s);

    // Second row
    lcd->setCursor(0,1);
    snprintf(s, 17, "%5s, %5u, %5d", mode->toString(mode->getValue()), speed->getValue(), pressure->getValue());
    lcd->print(s);
}

bool HomeScreen::event(menuEvent e) {

    switch (e) {
        case show:
            break;
        case ok:
            mode->changeState();
        default:
            break;
    }
}
