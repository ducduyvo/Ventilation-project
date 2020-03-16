#ifndef MENU_H_
#define MENU_H_
#include <vector>

#include "HomeScreen.h"
#include "MenuItem.h"
#include "ModeEdit.h"

#define HOMEPOS 0
#define SPEEDPOS 1
#define PRESSUREPOS 2
#define MAX_ITEMS 3

class Menu {
public:
    Menu(HomeScreen *homeScreen_, MenuItem *targetSpeed_, MenuItem *targetPressure_, ModeEdit *currentMode_);
    virtual ~Menu();
    void event(MenuItem::menuEvent e);
    bool hasPositionChanged();
    int getPosition() { return position; };

private:
    MenuItem *items[MAX_ITEMS]; // Array holding all menuitems
    HomeScreen *homeScreen;
    MenuItem *targetSpeed;
    MenuItem *targetPressure;
    ModeEdit *currentMode;
    uint8_t previousPosition;
    uint8_t position; // position in the items array, defines for unique positions above the class
    void handleUpOrDown(MenuItem::menuEvent e);
};

#endif /* MENU_H_ */
