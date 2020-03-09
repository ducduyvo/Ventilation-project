/*
 * Menu.h
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#ifndef MENU_H_
#define MENU_H_
#include <vector>

#include "HomeScreen.h"
#include "MenuItem.h"

#define HOMEPOS 0
#define SPEEDPOS 1
#define PRESSUREPOS 2
#define MAX_ITEMS 3

class Menu {
public:
	Menu(HomeScreen *homeScreen_, PropertyEdit *targetSpeed_, PropertyEdit *targetPressure_);
	virtual ~Menu();
	void event(menuEvent e);
private:
    MenuItem *items[3];
    HomeScreen *homeScreen;
    PropertyEdit *targetSpeed;
    PropertyEdit *targetPressure;
	uint8_t position;
};

#endif /* MENU_H_ */
