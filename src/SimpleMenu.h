/*
 * SimpleMenu.h
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#ifndef SIMPLEMENU_H_
#define SIMPLEMENU_H_
#include <vector>

#include "MenuItem.h"
#include "HomeScreen.h"

#define HOMEPOS 0
#define SPEEDPOS 1
#define PRESSUREPOS 2
#define MAX_ITEMS 3

class SimpleMenu {
public:
	enum homeEvent {
		home,
		speed,
        pressure
	};
	SimpleMenu(MenuItem *homeScreen_, MenuItem *targetSpeed_, MenuItem *targetPressure_);
	virtual ~SimpleMenu();
	void addItem(MenuItem *item);
	void event(MenuItem::menuEvent e);
	void event(homeEvent e);
private:
    MenuItem *items[MAX_ITEMS];
	int position;
};

#endif /* SIMPLEMENU_H_ */
