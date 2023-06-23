// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Leds.h"

// **** Set macros and preprocessor directives ****
#define LEFT 1
#define RIGHT 0
#define LEFT_LED 0x80
#define RIGHT_LED 0x01

// **** Declare any datatypes here ****
typedef struct {
    uint8_t event;
    uint8_t value;
} TimerResult;

// **** Define global, module-level, or external variables here ****
static TimerResult data;

// **** Declare function prototypes ****
int ChangeLED(int currentLED, int currentDirection);

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    LEDS_INIT(); // Initalize LEDS

    int currentDirection = RIGHT; // Starting direction
    int currentLED = LEFT_LED; // Starting position

    while (1) {
        if (data.event == TRUE) {
            if (currentLED == RIGHT_LED) { // If it reaches last LED on the right
                currentDirection = LEFT; // Change direction to left
            } else if (currentLED == LEFT_LED) { // If it reaches last LED on the left
                currentDirection = RIGHT; // Change direction to left
            }
            currentLED = ChangeLED(currentLED, currentDirection); // Calls function to get the next LED
            LEDS_SET(currentLED); // Turns on LED
            data.event = 0;
        }
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. During each call it increments a counter (the
 * value member of a module-level TimerResult struct). This counter is then checked against the
 * binary values of the four switches on the I/O Shield (where SW1 has a value of 1, SW2 has a value
 * of 2, etc.). If the current counter is greater than this switch value, then the event member of a
 * module-level TimerResult struct is set to true and the value member is cleared.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);

    // Increments counter
    (data.value)++;

    // Checks if counter is greater than switch value
    if (data.value > SWITCH_STATES()) {
        data.event = TRUE;
        data.value = 0;
    }
}

// Moves onto next LED
int ChangeLED(int currentLED, int currentDirection)
{    
    int nextLED = 0;
    
    if (currentDirection == RIGHT) {
        nextLED = currentLED >> 1;
    } else if (currentDirection == LEFT) {
        nextLED = currentLED << 1;
    }
    return nextLED;
}