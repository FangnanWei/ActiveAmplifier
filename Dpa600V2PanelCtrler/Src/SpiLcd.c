#include "SpiLcd.h"
#include "stm8s_spi.h"

char SpiLcdWrite(u8 data)
{
 #if 0
   int timeout = SPI_WaiteAckTime;

    /* Loop while DR register in not emplty */
    do {
        timeout--;
        if (timeout < 1)
        {
            SpiLcdInit();
            return 0;
        }
    }
#endif
    while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
    /* Send byte through the SPI3 peripheral */
    SPI_SendData(data);

#if 0
    /* Wait to receive a byte */
    do {
        timeout--;
        if (timeout < 1)
        {
            SpiLcdInit();
            return 0;
        }
    }
    while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
#endif
    /* Return the byte read from the SPI bus */
    return 0;//SPI_ReceiveData();
}

void SpiLcdInit() {
	#if 1
   SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER,\
        SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE, \
        SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
   SPI_Cmd(ENABLE);
   #endif
   GPIO_Init(SpiGpioGroup, SpiClkPin | SpiSdoPin, GPIO_MODE_OUT_PP_HIGH_FAST);
   GPIO_Init(SPI_CS , SPI_Pin_CS, GPIO_MODE_OUT_PP_HIGH_FAST);
   SPI_CS_HIGH();
}

