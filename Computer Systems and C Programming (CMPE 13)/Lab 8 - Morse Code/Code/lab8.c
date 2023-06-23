// Heap size: 2048 bytes

// **** Include libraries here ****
// Standard C libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Morse.h"
#include "Oled.h"

// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)
#define TRUE 1
#define FALSE 0

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
// Variables for event checking
static uint8_t timerEvent;
static MorseEvent eventData;

// Variables to keep morse input and output
static char morseInput[OLED_CHARS_PER_LINE]; // Space for morse code
static char decodedMorseOutput[(OLED_CHARS_PER_LINE * 3)]; // Space for decoded character
static char OLEDString[86]; // Space that OLED prints out

// **** Declare any function prototypes here ****
static void AppendMorseTop(char character);
static void AppendCharBottom(char character);
static void ClearMorseTop(void);
static void PrintOLED(void);

int main()
{
    BOARD_Init();


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

/******************************************************************************
 * Your code goes in between this comment and the following one with asterisks.
 *****************************************************************************/
    if (MorseInit() == SUCCESS) { // If Binary Tree was successfully generated
        OledInit();
        
        uint8_t error = FALSE; // Default to false
        morseInput[0] = '\0'; // Start with nothing in the string
        decodedMorseOutput[0] = '\0'; // Start with nothing in the string
        
        while (1) {
            if (timerEvent != FALSE) { // If event was triggered by interrupt
                switch(eventData) {
                case MORSE_EVENT_DOT:
                    if (MorseDecode(MORSE_CHAR_DOT) == STANDARD_ERROR) { // If decoding returns error
                        error = TRUE;
                    }
                    else { // If successful, append dot
                        AppendMorseTop('.');
                    }
                    PrintOLED();
                    break;
                case MORSE_EVENT_DASH:
                    if (MorseDecode(MORSE_CHAR_DOT) == STANDARD_ERROR) { // If decoding returns error
                        error = TRUE;
                    }
                    else { // If successful, append dash
                        AppendMorseTop('-');
                    }
                    PrintOLED();
                    break;
                case MORSE_EVENT_INTER_LETTER:
                    if (error == FALSE) { // If decoding was successful
                        AppendCharBottom(MorseDecode(MORSE_CHAR_END_OF_CHAR));
                    }
                    else { 
                        AppendCharBottom('#');
                    }
                    ClearMorseTop();
                    PrintOLED();
                    error = FALSE;
                    break;
                case MORSE_EVENT_INTER_WORD: 
                    if (error == FALSE) { // // If decoding was successful
                        AppendCharBottom(MorseDecode(MORSE_CHAR_END_OF_CHAR));
                    }
                    else {
                        AppendCharBottom('#');
                    }
                    AppendCharBottom(' ');
                    ClearMorseTop();
                    PrintOLED();
                    error = FALSE;
                    break;
                default:
                    break;
                }
                timerEvent = FALSE; // Resets event
            }
        }
    }
    
    else { // If Binary Tree was not successfully generated
        FATAL_ERROR();
    }
/******************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks.
 *****************************************************************************/

    while (1);
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //******** Put your code here *************//
    eventData = MorseCheckEvents();
    MorseEvent previousEvent = eventData; 
    
    if (eventData != MORSE_EVENT_NONE && eventData != previousEvent) { // Makes sure event hasn't repeated
        timerEvent = 1;
    } else {
        timerEvent = 0;
    }
}

static void AppendMorseTop(char character)
{
    int i;
    
    for (i = 0; i < (OLED_CHARS_PER_LINE - 1); i++) {
        if (morseInput[i] == '\0') { // If it reaches the end
            morseInput[i] = character;
            morseInput[(i + 1)] = '\0';
            break;
        }
    }
}

static void AppendCharBottom(char character)
{
    int i;

    for (i = 0; i < ((3 * OLED_CHARS_PER_LINE) - 1); i++) {
        if (decodedMorseOutput[i] == '\0') { // If it reaches the end
            decodedMorseOutput[i] = character;
            decodedMorseOutput[i + 1] = '\0';
            break;
        }
    }
}

static void ClearMorseTop(void)
{
    morseInput[0] = '\0'; // Clears string
}

static void PrintOLED(void)
{
    OledClear(OLED_COLOR_BLACK);
    
    sprintf(OLEDString, "%s\n%s", morseInput, decodedMorseOutput);
    
    OledDrawString(OLEDString);
    OledUpdate();
}