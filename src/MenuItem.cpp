/*
 * MenuItem.cpp
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#include "MenuItem.h"

MenuItem::MenuItem(PropertyEdit *property): pe(property) {

}

MenuItem::~MenuItem() {

}

void MenuItem::event(menuEvent e) {
    bool handled = true;
    switch(e) {
    case ok:
        pe->accept();
        break;
    case back:
        pe->cancel();
        break;
    case show:
        break;
    case up:
        pe->increment();
        break;
    case down:
        pe->decrement();
        break;
    }
    if(handled) pe->display();
}

