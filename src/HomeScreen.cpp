
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

// Display the upper row of the lcd screen
void HomeScreen::displayTitles() {
    lcd->setCursor(0, 0);
    char buffer[BUF_SIZE];
    snprintf(buffer, BUF_SIZE, "%-5s%-6s%-6s", getModeTitle(), getFanTitle(), getPressureTitle());
    lcd->print(buffer);
}

void HomeScreen::displayMode() {
    lcd->setCursor(0, 1);
    char buffer[SMALL_SIZE] = "";
    snprintf(buffer, SMALL_SIZE, "%-4s", mode->toString(mode->getValue()));
    lcd->print(buffer);
}

void HomeScreen::displayFan() {
    // + FAN_BAR_RATIO / 2 is used to have little padding
    // otherwise only the one absolute max value would give us full bar
    // also single bar is reached easier
    barGraph.draw2Bars((fan->getSpeed() + FAN_BAR_RATIO / 2) / FAN_BAR_RATIO,
                       mode->getValue() == Mode::manual ? (targetSpeed->getValue() + FAN_BAR_RATIO / 2) / FAN_BAR_RATIO : 0,
                       4, // X cursor position
                       1, // Y cursor position
                       FAN_BAR_REG); // Lcd register where fan bar is stored

    lcd->setCursor(5, 1);
    char buffer[SMALL_SIZE] = "";
    sprintf(buffer, "%u", fan->getSpeed());

    // note that because % has to be escaped with another % it will require one extra padding value
    strcat(buffer, "%%");

    snprintf(buffer, SMALL_SIZE, "%-6s", buffer);
    lcd->print(buffer);
}

void HomeScreen::displayPressure() {
    // + PRESSURE_BAR_RATIO / 2 is used to have little padding
    // otherwise only the one absolute max value would give us full bar
    // also single bar is reached easier
    barGraph.draw2Bars((pressure->getPressure() + PRESSURE_BAR_RATIO / 2)  / PRESSURE_BAR_RATIO,
                       mode->getValue() == Mode::automatic ? (targetPressure->getValue() + PRESSURE_BAR_RATIO / 2) / PRESSURE_BAR_RATIO : 0,
                       10, // X cursor position
                       1,  // Y cursor position
                       PRESSURE_BAR_REG); // Lcd register pressure this is stored

    lcd->setCursor(11, 1);
    char buffer[SMALL_SIZE] = "";
    sprintf(buffer, "%d", pressure->getPressure());
    strcat(buffer, "pa");

    snprintf(buffer, SMALL_SIZE, "%-6s", buffer);
    lcd->print(buffer);
}

void HomeScreen::event(menuEvent e)
{
    switch (e) {
        case ok:
            mode->changeState();
            break;
        case show:
            display();
            break;
        default:
            break;
    }
}
