#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "Pressure.h"
#include "Fan.h"
#include "IntegerEdit.h"
#include "ModeEdit.h"

class Controller {
public:
    Controller(Fan *fan_, Pressure *pressure_, IntegerEdit *targetSpeed_, IntegerEdit *targetPressure_, ModeEdit *state_);
    virtual ~Controller(){};
    void updatePeripherals();
    int16_t pressureDifference();
    bool isInRange(int offset);
    void setTargetSpeed(uint8_t targetSpeed_) { targetSpeed->setValue(targetSpeed_); }
    void setTargetPressure(uint8_t targetPressure_) { targetPressure->setValue(targetPressure_); }
    uint8_t getTargetSpeed() { return targetSpeed->getValue(); }
    uint8_t getTargetPressure() { return targetPressure->getValue(); }
    int16_t getDifference() {return pressureDifference();}



private:
    int16_t preDifference;
    double _integral;
    Fan *fan;
    Pressure *pressure;
    IntegerEdit *targetSpeed;      // in percent
    IntegerEdit *targetPressure; // in pascal
    ModeEdit *currentMode;
};

#endif /* CONTROLLER_H_ */
