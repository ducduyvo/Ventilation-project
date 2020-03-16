/*
 * BarGraph.cpp
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#include "BarGraph.h"

BarGraph::BarGraph(LiquidCrystal *lcd_): lcd(lcd_)
{
    lcd->setCursor(0,0); // move cursor back to visible area
}

void BarGraph::draw2Bars(int8_t leftBar, int8_t rightBar, uint8_t curX, uint8_t curY, uint8_t reg) {
    if (leftBar < 0) leftBar = 0;
    if (rightBar < 0) rightBar = 0;
    if (leftBar == 0 && rightBar == 0) {
        lcd->setCursor(curX, curY);
        lcd->write(32); // prints empty block to the cursor position
        return;
    }

    for (int8_t i = 7; i >= 0; --i, --leftBar, --rightBar) {
        newchar[i] = 0; // flush the horizontal bar

        if (leftBar > 0) {
            newchar[i] |= 0b11000;
        }

        if (rightBar > 0) {
            newchar[i] |= 0b00011;
        }
    }

    lcd->createChar(reg, (uint8_t *) newchar);

    // after creating the character must set cursor here
    // otherwise would not print the character
    lcd->setCursor(curX, curY);
    lcd->write(reg);
}
