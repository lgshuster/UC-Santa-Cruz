#include "xc.h"
#include "BOARD.h"
#include "Oled.h"

void main(void) {
    
    OledInit();
    OledDrawString("Hello\nWorldI'm\nWorking!\n");
    OledUpdate();
    
    while (1);
}