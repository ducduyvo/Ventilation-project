#ifndef PRESSURE_H_
#define PRESSURE_H_

#define SCALE_FACTOR 240

// not the actual maximum pressure but since our target pressure can be at max 120
// this should also be
#define MAX_PRESSURE 120
#define MIN_PRESSURE 0

#include "I2C.h"

class Pressure {
public:
    Pressure();
    virtual ~Pressure() { };
    int16_t getPressure();
    void updatePressure();

private:
    struct I2C_config i2c_config;
    I2C i2c;
    int16_t convertPressure(int16_t pressure_);
    int16_t pressure = 0;
};

#endif /* PRESSURE_H_ */
