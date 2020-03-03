#include "Controller.h"

Controller::Controller(Fan *fan_, Pressure *pressure_, State state_)
    : fan(fan_), pressure(pressure_), state(state_)
{
    targetSpeed = 0;
    targetPressure = 0;
}
