#include "Controller.h"
#include "math.h"

Controller::Controller(Fan &fan_, Pressure &pressure_, IntegerEdit &fanEdit_, IntegerEdit &pressureEdit_, ModeEdit &state_)
    : fan(fan_), pressure(pressure_), pressureEdit(pressureEdit_), fanEdit(fanEdit_), currentMode(state_)
{

    targetSpeed = 0;
    targetPressure = 0;
}

void Controller::updatePeripherals() {
    switch (currentMode.getValue()) {

    case controllerMode::manual:
        fan.setSpeed(targetSpeed);
        break;

    case controllerMode::automatic:
        int16_t offset = targetPressure - pressure.getPressure();
        if (offset < 0) offset = -sqrt(abs(offset));
        else            offset =  sqrt(abs(offset));
        fan.setSpeed(fan.getSpeed() + offset);
        break;
    }
}

