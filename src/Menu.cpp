/*
 * Menu.cpp
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#include "Menu.h"
#include "Printer.h"

Menu::Menu(HomeScreen *homeScreen_, PropertyEdit *targetSpeed_, PropertyEdit *targetPressure_) :
    homeScreen(homeScreen_), targetSpeed(targetSpeed_), targetPressure(targetPressure_)
{
    position = HOMEPOS;
}

Menu::~Menu() { }


void Menu::event(menuEvent e) {
    switch (e) {
    case home:

        break;
    case up:
        break;
    case down:
        break;
    case ok:
        break;
    }
}

