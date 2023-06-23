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

#define NUM_BITS 7

int main(void)
{

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /*
        Control_Reg_1_Write(NUM_BITS);
        CyDelay(500);
        Control_Reg_1_Write(0);
        CyDelay(500);
        */
        
        //For use with oscilloscope
        
        Control_Reg_1_Write(NUM_BITS);
        Control_Reg_1_Write(0);
        
        
    }
}

/* [] END OF FILE */
