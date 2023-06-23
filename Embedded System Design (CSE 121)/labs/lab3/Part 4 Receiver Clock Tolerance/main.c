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
uint8 PrimaryTXstatus;
uint8 PrimaryRXstatus;
uint8 SecondaryRXstatus;
uint8 SecondaryTXstatus;

// Flag for completion of receiving data
volatile uint8 flag_TRANSFER_COMPLETE;

// Used to keep track of number of transmits and receieves
int numPrimaryTransfers;
int numPrimaryReceives;
int numSecondaryReceives;

// Used to count number of interrupts and mismatches
int errors;
int runOnce;

// Interrupt is triggered when TX_FIFO is not full
CY_ISR(PrimaryTXInterrupt) {
    PrimaryTXstatus = UART_Primary_ReadTxStatus(); // Get status of TX register
    
    // Add bytes of data to TX FIFO until it is full
    while (PrimaryTXstatus & UART_Primary_TX_STS_FIFO_NOT_FULL) {
        UART_Primary_PutChar(TXDataArray[numPrimaryTransfers]);
        numPrimaryTransfers++;
        PrimaryTXstatus = UART_Primary_ReadTxStatus(); // Update status of TX register
    }
    if (numPrimaryTransfers == byteSize) {
        ISR_PRIMARY_TX_FIFO_NOTFULL_Disable();
    }
}

// Interrupt is triggered when RX_FIFO is not empty
CY_ISR(SecondaryRXInterrupt) {
    SecondaryRXstatus = UART_Secondary_ReadRxStatus(); // Get status of RX register
    
    // Read bytes of data from RX FIFO until it is empty
    while (SecondaryRXstatus & UART_Secondary_RX_STS_FIFO_NOTEMPTY) {
        RXDataArray[numSecondaryReceives] = UART_Secondary_GetChar();
        numSecondaryReceives++;
        SecondaryRXstatus = UART_Secondary_ReadRxStatus(); // Update status of RX register
    }
    // Set flag to true if received all the data
    if (numSecondaryReceives == byteSize) {
        flag_TRANSFER_COMPLETE = TRUE;
        ISR_SECONDARY_RX_FIFO_NOTEMPTY_Disable();
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

    // Start UART, LCD, and TX and RX interrupts
    UART_Primary_Start();
    UART_Secondary_Start();
    LCD_Start();
    ISR_PRIMARY_TX_FIFO_NOTFULL_StartEx(PrimaryTXInterrupt);
    ISR_SECONDARY_RX_FIFO_NOTEMPTY_StartEx(SecondaryRXInterrupt);
    
    for(;;)
    {
        while (flag_TRANSFER_COMPLETE != TRUE) {
            /*
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintNumber(numPrimaryTransfers);
            LCD_Position(0,5);
            LCD_PrintNumber(numSecondaryEchoed);
            LCD_Position(0,10);
            LCD_PrintNumber(numPrimaryReceives);
            CyDelay(100);
            */
        }
        // HIGHEST CLOCK CAN GO WITHOUT ERRORS IS 24 MGHZ
        // LOWEST CLOCK CAN GO WITHOUT ERRORS IS 24/25 MHGZ

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
            LCD_Position(1,0);
            LCD_PrintNumber(errors);
            LCD_PrintString(" errors");
            
            CyDelay(500);
        }
    }
}

/* [] END OF FILE */
