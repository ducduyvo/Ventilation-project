
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

HomeScreen::HomeScreen(LiquidCrystal *lcd_, Fan *fan_, Pressure *pressure_, IntegerEdit *targetSpeed_, IntegerEdit *targetPressure_, ModeEdit *mode_) :
    lcd(lcd_),
    fan(fan_),
    pressure(pressure_),
    targetSpeed(targetSpeed_),
    targetPressure(targetPressure_),
    mode(mode_),
    barGraph(lcd)
{
}

void HomeScreen::display()
{
    lcd->clear();
    displayTitles();
    displayMode();
    displayFan();
    displayPressure();
}

void HomeScreen::displayTitles() {
    // First row
    lcd->setCursor(0, 0);
    char buffer[BUF_SIZE];
    snprintf(buffer, BUF_SIZE, "%-6s%-5s%-5s", getModeTitle(), getFanTitle(), getPressureTitle());
    lcd->print(buffer);
}

void HomeScreen::displayMode() {
    lcd->setCursor(0, 1);
    char buffer[7] = "";
    snprintf(buffer, 7, "%-5s", mode->toString(mode->getValue()));
    lcd->print(buffer);
}

void HomeScreen::displayFan() {
    /* lcd->setCursor(5, 1); */
    barGraph.draw2Bars((fan->getSpeed() + 12.5 / 2) / 12.5, mode->getValue() == Mode::manual ? (targetSpeed->getValue() + 12.5 / 2) / 12.5 : 0, 5, 1, 0);
    lcd->setCursor(6, 1);
    char buffer[7] = "";
    sprintf(buffer, "%u", fan->getSpeed());
    strcat(buffer, "%%");

    snprintf(buffer, 7, "%-5s", buffer);
    lcd->print(buffer);
}

void HomeScreen::displayPressure() {
    /* lcd->setCursor(10, 1); */
    barGraph.draw2Bars((pressure->getPressure() + 15 / 2)  / 15, mode->getValue() == Mode::automatic ? (targetPressure->getValue() + 15 / 2) / 15 : 0, 10, 1, 1);
    lcd->setCursor(11, 1);
    char buffer[7] = "";
    sprintf(buffer, "%d", pressure->getPressure());
    strcat(buffer, "pa");

    snprintf(buffer, 7, "%-5s", buffer);
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
