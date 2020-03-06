/*
 * SimpleMenu.cpp
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#include "SimpleMenu.h"
#include "Printer.h"

SimpleMenu::SimpleMenu(MenuItem *homeScreen_, MenuItem *targetSpeed_, MenuItem *targetPressure_)
{
    items[HOMEPOS] = homeScreen_;
    items[SPEEDPOS] = targetSpeed_;
    items[PRESSUREPOS ] = targetPressure_;
    position = HOMEPOS;
}

SimpleMenu::~SimpleMenu() {
    // TODO Auto-generated destructor stub
}


void SimpleMenu::event(MenuItem::menuEvent e) {
    if (position == HOMEPOS)  return;


    if(e == MenuItem::ok) position = HOMEPOS;
    else if(e == MenuItem::down) position--;

    if(position < 0) position = items.size() - 1;
    if(position >= (int) items.size()) position = 0;

}

void SimpleMenu::event(homeEvent e) {
    switch (e) {
    case home:
        if (position != HOMEPOS) {
            homeScreen->event(MenuItem::show);
            position = HOMEPOS;
        }
        break;
    case speed:
        if (position != SPEEDPOS) {
            position = SPEEDPOS;
            targetSpeed->event(MenuItem::show);
        }
        break;
    case pressure:
        if (position != PRESSUREPOS) {
            position = PRESSUREPOS;
            targetPressure->event(MenuItem::show);
        }
        break;
    }
}

