#include "Controller.h"
#include "math.h"


Controller::Controller(Fan *fan_, Pressure *pressure_, IntegerEdit *targetSpeed_, IntegerEdit *targetPressure_, ModeEdit *state_)
    : fan(fan_), pressure(pressure_), targetSpeed(targetSpeed_), targetPressure(targetPressure_), currentMode(state_)
{
    previousMode = currentMode->getValue();
}

void Controller::updatePeripherals()
{
    pressure->updatePressure();
    fan->updateSpeed();

    switch (currentMode->getValue()) {

        case Mode::manual:
            if (fan->getSpeed() != targetSpeed->getValue()) {
                fan->setSpeed(targetSpeed->getValue());
            }
            break;

        case Mode::automatic:
            if (!isInRange(PRESSURE_RANGE)) {
                // Calculate difference
                int16_t difference = pressureDifference();
#ifdef USE_PID
                // Proportional term
                double Pout = P * difference;

                // Integral term
                integral += difference * 0.1;
                double Iout = I * integral;

                // Derivative term
                double derivative = (difference - preDifference);
                double Dout = D * derivative;

                // Calculate total output
                double output = Pout + Iout + Dout;
                printf("%.2lf + %.2lf + %.2lf = %.2lf,  ", Pout, Iout, Dout, output);

                preDifference = difference;
                printf("GetSpeed = %u\n",fan->getSpeed());
                fan->setSpeed(fan->getSpeed() + output);

#else
                if (difference < 0)
                    difference = -sqrt(abs(difference));
                else
                    difference = sqrt(abs(difference));

                fan->setSpeed((int)fan->getSpeed() + difference);
#endif
            }

            else {
                integral = 0;
            }

            break;
    }
}

int16_t Controller::pressureDifference()
{
    return targetPressure->getValue() - pressure->getPressure();
}

bool Controller::hasSpeedChanged()
{
    bool changed = fan->getSpeed() != previousSpeed;
    previousSpeed = fan->getSpeed();
    return changed;
}

bool Controller::hasPressureChanged()
{
    bool changed = pressure->getPressure() != previousPressure;
    previousPressure = pressure->getPressure();
    return changed;
}

bool Controller::hasModeChanged()
{
    bool changed = currentMode->getValue() != previousMode;
    previousMode = currentMode->getValue();
    return changed;
}

bool Controller::isInRange(int range)
{
    return (abs(pressureDifference()) <= range);
}
