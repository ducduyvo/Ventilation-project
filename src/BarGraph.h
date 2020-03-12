/*
 * BarGraph.h
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#ifndef BARGRAPH_H_
#define BARGRAPH_H_

#include "LiquidCrystal.h"

class BarGraph {
public:
    BarGraph(LiquidCrystal *lcd_);
    virtual ~BarGraph() { };
    void draw2Bars(int8_t first, int8_t second, uint8_t curX, uint8_t curY);

protected:
    LiquidCrystal *lcd;

private:
    uint8_t newchar[8];
};

#endif /* BARGRAPH_H_ */
