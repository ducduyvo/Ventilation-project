#include "Pressure.h"

Pressure::Pressure() : i2c(i2c_config) { }

int16_t Pressure::getPressure()
{
    uint8_t data[3];
    uint8_t cmd = READ_COMMAND;
    int16_t pressure = 0;
    i2c.transaction(PRESSURE_ADDRESS, &cmd, 1, data, 3);
    /* i2c.write(PRESSURE_ADDRESS, &cmd, 1); */
    /* i2c.read(PRESSURE_ADDRESS, data, 3); */

    pressure = (data[0] << 8) | data[1];
    return convertPressure(pressure);
}

int16_t Pressure::convertPressure(int16_t pressure)
{
    return (pressure / SCALE_FACTOR) * 0.95;
}
