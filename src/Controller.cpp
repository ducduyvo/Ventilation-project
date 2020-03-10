#include "Controller.h"
#include "math.h"

Controller::Controller(Fan *fan_, Pressure *pressure_, IntegerEdit *targetSpeed_, IntegerEdit *targetPressure_, ModeEdit *state_)
    : fan(fan_), pressure(pressure_), targetSpeed(targetSpeed_), targetPressure(targetPressure_), currentMode(state_)
{
}

void Controller::updatePeripherals()
{
    switch (currentMode->getValue())
    {

    case Mode::manual:
        fan->setSpeed(targetSpeed->getValue());
        break;

    case Mode::automatic:
        //int16_t offset = targetPressure->getValue() - pressure->getPressure();
        int16_t difference = pressureDifference();
        if (difference < 0)
            difference = -sqrt(abs(difference));
        else
            difference = sqrt(abs(difference));
        if (!isInRange(2))
        {
            fan->setSpeed(fan->getSpeed() + difference);
        }
        break;
    }
}

int16_t Controller::pressureDifference()
{
    return targetPressure->getValue() - pressure->getPressure();
}

/*
bool Controller::isInRange(int range){
	if (pressureDifference() > (pressure->getPressure() - range) &&
		pressureDifference() < (pressure->getPressure() + range))
          return true;
	else return false;
}
*/
bool Controller::isInRange(int range)
{
    if (pressureDifference() > -range &&
        pressureDifference() < range)
        return true;
    else
        return false;
}
