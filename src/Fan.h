#ifndef FAN_H_
#define FAN_H_

#include "ModbusMaster.h"
#include "ModbusRegister.h"

#if defined (__USE_LPCOPEN)
    #if defined(NO_BOARD_LIB)
        #include "chip.h"
    #else
        #include "board.h"
    #endif
#endif

void Sleep(int ms);

// values used for setting the fan speed
#define MAX_FREQUENCY 20000
#define MIN_FREQUENCY 0

// values that will be used by the interface
#define MAX_SPEED 100
#define MIN_SPEED 0


//#define USE_STATE_MACHINE

void Sleep(int ms);

enum StatusWordRegister { RDY_ON, RDY_RUN, RDY_REF, TRIPPED, OFF_2_STA, OFF_3_STA, SWC_ON_INHIB, ALARM, AT_SETPOINT, REMOTE, ABOVE_LIMIT, EXT_CTRL_LOC, EXT_RUN_ENABLE };

class Fan {
public:
    Fan();
    virtual ~Fan() { };

    void setSpeed(int8_t percent); // set the fan speed in percent (0-100)
    uint8_t getSpeed(); // get the fan speed in percent (0-100)
    void updateSpeed();

    uint16_t getStatusWord() { return statusWord; }
    bool getStatusBit(uint8_t bit);
    uint8_t writeSingleRegister(uint16_t u16WriteAddress, uint16_t u16WriteValue);


private:
    ModbusMaster node;
    ModbusRegister frequency;   // Used to control the motor speed (Reference 1)
    ModbusRegister controlWord; // Controlword register
    ModbusRegister statusWord;  // Statusword register
    uint8_t speed = 0;          // stores the fan speed value and should be updated at desired rate
};

#endif /* FAN_H_ */

