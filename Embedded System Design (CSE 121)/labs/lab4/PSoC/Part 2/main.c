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

#define byteValue 64
#define TRUE 1
#define FALSE 0

/* Variable declarations for DMA */
uint8 DMA_1_Chan;
uint8 DMA_2_Chan;
uint8 DMA_1_TD[2];
uint8 DMA_2_TD[2];

uint8_t ppBuffer1[byteValue];
uint8_t ppBuffer2[byteValue];
uint8_t ppBuffer3[byteValue];
uint8_t ppBuffer4[byteValue];

uint8 TXstatus;
uint8 RXstatus;

int ppBuffer1Index;
int ppBuffer2Index;
int ppBuffer3Index;
int ppBuffer4Index;

volatile uint8 flag_switchFirstPPBuffers;
volatile uint8 flag_switchSecondPPBuffers;
volatile uint8 flag_startTXtransmit;


CY_ISR(DMA1Interrupt) {    
    flag_startTXtransmit = TRUE;
}


CY_ISR(DMA2Interrupt) {    
    
}


CY_ISR(RXInterrupt) {
    RXstatus = UART_ReadRxStatus();
    
    if (flag_switchSecondPPBuffers == FALSE) {
        while (RXstatus & UART_RX_STS_FIFO_NOTEMPTY) {
            ppBuffer3[ppBuffer3Index] = UART_GetChar();
            ppBuffer3Index++;
            RXstatus = UART_ReadRxStatus();
        }  
        
        if (ppBuffer3Index == byteValue) {
            ppBuffer3Index = 0;
            flag_switchSecondPPBuffers = TRUE;
        }
    }
    
    else if (flag_switchSecondPPBuffers == TRUE) {
        while (RXstatus & UART_RX_STS_FIFO_NOTEMPTY) {
            ppBuffer4[ppBuffer4Index] = UART_GetChar();
            ppBuffer4Index++;
            RXstatus = UART_ReadRxStatus();
        }  
        
        if (ppBuffer4Index == byteValue) {
            ppBuffer4Index = 0;
            flag_switchSecondPPBuffers = FALSE;
        }
    }
    
}

   


void startTXtransmit(void) {
    
    TXstatus = UART_GetTxBufferSize();
    
    if (flag_switchFirstPPBuffers == FALSE) {
        while (ppBuffer1Index != byteValue) {
            if (TXstatus == 0 || TXstatus == 1) {
                UART_PutChar(ppBuffer1[ppBuffer1Index]);
                ppBuffer1Index++;

            }
            TXstatus = UART_GetTxBufferSize();
        }  
        ppBuffer1Index = 0;
        flag_switchFirstPPBuffers = TRUE;
    }
    
    else if (flag_switchFirstPPBuffers == TRUE) {
        while (ppBuffer2Index != byteValue) {
            if (TXstatus == 0 || TXstatus == 1) {
                UART_PutChar(ppBuffer2[ppBuffer2Index]);
                ppBuffer2Index++;
            }
            TXstatus = UART_GetTxBufferSize();
        }
        ppBuffer2Index = 0;
        flag_switchFirstPPBuffers = FALSE;
    }
    flag_startTXtransmit = FALSE;
}




int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    ADC_Start();
    ADC_StartConvert();
    DAC_Start();
    DAC_Enable();
    UART_Start();
    LCD_Start();
    ISR_DMA_1_StartEx(DMA1Interrupt);
    //ISR_DMA_2_StartEx(DMA2Interrupt);
    ISR_RX_FIFO_NOT_EMPTY_StartEx(RXInterrupt);



    /* Variable declarations for DMA_1 */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_1_Chan;
    uint8 DMA_1_TD[2];

    /* DMA Configuration for DMA_1 */
    DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST, 
        HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
    
    DMA_1_TD[0] = CyDmaTdAllocate();
    DMA_1_TD[1] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_1_TD[0], 64, DMA_1_TD[1], DMA_1__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_1_TD[1], 64, DMA_1_TD[0], DMA_1__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    
    CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)ppBuffer1));
    CyDmaTdSetAddress(DMA_1_TD[1], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)ppBuffer2));
    
    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
    
    CyDmaChEnable(DMA_1_Chan, 1);


    
    /* Variable declarations for DMA_2 */
    /* Move these variable declarations to the top of the function */
    
    uint8 DMA_2_Chan;
    uint8 DMA_2_TD[2];
    

    /* DMA Configuration for DMA_2 */
    
    DMA_2_Chan = DMA_2_DmaInitialize(DMA_2_BYTES_PER_BURST, DMA_2_REQUEST_PER_BURST, 
        HI16(DMA_2_SRC_BASE), HI16(DMA_2_DST_BASE));
    
    DMA_2_TD[0] = CyDmaTdAllocate();
    DMA_2_TD[1] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_2_TD[0], 64, DMA_2_TD[1], DMA_2__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_2_TD[1], 64, DMA_2_TD[0], DMA_2__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    
    CyDmaTdSetAddress(DMA_2_TD[0], LO16((uint32)ppBuffer3), LO16((uint32)DAC_Data_PTR));
    CyDmaTdSetAddress(DMA_2_TD[1], LO16((uint32)ppBuffer4), LO16((uint32)DAC_Data_PTR));
    
    CyDmaChSetInitialTd(DMA_2_Chan, DMA_2_TD[0]);
    
    
    CyDmaChEnable(DMA_2_Chan, 1);
    

    
    

    for(;;)
    {
        if (flag_startTXtransmit == TRUE) {
            startTXtransmit();   
        }
        /*
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintNumber(ppBuffer1Index);
        LCD_Position(1,0);
        LCD_PrintNumber(ppBuffer2Index);
        CyDelay(500);
        */
    }
}

/* [] END OF FILE */
