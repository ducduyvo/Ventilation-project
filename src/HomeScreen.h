
/*
 * MenuItem.h
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#ifndef HOMESCREEN_H_
#define HOMESCREEN_H_

#include "IntegerEdit.h"
#include "ModeEdit.h"
#include "LiquidCrystal.h"
#include "MenuItem.h"
#include <string>

class HomeScreen : public MenuItem
{
public:
    HomeScreen(LiquidCrystal *lcd_, IntegerEdit *Speed_, IntegerEdit *Pressure_, ModeEdit *mode_);
    ~HomeScreen(){};
    void setModeTitle(const char *title) { modeTitle = title; }
    void setSpeedTitle(const char *title) { speedTitle = title; }
    void setPressureTitle(const char *title) { pressureTitle = title; }

    const char *getModeTitle() { return modeTitle.c_str(); }
    const char *getSpeedTitle() { return speedTitle.c_str(); }
    const char *getPressureTitle() { return pressureTitle.c_str(); }
    void display();
    void event(menuEvent e);

private:
    LiquidCrystal *lcd;
    IntegerEdit *speed;    // in percent
    IntegerEdit *pressure; // in pascal
    ModeEdit *mode;

    std::string modeTitle = "MODE";
    std::string speedTitle = "S";
    std::string pressureTitle = "P";
};

#endif /* HOMESCREEN_H_ */
