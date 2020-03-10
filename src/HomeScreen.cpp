
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

HomeScreen::HomeScreen(LiquidCrystal *lcd_, IntegerEdit *Speed_, IntegerEdit *Pressure_, ModeEdit *mode_) :
      lcd(lcd_),
      speed(Speed_),
      pressure(Pressure_),
      mode(mode_)
{ }

void HomeScreen::display()
{
    lcd->clear();
    // First row
    lcd->setCursor(0, 0);
    char buffer[BUF_SIZE];
    snprintf(buffer, BUF_SIZE, "%-6s%-5s%-5s", getModeTitle(), getSpeedTitle(), getPressureTitle());
    lcd->print(buffer);

    // Second row
    char speedString[BUF_SIZE] = "";
    char pressureString[BUF_SIZE] = "";
    sprintf(speedString, "%u", speed->getValue());
    sprintf(pressureString, "%d", pressure->getValue());
    strcat(speedString, "%%");
    strcat(pressureString, "pa");

    lcd->setCursor(0, 1);
    // second padding value has to one greater here compared to row above
    // because escaping % with "%%" eats one character from the padding
    snprintf(buffer, BUF_SIZE, "%-6s%-6s%-5s",
             mode->toString(mode->getValue()),
             speedString,
             pressureString);
    lcd->print(buffer);
}

void HomeScreen::event(menuEvent e)
{
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
