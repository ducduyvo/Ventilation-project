#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "Pressure.h"
#include "Fan.h"

enum State { manual, automatic };

class Controller {
public:
    Controller(Fan *fan_ = nullptr, Pressure *pressure_ = nullptr, State state_ = manual);
    virtual ~Controller();
    void executeState();

private:
    Fan *fan;
    Pressure *pressure;
    uint16_t targetSpeed; // in percent
    uint8_t targetPressure;
    State state;
};

#endif /* CONTROLLER_H_ */
