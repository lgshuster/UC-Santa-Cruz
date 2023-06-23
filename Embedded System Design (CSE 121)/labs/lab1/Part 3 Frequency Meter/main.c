/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

int main(void)
{
    LCD_Start();
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert();
    PWM_1_Start();
    
    int32 ADC_output;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        ADC_output = ADC_DelSig_1_GetResult32();
        if (ADC_output < 0x0000) {
            ADC_output = 0;
        }
        else if (ADC_output > 0xFFFF) {
            ADC_output = 0xFFFF;
        }
        
        
        LCD_Position(0, 0);
        LCD_PrintString("Frequency: ");
        LCD_PrintNumber(ADC_output * 1000 / 0xFFFF);
        LCD_PrintString(" khz");
        
        PWM_1_WritePeriod(ADC_output * 1000 / 0xFFFF);
        PWM_1_WriteCompare(ADC_output * 500 / 0xFFFF);

        
        
        /*
        PWM_1_WriteCompare(ADC_output * 999 / 0xFFFF);
        PWM_1_WritePeriod(ADC_output * 999);
        
        LCD_Position(0, 0);
        LCD_PrintString("Frequency: ");
        LCD_PrintNumber(PWM_1_ReadPeriod()/999);
        LCD_PrintString(" khz");
        */
    }
}

/* [] END OF FILE */
