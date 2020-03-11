#include "Pressure.h"
#include <mutex>

Pressure::Pressure() : i2c(i2c_config) { }

int16_t Pressure::updatePressure()
{
    uint8_t data[3];
    uint8_t cmd = READ_COMMAND;
    int16_t value = 0;
    i2c.transaction(PRESSURE_ADDRESS, &cmd, 1, data, 3);
    /* i2c.write(PRESSURE_ADDRESS, &cmd, 1); */
    /* i2c.read(PRESSURE_ADDRESS, data, 3); */

    value = (data[0] << 8) | data[1];
    pressure = convertPressure(value);
}

int16_t Pressure::convertPressure(int16_t pressure)
{
    return (pressure / SCALE_FACTOR) * 0.95;
}


int16_t Pressure::getPressure() {
    std::lock_guard<Imutex> lock(guard);
    return pressure;
}
