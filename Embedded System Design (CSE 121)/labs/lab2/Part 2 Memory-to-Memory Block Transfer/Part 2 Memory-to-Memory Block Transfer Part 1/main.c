#include <device.h>
#include <cytypes.h>

#define DMA_BYTES_PER_BURST 126
#define DMA_REQUEST_PER_BURST 0
#define DMA_SRC_BASE (CYDEV_SRAM_BASE)
#define DMA_DST_BASE (CYDEV_SRAM_BASE)
#define TDmax 4096
#define TRUE 1
#define FALSE 0

uint8 sourceArray[TDmax][4];
uint8 destinationArray[TDmax][4];
uint8 DMA_Chan;
uint8 DMA_TD[5];
float64 time = 0;
volatile uint8 flag_DMADone;

int mismatches = 0;

// Interrupt triggers once DMA is finished
CY_ISR(Interrupt) {
    flag_DMADone = TRUE;
    
    Timer_Stop();
    
    time = (Timer_ReadPeriod() - Timer_ReadCounter()) / 24; // Convert to ms
    
    LCD_Position(1u, 0u);
    LCD_PrintString("Time: ");
    LCD_PrintNumber(time);
    LCD_PrintString("us");
}

//  DMA WAY
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Initialize arrays
    for (int i = 0; i <= 4095; i++) {
        for (int j = 0; j <= 4; j++) {
            sourceArray[i][j] = ((4*i)+j)%256;
            destinationArray[i][j] = 0;
        }
    }
    
    // Open channel
    DMA_Chan = DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    
    // Allocating TDs
    DMA_TD[0] = CyDmaTdAllocate();
    DMA_TD[1] = CyDmaTdAllocate();
    DMA_TD[2] = CyDmaTdAllocate();
    DMA_TD[3] = CyDmaTdAllocate();
    DMA_TD[4] = CyDmaTdAllocate();
    
    
    // Configurating data and loop with TDs
    CyDmaTdSetConfiguration(DMA_TD[0], 4092, DMA_TD[1], TD_SWAP_EN | TD_INC_SRC_ADR | TD_INC_DST_ADR | TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[1], 4092, DMA_TD[2], TD_SWAP_EN | TD_INC_SRC_ADR | TD_INC_DST_ADR | TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[2], 4092, DMA_TD[3], TD_SWAP_EN | TD_INC_SRC_ADR | TD_INC_DST_ADR | TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[3], 4092, DMA_TD[4], TD_SWAP_EN | TD_INC_SRC_ADR | TD_INC_DST_ADR | TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[4], 16, CY_DMA_DISABLE_TD, TD_SWAP_EN | DMA__TD_TERMOUT_EN | TD_INC_SRC_ADR | TD_INC_DST_ADR);
    
    
    // Without endians
    /* 
    CyDmaTdSetConfiguration(DMA_TD[0], 4092, DMA_TD[1], TD_INC_SRC_ADR | TD_INC_DST_ADR | TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[1], 4092, DMA_TD[2], TD_INC_SRC_ADR | TD_INC_DST_ADR | TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[2], 4092, DMA_TD[3], TD_INC_SRC_ADR | TD_INC_DST_ADR | TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[3], 4092, DMA_TD[4], TD_INC_SRC_ADR | TD_INC_DST_ADR | TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[4], 16, CY_DMA_DISABLE_TD, DMA__TD_TERMOUT_EN | TD_INC_SRC_ADR | TD_INC_DST_ADR);
    */ 
    
    // Memory transfer locations
    CyDmaTdSetAddress(DMA_TD[0], LO16((uint32)&sourceArray[0][0]), LO16((uint32)&destinationArray[0][0]));
    CyDmaTdSetAddress(DMA_TD[1], LO16((uint32)&sourceArray[1023][0]), LO16((uint32)&destinationArray[1023][0]));
    CyDmaTdSetAddress(DMA_TD[2], LO16((uint32)&sourceArray[2046][0]), LO16((uint32)&destinationArray[2046][0]));
    CyDmaTdSetAddress(DMA_TD[3], LO16((uint32)&sourceArray[3069][0]), LO16((uint32)&destinationArray[3069][0]));
    CyDmaTdSetAddress(DMA_TD[4], LO16((uint32)&sourceArray[4092][0]), LO16((uint32)&destinationArray[4092][0]));

    CyDmaChSetInitialTd(DMA_Chan, DMA_TD[0]);
    
    CyDmaChEnable(DMA_Chan, 1);
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    LCD_Start();
    ISR_DMADone_StartEx(Interrupt);
    
    Timer_Start();
    
    CyDmaChSetRequest(DMA_Chan, CPU_REQ);
    
    while(flag_DMADone == 0); // Stay in loop until DMA is finished
    
    // Checking if arrays are equal
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
    LCD_PrintString("Mismatches:");
    LCD_PrintNumber(mismatches/4);

    for(;;)
    {
        
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
