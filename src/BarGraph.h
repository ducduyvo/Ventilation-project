#ifndef BARGRAPH_H_
#define BARGRAPH_H_

#include "LiquidCrystal.h"

#define BAR_HEIGHT 8

class BarGraph {
public:
    BarGraph(LiquidCrystal *lcd_);
    virtual ~BarGraph() { };
    void draw2Bars(int8_t first, int8_t second, uint8_t curX, uint8_t curY, uint8_t reg);

protected:
    LiquidCrystal *lcd;

private:
    uint8_t newchar[8];
};

#endif /* BARGRAPH_H_ */
