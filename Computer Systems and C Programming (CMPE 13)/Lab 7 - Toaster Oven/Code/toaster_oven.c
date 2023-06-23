// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"


// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"
#include "Ascii.h"
#include "Adc.h"
#include "Buttons.h"
#include "Oled.h"
#include "OledDriver.h"

// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)

#define LONG_PRESS 5

#define BAKEorBROILOnFormat "|%c%c%c%c|\t Mode: %s\n" \
                                           "|    |\t Time: %d:%02d\n" \
                                           "|----|\t Temp: %d%cF\n" \
                                           "|%c%c%c%c|"

#define BAKEorBROILOffFormat "|%c%c%c%c|\t Mode: %s\n" \
                                               "|    |\t%sTime: %d:%02d\n" \
                                               "|----|\t%sTemp: %d%cF\n" \
                                               "|%c%c%c%c|"

#define TOASTFormat "|%c%c%c%c|\t Mode: Toast\n" \
                                 "|    |\t Time: %d:%02d\n" \
                                 "|----|\n" \
                                 "|%c%c%c%c|"

// **** Declare any datatypes here ****

typedef enum {
    RESET, START, COUNTDOWN, PENDING_SELECTOR_CHANGE, PENDING_RESET
} CookState;

typedef enum {
    BAKE, TOAST, BROIL
} CookMode;

typedef enum {
    TIME, TEMP
} CookSelection;

typedef struct {
    uint16_t timeLeft;
    uint16_t startTime;
    uint16_t temp;
    CookMode mode;
    CookState state;
    uint8_t buttonPressCounter;
    CookSelection input;
} CookData;

typedef struct {
    uint8_t event;
    uint8_t value;
} TimerEvent;

// **** Define any module-level, global, or external variables here ****
// String for OLED
static char output[255];

// Counter for 5Hz interrupt
static uint8_t timer5hzCounter;

// Event flags for interrupts
static uint8_t timer5hzEvent;
static uint8_t timer2hzEvent;

// Storing LED values
static uint8_t LEDS;

// Struct variables
static CookData oven;
static TimerEvent data;

// Configuration Bit settings

// Function prototypes
void DrawString(CookData *oven);
void UpdateAdcData(CookData *oven);
void LightLEDS(CookData *oven);

int main()
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR1 to F_PB / 256 / 2 yields a 0.5s timer.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, BOARD_GetPBClock() / 256 / 2);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, BOARD_GetPBClock() / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    // Initialize hardware
    OledInit();
    AdcInit();
    ButtonsInit();
    LEDS_INIT();

    // Default setup
    oven.timeLeft = 2;
    oven.startTime = 2;
    oven.temp = 300;
    oven.mode = BAKE;
    oven.state = RESET;
    oven.buttonPressCounter = 0;
    oven.input = TIME;
    LEDS = 0;

    // Initialize Buttons
    data.event = 0;
    data.value = BUTTON_EVENT_NONE;

    // Initialize counter and flags
    timer5hzCounter = 0;
    timer5hzEvent = 0;
    timer2hzEvent = 0;

    DrawString(&oven);

    while (1) {
        switch (oven.state) {

        case START:
            if (AdcChanged() != 0) {
                UpdateAdcData(&oven);
            }
            if (data.event != 0) {
                if ((data.value & BUTTON_EVENT_3DOWN) != 0) { // Changes oven mode
                    oven.state = PENDING_SELECTOR_CHANGE;
                    oven.buttonPressCounter = timer5hzCounter;
                }
                if ((data.value & BUTTON_EVENT_4DOWN) != 0) { // Starts countdown
                    oven.state = COUNTDOWN;
                    DrawString(&oven);
                }
                data.event = 0;
            }
            DrawString(&oven);
            break;

        case COUNTDOWN:
            if (timer2hzEvent != 0 && oven.timeLeft > 0) { // Decrements every second
                oven.timeLeft--;
                DrawString(&oven);
                LightLEDS(&oven);
                timer2hzEvent = 0;
            }
            if (data.event != 0 && (data.value & BUTTON_EVENT_4DOWN) != 0) { // Begins reset
                oven.buttonPressCounter = timer5hzCounter;
                data.event = 0;
                oven.state = PENDING_RESET;
            }
            if (timer2hzEvent != 0 && oven.timeLeft == 0) { // Resets if timer reaches 0
                oven.state = RESET;
                DrawString(&oven);
                LEDS = 0;
                LEDS_SET(LEDS);
            }
            break;

        case PENDING_SELECTOR_CHANGE:
            if (timer5hzEvent != 0 && (timer5hzCounter - oven.buttonPressCounter) >= LONG_PRESS) { // If button 3 is held down
                if (oven.mode == BAKE) { // Changes selector
                    oven.input = !oven.input;
                }

                UpdateAdcData(&oven);
                DrawString(&oven);
                LEDS = 0;
                LEDS_SET(0);

                oven.state = START;
                timer5hzEvent = 0;
                data.event = 0;
            } else if ((data.value & BUTTON_EVENT_3UP) != 0) { // Changes oven mode
                if (oven.mode != BROIL) {
                    oven.mode++;

                    if (oven.mode == BROIL) {
                        oven.temp = 500;
                    }
                } else {
                    oven.mode = BAKE;
                    oven.temp = 300;
                }

                oven.input = TIME;

                UpdateAdcData(&oven);
                DrawString(&oven);
                LEDS = 0;
                LEDS_SET(0);

                data.event = 0;
                oven.state = START;
            }
            break;

        case PENDING_RESET:
            if (timer2hzEvent != 0 && oven.timeLeft == 0) { // Resets if timer reaches 0
                oven.state = RESET;
                timer2hzEvent = 0;
                LEDS = 0;
                LEDS_SET(0);
            } else if (data.event != 0 && (data.value & BUTTON_EVENT_4UP) != 0) { // Starts countdown
                oven.state = COUNTDOWN;
                data.event = 0;
            } else if (timer5hzEvent != 0 && (timer5hzCounter - oven.buttonPressCounter) >= LONG_PRESS) { // Resets if button 4 is held down
                timer5hzEvent = 0;
                oven.state = RESET;
            } else if (timer2hzEvent != 0 && oven.timeLeft > 0) { // Counts down
                oven.timeLeft--;
                timer2hzEvent = 0;
                DrawString(&oven);
            }
            break;

        case RESET: // Resets everything to default
            oven.temp = 300;
            oven.input = TIME;
            UpdateAdcData(&oven);
            DrawString(&oven);
            LEDS = 0;
            LEDS_SET(0);

            oven.state = START;
            break;
        }
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
    while (1);
}

void __ISR(_TIMER_1_VECTOR, ipl4auto) TimerInterrupt2Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 4;

    timer2hzEvent = 1;
}

void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

    timer5hzCounter++;
    timer5hzEvent = 1;
}

void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    data.value = ButtonsCheckEvents();

    if (data.value != BUTTON_EVENT_NONE) {
        data.event = 1;
    }
}

void DrawString(CookData *oven)
{
    OledClear(OLED_COLOR_BLACK);

    if (oven->state == COUNTDOWN || oven->state == PENDING_RESET) { // If oven is on
        if (oven->mode == BAKE || oven->mode == BROIL) { // If oven is in bake or broil mode
            sprintf(output, BAKEorBROILOnFormat,
                    0x01, 0x01, 0x01, 0x01,
                    ((oven->mode == BAKE) ? "Bake" : "Broil"), ((oven->timeLeft / 2) / 60), ((oven->timeLeft / 2) % 60),
                    oven->temp, 0xF8,
                    0x03, 0x03, 0x03, 0x03);
        } else if (oven->mode == TOAST) { // If oven is in toast mode
            sprintf(output, TOASTFormat,
                    0x02, 0x02, 0x02, 0x02,
                    ((oven->timeLeft / 2) / 60), ((oven->timeLeft / 2) % 60),
                    0x04, 0x04, 0x04, 0x04);
        }
    } else { // If oven is off
        if (oven->mode == BAKE || oven->mode == BROIL) { // If oven is in bake or broil mode
            if (oven->mode == BAKE) { // If oven is backing
                if (oven->input == TIME) { // If input selection is time
                    sprintf(output, BAKEorBROILOffFormat,
                            0x02, 0x02, 0x02, 0x02,
                            "Bake", ">", ((oven->startTime / 2) / 60), ((oven->startTime / 2) % 60),
                            " ", oven->temp, 0xF8,
                            0x04, 0x04, 0x04, 0x04);
                } else if (oven->input == TEMP) { // If input selection is temp
                    sprintf(output, BAKEorBROILOffFormat,
                            0x02, 0x02, 0x02, 0x02,
                            "Bake", " ", ((oven->startTime / 2) / 60), ((oven->startTime / 2) % 60),
                            ">", oven->temp, 0xF8,
                            0x04, 0x04, 0x04, 0x04);
                }
            } else if (oven->mode == BROIL) { // If oven is in broil mode
                sprintf(output, BAKEorBROILOffFormat,
                        0x02, 0x02, 0x02, 0x02,
                        "Broil", " ", ((oven->startTime / 2) / 60), ((oven->startTime / 2) % 60),
                        " ", 500, 0xF8,
                        0x04, 0x04, 0x04, 0x04);
            }
        } else if (oven->mode == TOAST) { // If oven is in toast mode
            printf("I'm here\n");
            sprintf(output, TOASTFormat,
                    0x02, 0x02, 0x02, 0x02,
                    ((oven->startTime / 2) / 60), ((oven->startTime / 2) % 60),
                    0x04, 0x04, 0x04, 0x04);
        }
    }
    OledDrawString(output);
    OledUpdate();
}

void UpdateAdcData(CookData *oven)
{
    if (oven->input == TIME) { // Sets time based on ADC
        oven->startTime = ((AdcRead() >> 2) * 2) + 2;
        oven->timeLeft = oven->startTime;
    } else if (oven->input == TEMP) { // // Sets temp based on ADC
        oven->temp = (AdcRead() >> 2) + 300;
    }
}

void LightLEDS(CookData *oven)
{
    uint8_t split = oven->startTime >> 3;

    uint8_t shift = 7 - (oven->timeLeft / split);

    if (shift > 8) {
        shift = 0;
    }

    LEDS = (0xFF << shift);
    LEDS_SET(LEDS);
}
