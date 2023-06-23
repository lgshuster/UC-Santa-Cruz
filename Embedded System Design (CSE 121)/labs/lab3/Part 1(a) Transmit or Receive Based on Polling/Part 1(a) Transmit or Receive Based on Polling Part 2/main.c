/*
Name: Leonid Shuster
Lab Exercise: 3
Part: 1(a) Part 2
Role: Use UART to transfer data using software loop
*/

#include "project.h"

#define byteSize 4096 // Size of byte arrays
#define TRUE 1
#define FALSE 0

// Byte arrays used for transmitting and receiving data
uint8 TXDataArray[byteSize];
uint8 RXDataArray[byteSize];

// Used for polling of UART registers
uint8 TXstatus;
uint8 RXstatus;

// Used to keep track of number of transmits and receieves
int numTransfers;
int numReceives;

// Used to make sure program runs once
int runOnce;

// Used to check time spent for transferring and checking number of mismatches
uint64 time = 0;
int errors;


int main(void)
{
    // Start UART and LCD
    UART_Start();
    LCD_Start();
    
    // Initialize byte arrays
    for (int i = 0; i < byteSize; i++) {
        TXDataArray[i] = i%256;
        RXDataArray[i] = 0;
    }
    
    Timer_Start(); // Start timer
    for(;;) {
        // Poll statuses of UART TX and RX
        TXstatus = UART_ReadTxStatus();
        RXstatus = UART_ReadRxStatus();
        
        // Add byte of data to TX FIFO if it is not full
        if (TXstatus & UART_TX_STS_FIFO_NOT_FULL) {
            UART_PutChar(TXDataArray[numTransfers]);
            numTransfers++;
        }
        // Read byte of data from RX FIFO if it is non-empty
        if (RXstatus & UART_RX_STS_FIFO_NOTEMPTY) {
            RXDataArray[numReceives] = UART_GetChar();
            numReceives++;
        }
        // If max number of transfers and receives is reached
        if (numReceives == byteSize) {
            if (runOnce == FALSE) {
                runOnce = TRUE;  // Make sure program runs once
                Timer_Stop(); // Stop timer
                
                time = (Timer_ReadPeriod() - Timer_ReadCounter()) / 24; // Get time from timer
               
                LCD_ClearDisplay();
                
                // Print time of transfer
                LCD_Position(1,0);
                LCD_PrintNumber(time);
                LCD_PrintString(" us");
                
                // Check if byte arrays are equal
                for (int i = 0; i < byteSize; i++) {
                    if (TXDataArray[i] != RXDataArray[i]) {
                        errors++;   
                    }
                }
                
                // Print number of mismatches
                LCD_Position(0,0);
                LCD_PrintNumber(errors);
                LCD_PrintString(" errors");
                
                CyDelay(500);
            }
        }
    }
}

