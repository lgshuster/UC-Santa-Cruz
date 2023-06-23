#include <device.h>
#include <cytypes.h>

#define TDmax 4096
#define TRUE 1
#define FALSE 0

uint8 sourceArray[TDmax][4];
uint8 destinationArray[TDmax][4];
uint64 time = 0;

int mismatches = 0;

// SOFTWARE WAY
int main(void)
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    LCD_Start();
    
    // Initialize arrays
    for (int i = 0; i <= 4095; i++) {
        for (int j = 0; j <= 4; j++) {
            sourceArray[i][j] = ((4*i)+j)%256;
            destinationArray[i][j] = 0;
        }
    }
    
    Timer_Start();
    
    // Transfer data through software
    for (int i = 0; i <= 4095; i++) {
        (destinationArray[i][0] = sourceArray[i][1]);
            
        (destinationArray[i][1] = sourceArray[i][0]);

        (destinationArray[i][2] = sourceArray[i][3]);
            
        (destinationArray[i][3] = sourceArray[i][2]);
    }
    
    Timer_Stop();
    
    time = (Timer_ReadPeriod() - Timer_ReadCounter()) / 24; // Convert to ms
    
    LCD_Position(1u, 0u);
    LCD_PrintString("Time: ");
    LCD_PrintNumber(time);
    LCD_PrintString("us");
    
    // Check if arrays are equal
    for (int i = 0; i <= 4095; i++) {
        if (sourceArray[i][0] != destinationArray[i][1]) {
            mismatches++;
        }
        if (sourceArray[i][1] != destinationArray[i][0]) {
            mismatches++;
        }
        if (sourceArray[i][2] != destinationArray[i][3]) {
            mismatches++;
        }
        if (sourceArray[i][3] != destinationArray[i][2]) {
            mismatches++;
        }
    }
    
    LCD_Position(0,0);
    LCD_PrintString("Mismatches: ");
    LCD_PrintNumber(mismatches);

    for(;;)
    {
        
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
