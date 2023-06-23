#include "project.h"

int16 ADC_output;

CY_ISR(TimerInterrupt){
    ADC_output = ADC_Read16();
    if (ADC_output < 0) {
        ADC_output = 0;   
    }
    else if (ADC_output > 255) {
        ADC_output = 255;   
    }
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintNumber(ADC_output);
    UART_PutChar(ADC_output);
}

int main(void)
{
    CyGlobalIntEnable;
    
    ADC_Start();
    UART_Start();
    Timer_Start();
    LCD_Start();
    ISR_TIMER_StartEx(TimerInterrupt);
    
    for(;;)
    {
        
    }
    return (0);
}