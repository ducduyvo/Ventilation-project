#include "Fan.h"
#include <mutex>

Fan::Fan() :
    node(2), frequency(&node, 1), controlWord(&node, 0), statusWord(&node, 3)
{
    node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

    // need to use explicit conversion since printf's variable argument doesn't automatically convert this to an integer
    printf("Status=%04X\n", (int)statusWord); // for debugging

    controlWord = 0x0406; // prepare for starting

    printf("Status=%04X\n", (int)statusWord); // for debugging

    Sleep(1000); // give converter some time to set up
    // note: we should have a startup state machine that check converter status and acts per current status
    //       but we take the easy way out and just wait a while and hope that everything goes well

    printf("Status=%04X\n", (int)statusWord); // for debugging

    controlWord = 0x047F; // set drive to start mode

    printf("Status=%04X\n", (int)statusWord); // for debugging

    // TODO: Like in keijo's comment make state machine that check's whether
    // status is ready, so we are ready to go
    Sleep(1000); // give converter some time to set up
    // note: we should have a startup state machine that check converter status and acts per current status
    //       but we take the easy way out and just wait a while and hope that everything goes well

    printf("Status=%04X\n", (int)statusWord); // for debugging
    setSpeed(0);

}

void Fan::setSpeed(int8_t percent)
{
    if (percent > 100)     frequency = MAX_SPEED;
    else if (percent < 0)  frequency = MIN_SPEED;
    else                   frequency = percent * 200;
    updateSpeed();
}

void Fan::updateSpeed()
{
    speed = frequency / 200;
}

bool Fan::getStatusBit(uint8_t bit)
{
    return (statusWord & (1 << bit));
}

uint8_t Fan::getSpeed()
{
    std::lock_guard<Imutex> lock(guard);
    return speed;
}
