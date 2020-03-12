/*
 * IntegerEdit.h
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#ifndef INTEGEREDIT_H_
#define INTEGEREDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include <string>

class IntegerEdit: public PropertyEdit {
public:
    IntegerEdit(LiquidCrystal *lcd_, std::string editTitle, int min_, int max_, int step_);
    virtual ~IntegerEdit();
    bool increment();
    bool decrement();
    void accept();
    void cancel();
    void display();
    int getValue();
    int getEdit();
    void setValue(int value);
    void setTitle(const char *title_) { title = title_; }
    void setSymbol(const char *symbol_) { symbol = symbol_; }
    std::string getTitle();
private:
    void save();
    void displayEditValue();
    LiquidCrystal *lcd;
    std::string title;
    std::string symbol;
    int value;
    int edit;
    int min;
    int max;
    int step;
};

#endif /* INTEGEREDIT_H_ */
