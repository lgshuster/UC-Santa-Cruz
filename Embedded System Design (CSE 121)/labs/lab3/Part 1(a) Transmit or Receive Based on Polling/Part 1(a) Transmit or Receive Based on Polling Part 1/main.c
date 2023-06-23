/*
Name: Leonid Shuster
Lab Exercise: 3
Part: 1(a) Part 1
Role: Transmit byte 0xa5 continuously to UART and check serial data output with oscilloscope
*/

#include "project.h"
#define byteVal 0xa5

int main(void)
{
    // Start UART
    UART_Start();
 
    for(;;)
    {
        UART_WriteTxData(byteVal); // Transmits data to UART via TX
    }
}