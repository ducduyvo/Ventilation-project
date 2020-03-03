
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


enum controllerMode { manual, automatic }; // TODO Think about where to put this enum (own file?)

class ModeEdit: public PropertyEdit {
public:
	ModeEdit(LiquidCrystal *lcd_, std::string editTitle);
	virtual ~ModeEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	controllerMode getValue();
    controllerMode getEdit();
	void setValue(int value);
    std::string getTitle();
private:
	void save();
	void displayEditValue();
	LiquidCrystal *lcd;
	std::string title;
	controllerMode value;
	controllerMode edit;
	bool focus;
};

#endif /* MODEEDIT_H_ */
