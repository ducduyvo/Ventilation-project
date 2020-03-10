/*
 * Menu.cpp
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#include "Menu.h"
#include "Printer.h"
static Printer printer;

Menu::Menu(HomeScreen *homeScreen_, MenuItem *targetSpeed_, MenuItem *targetPressure_, ModeEdit *currentMode_) : homeScreen(homeScreen_), targetSpeed(targetSpeed_), targetPressure(targetPressure_), currentMode(currentMode_)
{
    items[HOMEPOS] = homeScreen;
    items[SPEEDPOS] = targetSpeed;
    items[PRESSUREPOS] = targetPressure;
    position = HOMEPOS;
}

Menu::~Menu() {}

void Menu::event(MenuItem::menuEvent e)
{
    printer.print("position = %u\n", position);
    items[position]->event(MenuItem::show);
    switch (e)
    {
    case MenuItem::up:
        if (position == HOMEPOS)
        {
            switchPosition();
            items[position]->event(MenuItem::show);
        }

        // we werent on the homescreen so just send the up command
        else
        {
            items[position]->event(e);
        }
        break;

    case MenuItem::down:
        // We are on homescreen so switch the position
        if (position == HOMEPOS)
        {
            switchPosition();
            items[position]->event(MenuItem::show);
        }

        // we werent on the homescreen so just send the up command
        else
        {
            items[position]->event(e);
        }
        break;

    case MenuItem::ok:
        items[position]->event(e);
        if (position != HOMEPOS)
        {
            position = HOMEPOS;
        }

        items[position]->event(MenuItem::show);
        break;

    case MenuItem::back:
        items[position]->event(e);
        position = HOMEPOS;
        homeScreen->display();
        break;

    case MenuItem::show:
        items[position]->event(e);
        break;
    }
}

// Switch the position according to the current mode
void Menu::switchPosition()
{
    if (currentMode->getValue() == Mode::manual)
    {
        position = SPEEDPOS;
    }
    else if (currentMode->getValue() == Mode::automatic)
    {
        position = PRESSUREPOS;
    }
}
