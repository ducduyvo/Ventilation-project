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

bool ModeEdit::increment()
{
    changeState();
    return true;
}

bool ModeEdit::decrement()
{
    changeState();
    return true;
}

void ModeEdit::changeState()
{
    edit = Mode(!edit);
    accept();
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
    if (focus) {
        snprintf(s, 17, "     [%4s]     ", toString(edit));
    }
    else {
        snprintf(s, 17, "      %4s      ", toString(edit));
    }
    lcd->print(s);
}

void ModeEdit::save()
{
    // set current value to be same as edit value
    value = edit;
    // todo: save current value for example to EEPROM for permanent storage
}

Mode ModeEdit::getValue()
{
    return value;
}

Mode ModeEdit::getEdit()
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

const char *ModeEdit::toString(Mode mode)
{
    if      (mode == Mode::automatic)   return "A";
    else if (mode == Mode::manual)      return "M";
    else                                return "";
}
