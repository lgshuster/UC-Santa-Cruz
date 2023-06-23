#ifndef LEDS_H
#define LEDS_H

#include "BOARD.h"
#include <xc.h>

#define LEDS_INIT() do { \
        TRISE = 0; \
        LATE = 0; \
}  while (0)

#define LEDS_GET() return LATE

#define LEDS_SET(x) LATE = (x)

//LEDS_INIT();

#endif