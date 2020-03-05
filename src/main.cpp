/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined(__USE_LPCOPEN)
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
#include "SimpleMenu.h"
#include "IntegerEdit.h"

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
static SimpleMenu menu; /* this could also be allocated from the heap */
static LiquidCrystal *lcd;
static Controller *controller;

IntegerEdit targetSpeed(lcd, "Speed", 0, 100, 10);
IntegerEdit targetPressure(lcd, "Pressure", 0, 120, 10);
ModeEdit modeEdit(lcd, "Mode", ModeEdit::Mode::automatic);

#ifdef __cplusplus
extern "C"
{
#endif
    /**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
    void SysTick_Handler(void)
    {
        systicks++;
        if (counter > 0)
            counter--;
    }
#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
    counter = ms;
    while (counter > 0)
    {
        __WFI();
    }
}

/* this function is required by the modbus library */
uint32_t millis()
{
    return systicks;
}

extern "C"
{
    void PIN_INT0_IRQHandler(void)
    {
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
        menu.event(MenuItem::up);
        //modeEdit->increment();
        printf("sw1\n");
    }

    void PIN_INT1_IRQHandler(void)
    {
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
        menu.event(MenuItem::ok);
        printf("sw2\n");
    }

    void PIN_INT2_IRQHandler(void)
    {
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
        printf("sw3\n");
        menu.event(MenuItem::down);
        //modeEdit->decrement();
    }
}

/**
 * @brief	Main UART program body
 * @return	Always returns 1
 */
int main(void)
{

#if defined(__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
    Chip_RIT_Init(LPC_RITIMER);
#endif
#endif
    LpcPinMap none = {-1, -1}; // unused pin has negative values in it
    LpcPinMap txpin = {0, 18}; // transmit pin that goes to debugger's UART->USB converter
    LpcPinMap rxpin = {0, 13}; // receive pin that goes to debugger's UART->USB converter
    LpcUartConfig cfg = {LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none};
    LpcUart dbgu(cfg);

    /* Set up SWO to PIO1_2 */
    Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

    /* Enable and setup SysTick Timer at a periodic rate */
    Chip_Clock_SetSysTickClockDiv(1);
    SysTick_Config(SystemCoreClock / 1000);

    Board_LED_Set(0, false);
    Board_LED_Set(1, true);
    printf("Started\n"); // goes to ITM console if retarget_itm.c is included

    // Switches used to initalize the pins
    DigitalIoPin sw1(1, 3, true, true, true);
    DigitalIoPin sw2(0, 9, true, true, true);
    DigitalIoPin sw3(0, 10, true, true, true);

    // Configure channel interrupt as edge sensitive and falling edge interrupt
    /* Initialize PININT driver */
    Chip_PININT_Init(LPC_GPIO_PIN_INT);
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);
    Chip_SYSCTL_PeriphReset(RESET_PININT);

    // DONT USE THESE DIGITAL PINS
    // {-1, -1};
    // { 1, 9 };
    // { 1, 10 };
    // { 0, 29 };

    // Confiure interrupts
    // switch 1
    Chip_INMUX_PinIntSel(0, 1, 3);
    Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(0));
    Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(0));
    NVIC_ClearPendingIRQ(PIN_INT0_IRQn);
    NVIC_EnableIRQ(PIN_INT0_IRQn);

    // switch 2
    Chip_INMUX_PinIntSel(1, 0, 9);
    Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(1));
    Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(1));
    NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
    NVIC_EnableIRQ(PIN_INT1_IRQn);

    // switch 3
    Chip_INMUX_PinIntSel(2, 0, 10);
    Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(2));
    Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(2));
    NVIC_ClearPendingIRQ(PIN_INT2_IRQn);
    NVIC_EnableIRQ(PIN_INT2_IRQn);

    // LCD
    DigitalIoPin rs(0, 8, false, true, false);
    DigitalIoPin en(1, 6, false, true, false);
    DigitalIoPin d4(1, 8, false, true, false);
    DigitalIoPin d5(0, 5, false, true, false);
    DigitalIoPin d6(0, 6, false, true, false);
    DigitalIoPin d7(0, 7, false, true, false);
    lcd = new LiquidCrystal(&rs, &en, &d4, &d5, &d6, &d7);
    lcd->begin(16, 2);
    lcd->setCursor(0, 0);
    lcd->print("hello");

    Fan fan;
    Pressure pressure;

    //int targetPressure = 50;

    controller = new Controller(fan, pressure, targetSpeed, targetPressure, modeEdit);

    menu.addItem(new MenuItem(&modeEdit));
    menu.addItem(new MenuItem(&targetSpeed));
    menu.addItem(new MenuItem(&targetPressure));

    menu.event(MenuItem::show); // display first menu item

    // while (1) {
    //     if (currentState == ModeEdit::Mode::manual) {
    //         fan.setSpeed(50);
    //     }

    //     else if (currentState == ModeEdit::Mode::automatic) {
    //         int offset = targetPressure - pressure.getPressure();
    //         if (offset < 0) offset = -sqrt(abs(offset));
    //         else offset = sqrt(abs(offset));
    //         fan.setSpeed(fan.getSpeed() + offset);
    //     }

    //     else printf("Some sort of error happened\n");

    //     printf("Pressure = %d, FanSpeed = %u\n", pressure.getPressure(), fan.getSpeed());
    //     Sleep(100);
    // }
    while (1)
    {
        controller->updatePeripherals();
        printf("Pressure = %d, FanSpeed = %u\n", controller->getTargetPressure(), controller->getTargetSpeed());

        Sleep(100);
    }

    return 1;
}
