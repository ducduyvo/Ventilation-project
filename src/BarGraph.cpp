/*
 * BarGraph.cpp
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#include "BarGraph.h"
#include "Printer.h"
static Printer printer;

BarGraph::BarGraph(LiquidCrystal *lcd_): lcd(lcd_)
{
    lcd->setCursor(0,0); // move cursor back to visible area
}

void BarGraph::draw2Bars(int8_t first, int8_t second, uint8_t curX, uint8_t curY, uint8_t reg) {
    if (first < 0) first = 0;
    if (second < 0) second = 0;
    if (first == 0 && second == 0) {
        lcd->setCursor(curX, curY);
        lcd->write(32);
        return;
    }

    for (int8_t i = 7; i >= 0; --i) {
        newchar[i] = 0; // flush the vertical bar
        if (first > 0) {
            newchar[i] |= 0b11000;
        }

        if (second > 0) {
            newchar[i] |= 0b00011;
        }

        newchar[i] &= 0b11011; // Set the middle bar 0
        --first;
        --second;

        printer.print("value %d = %u\n", newchar[i]);
    }


    lcd->createChar(reg, (uint8_t *) newchar);
    lcd->setCursor(curX, curY);
    lcd->write(reg);
}
