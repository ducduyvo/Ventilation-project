#include "Menu.h"

Menu::Menu(HomeScreen *homeScreen_, MenuItem *targetSpeed_, MenuItem *targetPressure_, ModeEdit *currentMode_) :
    homeScreen(homeScreen_),
    targetSpeed(targetSpeed_),
    targetPressure(targetPressure_),
    currentMode(currentMode_)
{
    items[HOMEPOS] = homeScreen;
    items[SPEEDPOS] = targetSpeed;
    items[PRESSUREPOS] = targetPressure;
    position = HOMEPOS;
    previousPosition = position;
}

Menu::~Menu() { }

void Menu::event(MenuItem::menuEvent e)
{
    switch (e) {
        case MenuItem::up:
            handleUpOrDown(e);
            break;

        case MenuItem::down:
            handleUpOrDown(e);
            break;

        case MenuItem::ok:
            items[position]->event(e);
            // if we are not on home screen we want to go back to it
            if (position != HOMEPOS) {
                position = HOMEPOS;
            }

            items[position]->event(MenuItem::menuEvent::show);
            break;

        // Always goes to back to homescreen
        // Menuitems are not going to save their modified value
        case MenuItem::back:
            if (position != HOMEPOS) {
                items[position]->event(e);
                position = HOMEPOS;
            }
            homeScreen->display();

            break;

        case MenuItem::show:
            items[position]->event(e);
            break;
    }
}

void Menu::handleUpOrDown(MenuItem::menuEvent e)
{
    // We are on homescreen so we want to change the screen view according to
    // the current mode thus we change the position variable
    if (position == HOMEPOS) {
        if (currentMode->getValue() == Mode::manual) {
            position = SPEEDPOS;
        }
        else if (currentMode->getValue() == Mode::automatic) {
            position = PRESSUREPOS;
        }

        // call show event for correct menuitem
        items[position]->event(MenuItem::show);
    }

    // we were on either pressure or speed menuitem
    // so just send the up/down event and let them handle it
    // menuitem is also going to do the screen updating if it has to
    else {
        items[position]->event(e);
    }
}

bool Menu::hasPositionChanged() {
    bool changed = position != previousPosition;
    previousPosition = position;
    return changed;
}
