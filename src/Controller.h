#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "Pressure.h"
#include "Fan.h"
#include "IntegerEdit.h"
#include "ModeEdit.h"

class Controller {
public:
    Controller(Fan *fan_, Pressure *pressure_, ModeEdit *state_);
    virtual ~Controller();
    void updatePeripherals();
    void setTargetSpeed(uint8_t targetSpeed_) { targetSpeed = targetSpeed_; }
    void setTargetPressure(uint8_t targetPressure_) { targetPressure = targetPressure_; }

private:
    Fan *fan;
    Pressure *pressure;
    IntegerEdit *fanEdit;
    IntegerEdit *pressureEdit;
    ModeEdit *currentMode;
    uint16_t targetSpeed;   // in percent
    uint8_t targetPressure; // in pascal
};

#endif /* CONTROLLER_H_ */
