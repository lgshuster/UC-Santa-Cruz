/*
Name: Leonid Shuster
Lab Exercise: 5
Part: 1
Role: Continuously converts value from potentiometer using ADC and sends data over to Raspberry Pi
*/
#include "project.h"

#define bufferSize 1

int16 ADC_output; // Holds 16-bit value of bounded ADC output
uint8 buffer; // Holds 8-bit value of ADC output to send over USB

// Interrupt triggered every millisecond, used to read from ADC
CY_ISR(TimerInterrupt){
    ADC_output = ADC_Read16(); // Read 16-bit value from ADC
    
    // Bound ADC_output to 8 bit value
    if (ADC_output < 0) {
        ADC_output = 0;
    }
    else if (ADC_output > 255) {
        ADC_output = 255;   
    }
    
    buffer = ADC_output; // Move ADC output into unsigned 8 bit int
}

int main(void)
{
    CyGlobalIntEnable;
    
    // Start ADC, Timer, LCD, timer interrupt, and USB
    ADC_Start();
    Timer_Start();
    LCD_Start();
    ISR_TIMER_StartEx(TimerInterrupt);
    USBFS_Start(0u, USBFS_5V_OPERATION); 
    
    while (!USBFS_GetConfiguration()){}; // Wait until USB is configured
    
    for (;;) {
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintNumber(ADC_output);
    
        // If USB configuration changes, wait until USB is configured
        if (USBFS_IsConfigurationChanged()){
            while (!USBFS_GetConfiguration()){};   
        } 
        
        while (USBFS_GetEPState(0x1) != USBFS_IN_BUFFER_EMPTY){}; // Wait until USB In Buffer is empty to send data
        USBFS_LoadInEP(0x1, &buffer, bufferSize); // Send value of ADC over USB to Pi
    }
    
    return (0);
}
