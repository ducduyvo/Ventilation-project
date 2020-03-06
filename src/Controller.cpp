#include "Controller.h"
#include "math.h"

Controller::Controller(Fan *fan_, Pressure *pressure_, IntegerEdit *targetSpeed_, IntegerEdit *targetPressure_, ModeEdit *state_)
    : fan(fan_), pressure(pressure_), targetSpeed(targetSpeed_), targetPressure(targetPressure_), currentMode(state_)
{ }

void Controller::updatePeripherals() {
    switch (currentMode->getValue()) {

        case Mode::manual:
        fan->setSpeed(targetSpeed->getValue());
        break;

        case Mode::automatic:
        int16_t offset = targetPressure->getValue() - pressure->getPressure();
        if (offset < 0) offset = -sqrt(abs(offset));
        else            offset =  sqrt(abs(offset));
        fan->setSpeed(fan->getSpeed() + offset);
        break;
    }
}
