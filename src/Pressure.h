#ifndef PRESSURE_H_
#define PRESSURE_H_

#define SCALE_FACTOR 240

#include "I2C.h"
#include "Imutex.h"

class Pressure {
public:
    Pressure();
    virtual ~Pressure() { };
    int16_t getPressure();
    int16_t updatePressure();

private:
    Imutex guard;
    struct I2C_config i2c_config;
    I2C i2c;
    int16_t convertPressure(int16_t pressure);
    int16_t pressure = 0;
    // TODO: altitude private member
};

#endif /* PRESSURE_H_ */
