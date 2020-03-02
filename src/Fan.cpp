#include "Fan.h"


Fan::Fan() :
    node(2), speed(&node, 1), controlWord(&node, 2), statusWord(&node, 3) {
    node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

    ModbusRegister ControlWord(&node, 0);
    ModbusRegister StatusWord(&node, 3);
    ModbusRegister OutputFrequency(&node, 102);
    ModbusRegister Current(&node, 103);


    // need to use explicit conversion since printf's variable argument doesn't automatically convert this to an integer
    printf("Status=%04X\n", (int)StatusWord); // for debugging

    ControlWord = 0x0406; // prepare for starting

    printf("Status=%04X\n", (int)StatusWord); // for debugging

    Sleep(1000); // give converter some time to set up
    // note: we should have a startup state machine that check converter status and acts per current status
    //       but we take the easy way out and just wait a while and hope that everything goes well

    printf("Status=%04X\n", (int)StatusWord); // for debugging

    ControlWord = 0x047F; // set drive to start mode

    printf("Status=%04X\n", (int)StatusWord); // for debugging

    Sleep(1000); // give converter some time to set up
    // note: we should have a startup state machine that check converter status and acts per current status
    //       but we take the easy way out and just wait a while and hope that everything goes well

    printf("Status=%04X\n", (int)StatusWord); // for debugging

    setFanSpeed(fanSpeed);
}

Fan::~Fan() { };

void Fan::setFanSpeed(uint16_t speed_) {
    fanSpeed = speed_;
    printf("Set freq = %d\n", fanSpeed); // for debugging

}

void Fan::setSpeed(uint8_t percent)  {
    if (percent > 100)     speed = MAX_SPEED;
    else if (percent < 0)  speed = 0;
    else                   speed = percent * 200;

    setFanSpeed(speed);
}

bool Fan::getStatusBit(uint8_t bit) {
    return (statusWord & (1 << bit));
}

