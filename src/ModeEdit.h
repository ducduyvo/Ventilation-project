/*
 * ModeEdit.h
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#ifndef MODEEDIT_H_
#define MODEEDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include <string>

enum Mode { manual, automatic };

class ModeEdit: public PropertyEdit {
public:
    ModeEdit(LiquidCrystal *lcd_, std::string editTitle, Mode mode);
    virtual ~ModeEdit() { };
    bool increment();
    bool decrement();
    void accept();
    void cancel();
    void setFocus(bool focus);
    void changeState();
    bool getFocus();
    void display();
    Mode getValue();
    Mode getEdit();
    void setValue(Mode value);
    std::string getTitle();
    const char *toString(Mode mode);
private:
    void save();
    void displayEditValue();
    LiquidCrystal *lcd;
    std::string title;
    Mode value;
    Mode edit;
    bool focus;
};

#endif /* MODEEDIT_H_ */
