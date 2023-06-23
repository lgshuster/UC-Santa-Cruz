// Header guards
#ifndef LEDS_H
#define LEDS_H

// User Libraries
#include "BOARD.h"
#include <xc.h>

// Initializes LEDS
#define LEDS_INIT() do { \
       TRISE = 0x00; \
       LATE = 0x00; \
} while (0)
// Gets value of LEDS
#define LEDS_GET() LATE
// Sets value of LEDS
#define LEDS_SET(x) LATE = (x)

// End Header guard
#endif