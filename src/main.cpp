/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#include <cstring>
#include <cstdio>

#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include "LpcUart.h"
#include "Printer.h"
#include "Fan.h"
#include "I2C.h"
#include "crc16.h"
#include "Pressure.h"
#include "Controller.h"
#include "math.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/


/*****************************************************************************
 * Public functions
 ****************************************************************************/
static volatile int counter;
static volatile uint32_t systicks;

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
    systicks++;
    if(counter > 0) counter--;
}
#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
    counter = ms;
    while(counter > 0) {
        __WFI();
    }
}

/* this function is required by the modbus library */
uint32_t millis() {
    return systicks;
}


/**
 * @brief	Main UART program body
 * @return	Always returns 1
 */
int main(void)
{

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif
    LpcPinMap none = {-1, -1}; // unused pin has negative values in it
    LpcPinMap txpin = { 0, 18 }; // transmit pin that goes to debugger's UART->USB converter
    LpcPinMap rxpin = { 0, 13 }; // receive pin that goes to debugger's UART->USB converter
    LpcUartConfig cfg = { LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none };
    LpcUart dbgu(cfg);

    /* Set up SWO to PIO1_2 */
    Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

    /* Enable and setup SysTick Timer at a periodic rate */
    SysTick_Config(SystemCoreClock / 1000);

    Board_LED_Set(0, false);
    Board_LED_Set(1, true);
    printf("Started\n"); // goes to ITM console if retarget_itm.c is included

    Fan fan;
    int j = 0;
    Pressure pressure;
    controllerMode currentState = manual;
    int targetPressure = 50;
    while (1) {
        if (currentState == controllerMode::manual) {
            fan.setSpeed(0);
        }

        else if (currentState == controllerMode::automatic) {
            int offset = targetPressure - pressure.getPressure();
            if (offset < 0) offset = -sqrt(abs(offset));
            else offset = sqrt(abs(offset));
            fan.setSpeed(fan.getSpeed() + offset);
        }

        else printf("Some sort of error happened\n");

        printf("Pressure = %d, FanSpeed = %u\n", pressure.getPressure(), fan.getSpeed());
        Sleep(100);
    }

    /* while(1) { */
    /*     printf("Pressure = %d, FanSpeed = %u\n", pressure.getPressure(), fan.getSpeed()); */
    /*     fan.setSpeed(j++); */
    /*     if (j > 100) j = 0; */
    /*     Sleep(100); */
    /* } */

    return 1;
}

