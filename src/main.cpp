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
#include "Menu.h"
#include "IntegerEdit.h"
#include "HomeScreen.h"

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
static LiquidCrystal *lcd;
static Controller *controller;
static Printer printer;
static Menu *menu;


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
        menu->event(MenuItem::up);
        printer.print("sw1\n");
    }

    void PIN_INT1_IRQHandler(void)
    {
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
        menu->event(MenuItem::ok);
        printer.print("sw2\n");
    }

    void PIN_INT2_IRQHandler(void)
    {
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
        printer.print("sw3\n");
        menu->event(MenuItem::down);
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


    /* Enable and setup SysTick Timer at a periodic rate */
    Chip_Clock_SetSysTickClockDiv(1);
    SysTick_Config(SystemCoreClock / 1000);

    Board_LED_Set(0, false);
    Board_LED_Set(1, true);
    printer.print("Started\n"); // goes to ITM console if retarget_itm.c is included

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
    Chip_INMUX_PinIntSel(0, 0, 17);
    Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(0));
    Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(0));
    NVIC_ClearPendingIRQ(PIN_INT0_IRQn);
    NVIC_EnableIRQ(PIN_INT0_IRQn);

    // switch 2
    Chip_INMUX_PinIntSel(1, 1, 11);
    Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(1));
    Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(1));
    NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
    NVIC_EnableIRQ(PIN_INT1_IRQn);

    // switch 3
    Chip_INMUX_PinIntSel(2, 1, 9);
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

    IntegerEdit targetSpeed(lcd, "Speed", 0, 100, 10);
    IntegerEdit targetPressure(lcd, "Pressure", 0, 120, 10);
    ModeEdit currentMode(lcd, "Mode", Mode::automatic);
    MenuItem speedItem(&targetSpeed);
    MenuItem pressureItem(&targetPressure);
    HomeScreen homeScreen(lcd, &targetSpeed, &targetPressure, &currentMode);

    menu = new Menu(&homeScreen, &speedItem, &pressureItem, &currentMode); /* this could also be allocated from the heap */
    /* Fan fan; */
    /* Pressure pressure; */

    //int targetPressure = 50;

    /* controller = new Controller(fan, pressure, targetSpeed, targetPressure, currentMode); */

    lcd->print("hello");
    printer.print("test1\n");
    menu->event(MenuItem::show); // display first menu item
    printer.print("test2\n");

    // Switches used to initalize the pins
    DigitalIoPin sw0(0, 17, true, true, true);
    DigitalIoPin sw1(1, 11, true, true, true);
    DigitalIoPin sw2(1, 9, true, true, true);

    while (1)
    {
        printer.print("mode = %s, targetSpeed = %u, targetPressure = %d\n",
                      currentMode.toString(currentMode.getValue()), targetSpeed.getValue(), targetPressure.getValue());

        Sleep(1000);
    }

    return 1;
}
