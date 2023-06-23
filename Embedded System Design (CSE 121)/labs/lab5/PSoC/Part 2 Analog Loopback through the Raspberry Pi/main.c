/*
Name: Leonid Shuster
Lab Exercise: 5
Part: 2
Role: Continuously reads and converts analog signal to digital, sends data to Raspberry Pi, 
        receives same data back from Pi, and then converts back to analog signal
*/

#include "project.h"

/* Defines for DMA_1 */
#define DMA_1_BYTES_PER_BURST 1
#define DMA_1_REQUEST_PER_BURST 1
#define DMA_1_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_1_DST_BASE (CYDEV_SRAM_BASE)

/* Defines for DMA_2 */
#define DMA_2_BYTES_PER_BURST 1
#define DMA_2_REQUEST_PER_BURST 1
#define DMA_2_SRC_BASE (CYDEV_SRAM_BASE)
#define DMA_2_DST_BASE (CYDEV_PERIPH_BASE)

#define bufferSize 64 // Max buffer size
#define TRUE 1
#define FALSE 0

/* Variable declarations for DMA */
uint8 DMA_1_Chan;
uint8 DMA_2_Chan;
uint8 DMA_1_TD[2];
uint8 DMA_2_TD[2];

// Ping-pong buffers
uint8 ppBuffer1[bufferSize];
uint8 ppBuffer2[bufferSize];
uint8 ppBuffer3[bufferSize];
uint8 ppBuffer4[bufferSize];      

// Flags used to switch between buffers
volatile uint8 flag_switchFirstPPBuffers;
volatile uint8 flag_switchSecondPPBuffers;

// Flag used to check data loss when one buffer becomes full while the other buffer is still sending data
volatile uint8 flag_FirstCurrentFlag;
volatile uint8 flag_SecondCurrentFlag;

// Interrupt triggered when first DMA transfer is done, sets a flag used to switch between first set of ping-pong buffers
CY_ISR(DMA1Interrupt) { 
    flag_switchFirstPPBuffers = !flag_switchFirstPPBuffers;
}

// Interrupt triggered when second DMA transfer is done, sets a flag used to switch between second set of ping-pong buffers
CY_ISR(DMA2Interrupt) {    
    flag_switchSecondPPBuffers = !flag_switchSecondPPBuffers;
}


int main(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* DMA Configuration for DMA_1 which transfers data from ADC to alternating ping-pong buffers */
    DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST, 
        HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
    
    DMA_1_TD[0] = CyDmaTdAllocate();
    DMA_1_TD[1] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_1_TD[0], bufferSize, DMA_1_TD[1], DMA_1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR );
    CyDmaTdSetConfiguration(DMA_1_TD[1], bufferSize, DMA_1_TD[0], DMA_1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    
    CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)ppBuffer1));
    CyDmaTdSetAddress(DMA_1_TD[1], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)ppBuffer2));
    
    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
    
    CyDmaChEnable(DMA_1_Chan, 1);
    

    /* DMA Configuration for DMA_2 which transfer data from alternating ping-pong buffers to DAC*/
    DMA_2_Chan = DMA_2_DmaInitialize(DMA_2_BYTES_PER_BURST, DMA_2_REQUEST_PER_BURST, 
        HI16(DMA_2_SRC_BASE), HI16(DMA_2_DST_BASE));
    
    DMA_2_TD[0] = CyDmaTdAllocate();
    DMA_2_TD[1] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_2_TD[0], bufferSize, DMA_2_TD[1], DMA_2__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetConfiguration(DMA_2_TD[1], bufferSize, DMA_2_TD[0], DMA_2__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR);
    
    CyDmaTdSetAddress(DMA_2_TD[0], LO16((uint32)ppBuffer3), LO16((uint32)DAC_Data_PTR));
    CyDmaTdSetAddress(DMA_2_TD[1], LO16((uint32)ppBuffer4), LO16((uint32)DAC_Data_PTR));
    
    CyDmaChSetInitialTd(DMA_2_Chan, DMA_2_TD[0]);
    
    CyDmaChEnable(DMA_2_Chan, 1);
    
    // Start ADC, DAC, LCD, DMA interrupts and USB, enable DAC, and start ADC conversion
    ADC_Start();
    DAC_Start();
    LCD_Start();
    ISR_DMA_1_StartEx(DMA1Interrupt);
    ISR_DMA_2_StartEx(DMA2Interrupt);
    
    DAC_Enable();
    ADC_StartConvert();
    USBFS_Start(0u, USBFS_5V_OPERATION);
    
    // Wait until USB is configured
    while (!USBFS_GetConfiguration()){}; 
    USBFS_EnableOutEP(0x2);
    
    for(;;)
    {
        // If USB configuration changes, wait until USB is configured
        if (USBFS_IsConfigurationChanged()){
            if (USBFS_GetConfiguration()){
                USBFS_EnableOutEP(0x2);
            }
        }
        // Only read when USB Out Buffer is full
        if (USBFS_GetEPState(0x2) == USBFS_OUT_BUFFER_FULL) {
            
            // Check which ping-pong buffer should be read based on flag set by second DMA transfer interrupt
            if (flag_switchSecondPPBuffers == FALSE) {
                USBFS_ReadOutEP(0x2, ppBuffer3, bufferSize);
            }
            else if (flag_switchSecondPPBuffers == TRUE) {
                USBFS_ReadOutEP(0x2, ppBuffer4, bufferSize);
            } 
        }
        
        // Wait until USB Out Buffer processes data
        while (USBFS_GetEPState(0x2) == USBFS_OUT_BUFFER_FULL){
            // Error checking if buffer needs to switch when reading
            flag_SecondCurrentFlag = flag_switchSecondPPBuffers;
            
            if (flag_SecondCurrentFlag != flag_switchSecondPPBuffers) {
                ErrorLED_Write(1);
            }
        }
        
        // Wait until USB In Buffer is empty to send data
        while (USBFS_GetEPState(0x1) != USBFS_IN_BUFFER_EMPTY){
            // Error checking if buffer needs to switch when writing
            flag_FirstCurrentFlag = flag_switchFirstPPBuffers;
            
            if (flag_FirstCurrentFlag != flag_switchFirstPPBuffers) {
                ErrorLED_Write(1);
            }
        
        }
            
        // Check which ping-pong buffer should be sent based on flag set by first DMA transfer interrupt
        if (flag_switchFirstPPBuffers == FALSE) {
            USBFS_LoadInEP(0x1, ppBuffer1, bufferSize);
        }
        else if (flag_switchFirstPPBuffers == TRUE) {
            USBFS_LoadInEP(0x1, ppBuffer2, bufferSize);
        } 
    }
}

/* [] END OF FILE */
