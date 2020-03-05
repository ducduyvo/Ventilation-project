
/*
 * ModeEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#include "ModeEdit.h"
#include <cstdio>

ModeEdit::ModeEdit(LiquidCrystal *lcd_, std::string editTitle, Mode mode) : lcd(lcd_),
                                                                                      title(editTitle),
                                                                                      value(mode),
                                                                                      edit(mode)
{
    focus = false;
}

ModeEdit::~ModeEdit()
{
}

void ModeEdit::increment()
{
    // TODO: create incrment function
    edit = Mode::automatic;
}

void ModeEdit::decrement()
{
    // TODO: create decrement function
    edit = Mode::manual;
}
void ModeEdit::changeState()
{
}

void ModeEdit::accept()
{
    save();
}

void ModeEdit::cancel()
{
    edit = value;
}

void ModeEdit::setFocus(bool focus)
{
    this->focus = focus;
}

bool ModeEdit::getFocus()
{
    return this->focus;
}

void ModeEdit::display()
{
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print(title);
    lcd->setCursor(0, 1);
    char s[17];
    if (focus)
    {
        snprintf(s, 17, "     [%4s]     ", edit ? "automatic" : "manual"); // TODO do something like edit.getString()
    }
    else
    {
        snprintf(s, 17, "      %4s      ", edit ? "automatic" : "manual");
    }
    lcd->print(s);
}

void ModeEdit::save()
{
    // set current value to be same as edit value
    value = edit;
    // todo: save current value for example to EEPROM for permanent storage
}

ModeEdit::Mode ModeEdit::getValue()
{
    return value;
}

ModeEdit::Mode ModeEdit::getEdit()
{
    return edit;
}

void ModeEdit::setValue(Mode value)
{
    edit = value;
    this->value = edit;
    save();
}

std::string ModeEdit::getTitle()
{
    return title;
}
