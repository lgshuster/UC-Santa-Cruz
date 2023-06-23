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
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /*
        Pin_1_Write(!Pin_1_Read());
        Pin_2_Write(!Pin_2_Read());
        Pin_3_Write(!Pin_3_Read());
        CyDelay(500);
        */
         
        // For use with the oscilloscope
        
        Pin_1_Write(!Pin_1_Read());
        Pin_2_Write(!Pin_2_Read());
        Pin_3_Write(!Pin_3_Read());
        
    }
}

/* [] END OF FILE */
