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

#define REACHTIME 65
#define TICKRATE 1000
#define MAXREPEAT 500
#define MINREPEAT 10
#define DEBOUNCE_TIME 150

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
static volatile int debounce = 0;
static volatile uint32_t systicks = 0;
static volatile int backCounter = 10000;
//static volatile uint32_t systicks;
static LiquidCrystal *lcd;
static Controller *controller;
static Printer printer;
static Menu *menu;
static HomeScreen *homeScreen;

// No switch 2 since it's for the ok button and we dont need to repeat ok
static volatile int intRepeat = 0;
static volatile int previousIntRepeat = 0;
static bool releasedSw0 = true;
static bool releasedSw2 = true;

void switchEvent(MenuItem::menuEvent event)
{
    if (intRepeat <= 0) {
        menu->event(event);

        intRepeat = previousIntRepeat / 1.5;

        if (intRepeat < MINREPEAT) {
            intRepeat = MINREPEAT;
        }

        else if (intRepeat > MAXREPEAT) {
            intRepeat = MAXREPEAT;
        }

        previousIntRepeat = intRepeat;
    }
}

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
    backCounter--;
    if (intRepeat > 0) {
        intRepeat--;
    }
    if (debounce > 0)
        debounce--;
    if (counter > 0)
        counter--;

    if (!releasedSw0) {
        switchEvent(MenuItem::menuEvent::down);
    }

    if (!releasedSw2) {
        switchEvent(MenuItem::menuEvent::up);
    }
    if (backCounter <= 0) {
        backCounter = 10000;
        menu->event(MenuItem::menuEvent::back);
    }

}
#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
    counter = ms;
    while (counter > 0) {
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
        if (debounce <= 0) menu->event(MenuItem::down);
        debounce = DEBOUNCE_TIME;
        backCounter = 10000;
        printf("sw1\n");
    }

    void PIN_INT1_IRQHandler(void)
    {
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
        if (debounce <= 0) menu->event(MenuItem::ok);
        debounce = DEBOUNCE_TIME;
        printf("sw2\n");
        backCounter = 10000;
    }

    void PIN_INT2_IRQHandler(void)
    {
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
        if (debounce <= 0) menu->event(MenuItem::up);
        debounce = DEBOUNCE_TIME;
        printf("sw3\n");
        backCounter = 10000;
    }

    /* void PIN_INT0_IRQHandler(void) */
    /* { */
    /*     // check whether the interrupt was low or high */
    /*     if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) == PININTCH(0)) { */
    /*         printf("sw0 Low\n"); */
    /*         releasedSw0 = true; */
    /*     } */

    /*     else { */
    /*         if (debounce <= 0) { */
    /*             menu->event(MenuItem::menuEvent::down); */
    /*         } */
    /*         printf("sw0 High\n"); */
    /*         releasedSw0 = false; */
    /*         intRepeat = MAXREPEAT; */
    /*         previousIntRepeat = intRepeat; */
    /*         Chip_PININT_ClearFallStates(LPC_GPIO_PIN_INT, PININTCH(0)); */
    /*     } */

    /*     debounce = DEBOUNCE_TIME; */
    /*     backCounter = 10000; */
    /*     Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0)); */
    /* } */

    /* void PIN_INT1_IRQHandler(void) */
    /* { */
    /*     if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) == PININTCH(1)) { */
    /*         printf("sw1\n"); */
    /*         if (debounce <= 0) { */
    /*             menu->event(MenuItem::ok); */
    /*         } */
    /*     } */
    /*     debounce = DEBOUNCE_TIME; */
    /*     backCounter = 10000; */
    /*     Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1)); */
    /* } */

    /* void PIN_INT2_IRQHandler(void) */
    /* { */
    /*     // check whether the interrupt was low or high */
    /*     if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) == PININTCH(2)) { */
    /*         printf("sw2 Low\n"); */
    /*         releasedSw2 = true; */
    /*     } */

    /*     else { */
    /*         if (debounce <= 0) { */
    /*             menu->event(MenuItem::menuEvent::up); */
    /*         } */
    /*         printf("sw2 High\n"); */
    /*         releasedSw2 = false; */
    /*         intRepeat = MAXREPEAT; */
    /*         previousIntRepeat = intRepeat; */
    /*         Chip_PININT_ClearFallStates(LPC_GPIO_PIN_INT, PININTCH(2)); */
    /*     } */

    /*     debounce = DEBOUNCE_TIME; */
    /*     backCounter = 10000; */
    /*     Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2)); */
    /* } */
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
    SysTick_Config(SystemCoreClock / TICKRATE);

    Board_LED_Set(0, false);
    Board_LED_Set(1, true);
    printf("Started\n"); // goes to ITM console if retarget_itm.c is included

    // Switches used to initalize the pins
    DigitalIoPin sw0(1, 3, true, true, true);
    DigitalIoPin sw1(0, 9, true, true, true);
    DigitalIoPin sw2(0, 10, true, true, true);

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

    /* Confiure interrupts */
    // switch 1
    Chip_INMUX_PinIntSel(0, 1, 3);
    Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(0));
    Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(0));
    Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(0));
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
    Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(2));
    NVIC_ClearPendingIRQ(PIN_INT2_IRQn);
    NVIC_EnableIRQ(PIN_INT2_IRQn);

    /* LCD */
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

    /* Variables */
    Fan fan;
    Pressure pressure;
    int reachCounter = 0;

    IntegerEdit targetSpeed(lcd, "Target Speed", 0, 100, 1);
    IntegerEdit targetPressure(lcd, "Target Pressure", 0, 120, 1);
    ModeEdit currentMode(lcd, "Mode", Mode::automatic);


    MenuItem speedItem(&targetSpeed);
    MenuItem pressureItem(&targetPressure);
    homeScreen = new HomeScreen(lcd, &fan, &pressure, &currentMode);
    controller = new Controller(&fan, &pressure, &targetSpeed, &targetPressure, &currentMode);

    menu = new Menu(homeScreen, &speedItem, &pressureItem, &currentMode); /* this could also be allocated from the heap */
    menu->event(MenuItem::show);

    while (1) {
        controller->updatePeripherals();
        printf("targetPressure = %d, targetFanSpeed = %u\n", controller->getTargetPressure(), controller->getTargetSpeed());
        printf("pressure = %d, speed = %u\n", pressure.getPressure(), fan.getSpeed());

        if (controller != nullptr && menu->getPosition() == HOMEPOS) {
            if (controller->hasPressureChanged()) {
                homeScreen->displayPressure();
            }
            if (controller->hasModeChanged()) {
                homeScreen->displayMode();
            }
            if (controller->hasSpeedChanged()) {
                homeScreen->displayFan();
            }
            if (menu->hasPositionChanged()) {
                menu->event(MenuItem::show);
            }
        }

        /* if (controller->pressureDifference() == 0) */
        /*     reachCounter = 0; */

        /* else if (controller->pressureDifference() != 0 && currentMode.getValue() == Mode::automatic) */
        /*     reachCounter++; */

        /* else if (currentMode.getValue() == Mode::manual) */
        /*     reachCounter = 0; */

        /* if (reachCounter == REACHTIME) { */
        /*     printf("Unreachable\n"); */
        /*     lcd->clear(); */
        /*     lcd->setCursor(0, 0); */
        /*     lcd->print("Can't reach"); */
        /*     lcd->setCursor(0, 1); */
        /*     lcd->print("target pressure"); */
        /*     Sleep(3000); */
        /*     reachCounter = 0; */
        /* } */
        /* Sleep(100); */
    }

    return 1;
}
