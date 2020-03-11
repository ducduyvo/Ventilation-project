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

// values for speed member
#define MAX_SPEED 20000
#define MIN_SPEED 0

void Sleep(int ms);

enum StatusRegister { RDY_ON, RDY_RUN, RDY_REF, TRIPPED, OFF_2_STA, OFF_3_STA, SWC_ON_INHIB, ALARM, AT_SETPOINT, REMOTE, ABOVE_LIMIT, EXT_CTRL_LOC, EXT_RUN_ENABLE };

class Fan {
public:
    Fan();
    virtual ~Fan() { };

    void setSpeed(uint8_t percent);            // set the fan speed in percent (0-100)
    uint8_t getSpeed() { return 100; } // get the fan speed in percent (0-100)
    /* uint8_t getSpeed() { return speed / 200; } // get the fan speed in percent (0-100) */

    uint16_t getStatusWord() { return statusWord; }
    bool getStatusBit(uint8_t bit);


private:
    ModbusMaster node;
    ModbusRegister speed;              // Used to control the motor speed (Reference 1)
    ModbusRegister controlWord;        // Control register
    ModbusRegister statusWord;         // Status word register
    uint16_t fanSpeed = 0;             // motor speed
    void setFanSpeed(uint16_t speed_); // set the motor speed according to speed variable
};

#endif /* FAN_H_ */

