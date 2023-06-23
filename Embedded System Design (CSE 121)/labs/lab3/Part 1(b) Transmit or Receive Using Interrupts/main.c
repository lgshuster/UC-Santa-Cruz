/*
Name: Leonid Shuster
Lab Exercise: 3
Part: 1(b)
Role: Use UART to transfer data using interrupts
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

// Used to keep track of number of transmits and receieves
int numTransfers;
int numReceives;

// Used to count number of interrupts and mismatches
int numTXInterrupts;
int numRXInterrupts;
int errors;

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
    numTXInterrupts++;
}

// Interrupt is triggered when RX_FIFO is not empty
CY_ISR(RXInterrupt) {
    RXstatus = UART_ReadRxStatus(); // Get status of RX register
    
    // Read bytes of data from RX FIFO until it is empty
    while (RXstatus & UART_RX_STS_FIFO_NOTEMPTY) {
        RXDataArray[numReceives] = UART_GetChar();
        numReceives++;
        RXstatus = UART_ReadRxStatus(); // Update status of RX register
    }
    // Set flag to true if received all the data
    if (numReceives == byteSize) {
        flag_TRANSFER_COMPLETE = TRUE;
        ISR_RX_FIFO_NOTEMPTY_Disable();
    }
    numRXInterrupts++;
}

int main(void)
{
    CyGlobalIntEnable;
    
    // Initialize byte arrays
    for (int i = 0; i < byteSize; i++) {
        TXDataArray[i] = i%256;
        RXDataArray[i] = 0;
    }

    // Start UART, LCD, and TX and RX interrupts
    UART_Start();
    LCD_Start();
    ISR_TX_FIFO_NOTFULL_StartEx(TXInterrupt);
    ISR_RX_FIFO_NOTEMPTY_StartEx(RXInterrupt);
    
    for(;;)
    {
        
        // Stay in idle loop until all the data is transferred and received
        while (flag_TRANSFER_COMPLETE != TRUE) {}

        LCD_ClearDisplay();
        
        // Print number of interrupts
        LCD_Position(1,0);
        LCD_PrintNumber(numTXInterrupts);
        LCD_PrintString(" ");
        LCD_PrintNumber(numRXInterrupts);
        LCD_PrintString(" ");
        LCD_PrintNumber(numTXInterrupts+numRXInterrupts);
        LCD_PrintString(" ");
        
        // Check if byte arrays are equal
        for (int i = 0; i < byteSize; i++) {
            if (TXDataArray[i] != RXDataArray[i]) {
                errors++;   
            }
        }
        
        // Print number of mismatches
        LCD_Position(0,0);
        LCD_PrintNumber(errors);
        LCD_PrintString(" mismatches");
        
        CyDelay(500);
        
    }
}

/* [] END OF FILE */
