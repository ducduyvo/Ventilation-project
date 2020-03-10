#include "Controller.h"
#include "math.h"

Controller::Controller(Fan *fan_, Pressure *pressure_, IntegerEdit *targetSpeed_, IntegerEdit *targetPressure_, ModeEdit *state_)
    : fan(fan_), pressure(pressure_), targetSpeed(targetSpeed_), targetPressure(targetPressure_), currentMode(state_)
{
}

void Controller::updatePeripherals()
{
    switch (currentMode->getValue()) {

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
            if (!isInRange(1)) {
                fan->setSpeed(fan->getSpeed() + difference);
            }

            // // Calculate difference
            // int16_t difference = pressureDifference();
            // // Proportional term
            // double Pout = 0.1 * difference;

            // // Integral term
            // _integral += difference * 0.1;
            // double Iout = 0.5 * _integral;

            // // Derivative term
            // double derivative = (difference - preDifference) / 0.2;
            // double Dout = 0.01 * derivative;

            // // Calculate total output
            // double output = Pout + Iout + Dout;

            // preDifference = difference;

            // fan->setSpeed(fan->getSpeed() + output);

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
    if (pressureDifference() >= -range &&
            pressureDifference() <= range)
        return true;
    else
        return false;
}
