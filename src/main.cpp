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

#define REACH_TIME 10000
#define TICKRATE 1000
#define MAXREPEAT 500
#define MINREPEAT 10
#define DEBOUNCE_TIME 50
#define BACK_TIME 10000
#define WARNING_TIME

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static bool loaded = false;
static volatile int counter;
static int reachCounter = 0;
static volatile int debounce = 0;
static volatile uint32_t systicks = 0;
static volatile int backCounter = BACK_TIME;
//static volatile uint32_t systicks;
static LiquidCrystal *lcd;
static Controller *controller;
static Printer printer;
static Menu *menu;
static HomeScreen *homeScreen;
static ModeEdit *currentMode;

// No switch 2 since it's for the ok button and we dont need to repeat ok
static volatile int intRepeat = 0;
static volatile int previousIntRepeat = 0;
static bool releasedSw0 = true;
static bool releasedSw1 = true;
static bool releasedSw2 = true;
/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void checkButtons();
void switchEvent(MenuItem::menuEvent event);
void updateScreen();

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
    if (backCounter > 0)
        backCounter--;
    if (intRepeat > 0)
        intRepeat--;
    if (debounce > 0)
        debounce--;
    if (counter > 0)
        counter--;
    if(backCounter <= 0){
                backCounter = 10000;
                menu->event(MenuItem::menuEvent::back);
            }


    if (loaded) {
        if (controller->isInRange(PRESSURE_RANGE))
            reachCounter = 0;

        else if (controller->pressureDifference() != 0 && currentMode->getValue() == Mode::automatic)
            reachCounter++;
        if (reachCounter < REACH_TIME)
            updateScreen();
    }
}
#ifdef __cplusplus
}
#endif

void updateScreen()
{
    {
        /* checkButtons(); */
        if (menu->getPosition() == HOMEPOS) {
            if (controller->hasModeChanged()) {
                homeScreen->displayMode();
            }
            if (controller->hasPressureChanged()) {
                homeScreen->displayPressure();
            }
            if (controller->hasSpeedChanged()) {
                homeScreen->displayFan();
            }
        }
    }
}

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
        // check whether the interrupt was low or high
        if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) == PININTCH(0)) {
            /* printf("sw0 Low\n"); */
            releasedSw0 = true;
        }

        else {
            if (debounce <= 0)
                menu->event(MenuItem::menuEvent::down);
            /* printf("sw0 High\n"); */
            releasedSw0 = false;
            intRepeat = MAXREPEAT;
            previousIntRepeat = MAXREPEAT;
            Chip_PININT_ClearFallStates(LPC_GPIO_PIN_INT, PININTCH(0));
        }

        backCounter = BACK_TIME;
        debounce = DEBOUNCE_TIME;
        reachCounter = 0;
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
    }

    void PIN_INT1_IRQHandler(void)
    {
        if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) == PININTCH(1)) {
            /* printf("sw1\n"); */
            if (debounce <= 0)
                switchEvent(MenuItem::menuEvent::ok);
        }
        backCounter = BACK_TIME;
        debounce = DEBOUNCE_TIME;
        reachCounter = 0;
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
    }

    void PIN_INT2_IRQHandler(void)
    {
        // check whether the interrupt was low or high
        if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) == PININTCH(2)) {
            /* printf("sw2 Low\n"); */
            releasedSw2 = true;
        }

        else {
            if (debounce <= 0)
                menu->event(MenuItem::menuEvent::up);
            /* printf("sw2 High\n"); */
            releasedSw2 = false;
            Chip_PININT_ClearFallStates(LPC_GPIO_PIN_INT, PININTCH(2));
            intRepeat = MAXREPEAT;
            previousIntRepeat = MAXREPEAT;
        }

        backCounter = BACK_TIME;
        reachCounter = 0;
        debounce = DEBOUNCE_TIME;
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
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
    Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(1));
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

    // TODO: what should the step be
    IntegerEdit targetSpeed(lcd, "Target Speed", 0, 100, 7);
    IntegerEdit targetPressure(lcd, "Target Pressure", 0, 120, 7);
    targetSpeed.setSymbol("%%");
    targetPressure.setSymbol("pa");
    currentMode = new ModeEdit(lcd, "Mode", Mode::automatic);

    MenuItem speedItem(&targetSpeed);
    MenuItem pressureItem(&targetPressure);
    homeScreen = new HomeScreen(lcd, &fan, &pressure, currentMode);
    controller = new Controller(&fan, &pressure, &targetSpeed, &targetPressure, currentMode);

    menu = new Menu(homeScreen, &speedItem, &pressureItem, currentMode); /* this could also be allocated from the heap */
    menu->event(MenuItem::show);

    loaded = true;
    while (1) {
        printf("%d/%d, %u/%u\n", pressure.getPressure(), controller->getTargetPressure(), fan.getSpeed(), controller->getTargetSpeed());
        controller->updatePeripherals();

        if (reachCounter >= REACH_TIME) {
            printf("Unreachable\n");
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("Can't reach");
            lcd->setCursor(0, 1);
            lcd->print("target pressure");
            Sleep(5000);
            menu->event(MenuItem::back);
            reachCounter = 0;
        }
        Sleep(500);
    }

    return 1;
}

void checkButtons()
{
    if (!releasedSw0) {
        switchEvent(MenuItem::menuEvent::down);
    }

    if (!releasedSw2) {
        switchEvent(MenuItem::menuEvent::up);
    }

    if (backCounter <= 0) {
        backCounter = BACK_TIME;
        menu->event(MenuItem::menuEvent::back);
    }
}

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
