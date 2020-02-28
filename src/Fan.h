#ifndef FAN_H_
#define FAN_H_

// values for speed member
#define MAX_SPEED 20000
#define MIN_SPEED 0

#include "ModbusMaster.h"

void Sleep(int ms);


#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

class Fan {
public:
    Fan(uint8_t percent = 0);
    virtual ~Fan();

    void setSpeed(uint8_t percent) ; // set the fan speed in percent (0-100)
    uint16_t getSpeed() { return speed; }; // get the fan speed (0-20000)
    void setFrequency(); // set the motor frequency according to speed variable

private:
    ModbusMaster node;
    uint16_t speed; // motor speed
};

#endif /* FAN_H_ */
