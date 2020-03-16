#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "Pressure.h"
#include "Fan.h"
#include "IntegerEdit.h"
#include "ModeEdit.h"

//#define USE_PID

#define P 1.05
#define I 0.1
#define D 0.1

#define PRESSURE_RANGE 2

class Controller {
public:
    Controller(Fan *fan_, Pressure *pressure_, IntegerEdit *targetSpeed_, IntegerEdit *targetPressure_, ModeEdit *state_);
    virtual ~Controller() { };
    void updatePeripherals();
    int16_t pressureDifference();
    bool isInRange(int range);
    void setTargetSpeed(uint8_t targetSpeed_)       { targetSpeed->setValue(targetSpeed_); }
    void setTargetPressure(uint8_t targetPressure_) { targetPressure->setValue(targetPressure_); }
    uint8_t getTargetSpeed()                        { return targetSpeed->getValue(); }
    uint8_t getTargetPressure()                     { return targetPressure->getValue(); }
    bool hasSpeedChanged();
    bool hasPressureChanged();
    bool hasModeChanged();
private:
    // Since c11 it is allowed to initialize data members in header file
    int16_t preDifference = 0;
    double integral = 0;
    Fan *fan;
    Pressure *pressure;
    IntegerEdit *targetSpeed;      // in percent
    IntegerEdit *targetPressure;   // in pascal
    ModeEdit *currentMode;

    uint8_t previousSpeed = 0;
    int16_t previousPressure = 0;
    Mode previousMode;
};

#endif /* CONTROLLER_H_ */
