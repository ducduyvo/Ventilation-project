#include "Fan.h"

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

#ifdef USE_STATE_MACHINE

    int stateNumber = 1;
    uint16_t controlWordList[5];


	while (true) { //indicate state number
		switch (stateNumber) {
		case 1: //NOT READY TO SWITCH ON
			writeSingleRegister(0, controlWordList[0]); //send signal to next state
			printf("Sent control word 0x%X to drive \n", controlWordList[0]);
			stateNumber++;
			break;
		case 2: //READY TO SWITCH ON
			writeSingleRegister(0, controlWordList[1]);
			printf("Sent control word 0x%X to drive \n", controlWordList[1]);
			stateNumber++;
			break;
		case 3: //READY TO OPERATE
			writeSingleRegister(0, controlWordList[2]);
			printf("Sent control word 0x%X to drive \n", controlWordList[2]);
			stateNumber++;
			break;
		case 4: //OPERATION ENABLED, this has problem with simulator
			writeSingleRegister(0, controlWordList[3]);
			printf("Sent control word 0x%X to drive \n", controlWordList[3]);
			stateNumber++;
			break;
		case 5: //RFG: ACCELERATOR ENABLED , this state is a bit special
			writeSingleRegister(0, controlWordList[4]);
			printf("Sent control word 0x%X to drive \n", controlWordList[4]);
			stateNumber++;
			break;
		}
    }
#endif

    Sleep(1000); // give converter some time to set up
    // note: we should have a startup state machine that check converter status and acts per current status
    //       but we take the easy way out and just wait a while and hope that everything goes well

    printf("Status=%04X\n", (int)statusWord); // for debugging
    setSpeed(0);

}

void Fan::setSpeed(int8_t percent)
{
    if (percent > 100)     frequency = MAX_FREQUENCY;
    else if (percent < 0)  frequency = MIN_FREQUENCY;
    else                   frequency = percent * 200;
	int ctr = 0;
	bool atSetpoint = false;
	int result = 0;
	do {
		Sleep(50);
		// read status word
		result = statusWord;
		// check if we are at setpoint
		if (result >= 0 && (result & 0x0100)) atSetpoint = true;
		ctr++;
	} while(ctr < 20 && !atSetpoint);
}

// update speed of fan in percent based on frequency of kit
void Fan::updateSpeed()
{
    speed = frequency / 200;
}

// Get specific bit from statusword, statusregister enum is meant to be used with this function
bool Fan::getStatusBit(uint8_t bit)
{
    return (statusWord & (1 << bit));
}

// returns uint8_t speed variables value, NOT the actual current reading from the fan sensor
uint8_t Fan::getSpeed()
{
    return speed;
}

//write a value to a register
uint8_t Fan::writeSingleRegister(uint16_t u16WriteAddress, uint16_t u16WriteValue) {
	return node.writeSingleRegister(u16WriteAddress, u16WriteValue);
}

