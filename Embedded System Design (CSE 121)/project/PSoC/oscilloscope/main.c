/*
Name: Leonid Shuster
Lab: Lab Project

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
#define DMA_2_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_2_DST_BASE (CYDEV_SRAM_BASE)

#define RD_BUFFER_SIZE        (2u)
#define WR_BUFFER_SIZE        (1u)

#define bufferSize 64 // Max buffer size
#define FALSE 0
#define TRUE 1


/* Variable declarations for DMA */
uint8 DMA_1_Chan;
uint8 DMA_2_Chan;
uint8 DMA_1_TD[2];
uint8 DMA_2_TD[2];

// Ping-pong buffers
uint8 chan1ppBuffer1[bufferSize];
uint8 chan1ppBuffer2[bufferSize];
uint8 chan2ppBuffer1[bufferSize];
uint8 chan2ppBuffer2[bufferSize];

uint8 read_buffer[RD_BUFFER_SIZE];
uint8 write_buffer[WR_BUFFER_SIZE];

uint8 readbyteCount;

int samp_rate; // Start signal and samp_rate

int16 pot1_data;
int16 pot2_data;

// Flags used to switch between buffers
volatile uint8 flag_switchchan1ppBuffers;
volatile uint8 flag_switchchan2ppBuffers;

volatile uint8 flag_muxSwitch;

// Interrupt triggered when first DMA transfer is done, sets a flag used to switch between first set of ping-pong buffers
CY_ISR(DMA1Interrupt) { 
    flag_switchchan1ppBuffers = !flag_switchchan1ppBuffers;
}

// Interrupt triggered when second DMA transfer is done, sets a flag used to switch between second set of ping-pong buffers
CY_ISR(DMA2Interrupt) {    
    flag_switchchan2ppBuffers = !flag_switchchan2ppBuffers;
}

int main(void)
{
    
    /* DMA Configuration for DMA_1 which transfers data from ADC to alternating ping-pong buffers */
    DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST, 
        HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
    
    DMA_1_TD[0] = CyDmaTdAllocate();
    DMA_1_TD[1] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_1_TD[0], bufferSize, DMA_1_TD[1], DMA_1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR );
    CyDmaTdSetConfiguration(DMA_1_TD[1], bufferSize, DMA_1_TD[0], DMA_1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    
    CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR), LO16((uint32)chan1ppBuffer1));
    CyDmaTdSetAddress(DMA_1_TD[1], LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR), LO16((uint32)chan1ppBuffer2));
    
    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
    
    /* DMA Configuration for DMA_2 */
    DMA_2_Chan = DMA_2_DmaInitialize(DMA_2_BYTES_PER_BURST, DMA_2_REQUEST_PER_BURST, 
        HI16(DMA_2_SRC_BASE), HI16(DMA_2_DST_BASE));
    
    DMA_2_TD[0] = CyDmaTdAllocate();
    DMA_2_TD[1] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_2_TD[0], bufferSize, DMA_2_TD[1], DMA_2__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA_2_TD[1], bufferSize, DMA_2_TD[0], DMA_2__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    
    CyDmaTdSetAddress(DMA_2_TD[0], LO16((uint32)ADC_SAR_2_SAR_WRK0_PTR), LO16((uint32)chan2ppBuffer1));
    CyDmaTdSetAddress(DMA_2_TD[1], LO16((uint32)ADC_SAR_2_SAR_WRK0_PTR), LO16((uint32)chan2ppBuffer2));
    
    CyDmaChSetInitialTd(DMA_2_Chan, DMA_2_TD[0]);
        
    // Set up slave write data buffer
    I2CS_SlaveInitWriteBuf((uint8 *) write_buffer, WR_BUFFER_SIZE);   
    // Set up slave read data buffer
    I2CS_SlaveInitReadBuf((uint8 *) read_buffer, RD_BUFFER_SIZE);
     // Start I2C slave component
    I2CS_Start();
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    while (samp_rate == 0) {
        if(I2CS_SlaveStatus() & I2CS_SSTAT_WR_CMPLT) {
            
            // Copy command into command register
            samp_rate = write_buffer[0];
            Clock_SetDividerValue(100000 / (samp_rate * 1000));
            
            // Clear status
            I2CS_SlaveClearWriteStatus();
            I2CS_SlaveClearWriteBuf();
            
            
        }
    }

    
    CyDmaChEnable(DMA_1_Chan, 1);
    CyDmaChEnable(DMA_2_Chan, 1);
    
    // Start ADCs, LCD, DMA interrupts and USB, and start ADC conversions
    ADC_SAR_1_Start();
    ADC_SAR_2_Start();
    ADC_DelSig_Start();
    ADC_SAR_1_StartConvert();
    ADC_SAR_2_StartConvert();
    ADC_DelSig_StartConvert();
    
    LCD_Start();
    LCD_ClearDisplay();
    
    AMux_Init();
    
    ISR_DMA_1_StartEx(DMA1Interrupt);
    ISR_DMA_2_StartEx(DMA2Interrupt);
   
   
    USBFS_Start(0u, USBFS_5V_OPERATION);
    
    while (!USBFS_GetConfiguration()){};
    
    for(;;)
    {
        /* Check if the slave buffer has been read */
        if(I2CS_SlaveStatus() & I2CS_SSTAT_RD_CMPLT) {
            readbyteCount = I2CS_SlaveGetReadBufSize();
         /* If both bytes of the read buffer have been read */
            if(readbyteCount == RD_BUFFER_SIZE) {
                // Clear read status
                I2CS_SlaveClearReadStatus();
                I2CS_SlaveClearReadBuf();
                /* Disable the interrupts when writing to the Read buffer
                * so that the Master cannot read the slave buffer during the act of writing
                * to the buffer.
                */
                CyGlobalIntDisable;
                /* Replace this code with your code to read potentiometer settings and
                 copy into read_buffer */
                
                if (flag_muxSwitch == FALSE) {
                    AMux_Select(0);
                    
                    pot1_data = ADC_DelSig_Read16();
                    
                    if (pot1_data < 0) {
                        pot1_data = 0;   
                    }
                    else if (pot1_data > 255) {
                        pot1_data = 255;   
                    } 
                    
                    read_buffer[0] = pot1_data;
                    flag_muxSwitch = TRUE;
                }
                else if (flag_muxSwitch == TRUE) {
                    AMux_Select(1);
                    
                    pot2_data = ADC_DelSig_Read16();
                    
                    if (pot2_data < 0) {
                        pot2_data = 0;   
                    }
                    else if (pot2_data > 255) {
                        pot2_data = 255;   
                    } 
                    
                    read_buffer[1] = pot2_data;
                    flag_muxSwitch = FALSE;
                }
                // Turn the interrupts back on
                CyGlobalIntEnable;
            }
        }
        
        if (USBFS_IsConfigurationChanged()){
            while (!USBFS_GetConfiguration()){};
        }
        
        // Wait until USB In Buffer for first endpoint and channel is empty
        //while (USBFS_GetEPState(0x1) != USBFS_IN_BUFFER_EMPTY){};
            
        // Check which first channel ping-pong buffer should be sent based on flag set by first DMA transfer interrupt
        if (flag_switchchan1ppBuffers == FALSE) {
            USBFS_LoadInEP(0x1, chan1ppBuffer1, bufferSize);
        }
        else if (flag_switchchan1ppBuffers == TRUE) {
            USBFS_LoadInEP(0x1, chan1ppBuffer2, bufferSize);
        }
        
        // Wait until USB In Buffer for second endpoint and channel is empty
        //while (USBFS_GetEPState(0x2) != USBFS_IN_BUFFER_EMPTY){};
            
        // Check which second channel ping-pong buffer should be sent based on flag set by second DMA transfer interrupt
        if (flag_switchchan2ppBuffers == FALSE) {
            USBFS_LoadInEP(0x2, chan2ppBuffer1, bufferSize);
        }
        else if (flag_switchchan2ppBuffers == TRUE) {
            USBFS_LoadInEP(0x2, chan2ppBuffer2, bufferSize);
        }
        
    }
}

/* [] END OF FILE */
