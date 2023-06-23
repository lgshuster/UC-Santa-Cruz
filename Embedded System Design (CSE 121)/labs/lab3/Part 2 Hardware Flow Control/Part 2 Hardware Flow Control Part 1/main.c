/*
Name: Leonid Shuster
Lab Exercise: 3
Part: 2 Part 1
Role: Use UART to transfer data using TX interrupt and Timer interrupt
*/
#include "project.h"

#define byteSize 4096 // Size of byte arrays
#define TRUE 1
#define FALSE 0

// Byte arrays used for transmitting and receiving data
uint8 TXDataArray[byteSize];
uint8 RXDataArray[byteSize];

// Used for polling of TX and RX registers
uint8 TXstatus;
uint8 RXstatus;

// Flag for completion of receiving data
volatile uint8 flag_TRANSFER_COMPLETE;
volatile uint8 flag_RX_ERROR;
volatile uint8 flag_RX_FIFO_OVERFLOW;

// Used to keep track of number of transmits and receieves
int numTransfers;
int numReceives;

// Used to count number of interrupts and mismatches
int errors;
int runOnce;

// Interrupt is triggered when TX_FIFO is not full
CY_ISR(TXInterrupt) {
    TXstatus = UART_ReadTxStatus(); // Get status of TX register
    
    // Add bytes of data to TX FIFO until it is full
    while (TXstatus & UART_TX_STS_FIFO_NOT_FULL) {
        UART_PutChar(TXDataArray[numTransfers]);
        numTransfers++;
        TXstatus = UART_ReadTxStatus(); // Update status of TX register
    }
    if (numTransfers == byteSize) {
        ISR_TX_FIFO_NOTFULL_Disable();
    }
}

// Interrupt is triggered when Timer goes off
CY_ISR(TimerInterrupt) {
    RXstatus = UART_ReadRxStatus(); // Get status of RX register
    
    // Read bytes of data from RX FIFO until it is empty
    while (RXstatus & UART_RX_STS_FIFO_NOTEMPTY) {
        if ((RXstatus & UART_RX_STS_PAR_ERROR) || (RXstatus & UART_RX_STS_STOP_ERROR) || (RXstatus & UART_RX_STS_OVERRUN)) {
            if (RXstatus & UART_RX_STS_OVERRUN) {
                flag_RX_FIFO_OVERFLOW = TRUE;
            }
            errors++;
            flag_RX_ERROR = TRUE;
        }
        RXDataArray[numReceives] = UART_GetChar();
        numReceives++;
        RXstatus = UART_ReadRxStatus(); // Update status of RX register
    }
    // Set flag to true if received all the data
    if (numReceives == byteSize || flag_RX_ERROR) {
        flag_TRANSFER_COMPLETE = TRUE;
        ISR_TIMER_Disable();
    }
}

int main(void)
{
    CyGlobalIntEnable;
    
    // Initialize byte arrays
    for (int i = 0; i < byteSize; i++) {
        TXDataArray[i] = i%256;
        RXDataArray[i] = 0;
    }

    // Start UART, Timer, TX and RX interrupts, and LCD
    UART_Start();
    Timer_Start();
    ISR_TX_FIFO_NOTFULL_StartEx(TXInterrupt);
    ISR_TIMER_StartEx(TimerInterrupt);
    LCD_Start();
    
    for(;;)
    {
        while (flag_TRANSFER_COMPLETE != TRUE) { }
        
        if (runOnce == FALSE) {
            runOnce = TRUE;
            // Check if byte arrays are equal
            for (int i = 0; i < byteSize; i++) {
                if (TXDataArray[i] != RXDataArray[i]) {
                    errors++;   
                }
            }
            
            LCD_ClearDisplay();
            
            // Print number of mismatches
            LCD_Position(0,0);
            LCD_PrintNumber(errors);
            LCD_PrintString(" errors");
            

            if (flag_RX_ERROR == TRUE) {
                LCD_Position(1,0);
                LCD_PrintString("Errors");
            }
            if (flag_RX_FIFO_OVERFLOW == TRUE) {
                LCD_Position(1,7);
                LCD_PrintString("Overflow");
            }
            
            
            CyDelay(500);
        }
        
    }
}

/* [] END OF FILE */
