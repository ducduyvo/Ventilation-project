#include "Pressure.h"
#include <mutex>

Pressure::Pressure() : i2c(i2c_config) {}

void Pressure::updatePressure()
{
    uint8_t data[3];
    uint8_t cmd = READ_COMMAND;
    int16_t value = 0;
    if (i2c.transaction(PRESSURE_ADDRESS, &cmd, 1, data, 3))
    {
        //  Output pressure
        // <<: binary left shift, return data is 2 bytes, the last byte is CRC-8.
        value = (data[0] << 8) | data[1];
        pressure = convertPressure(value);
    }
    else
    {
        printf("Error in getting pressure");
    }
}

int16_t Pressure::convertPressure(int16_t pressure_)
{
    return (pressure_ / SCALE_FACTOR) * 0.95;
}

int16_t Pressure::getPressure()
{
    // std::lock_guard<Imutex> lock(guard);
    return pressure;
}
