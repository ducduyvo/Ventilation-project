#ifndef MENUITEM_H_
#define MENUITEM_H_

#include "PropertyEdit.h"


class MenuItem {
public:
    enum menuEvent {
        up,
        down,
        ok,
        back,
        show
    };
    MenuItem() { };
    MenuItem(PropertyEdit *property);
    virtual ~MenuItem();
    virtual void event(menuEvent e);

private:
    PropertyEdit *pe;
};

#endif /* MENUITEM_H_ */
