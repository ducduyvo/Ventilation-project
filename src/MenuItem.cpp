#include "MenuItem.h"

MenuItem::MenuItem(PropertyEdit *property): pe(property) { }

MenuItem::~MenuItem() { }

void MenuItem::event(menuEvent e)
{
    bool changed = false;
    switch (e) {
        case ok:
            pe->accept();
            break;
        case back:
            pe->cancel();
            changed = true;
            break;
        case show:
            changed = true;
            break;
        case up:
            changed = pe->increment();
            break;
        case down:
            changed = pe->decrement();
            break;
    }
    if (changed) pe->display();
}

