#include "stm8s.h"
#include "Uart.h"

void Uart2Init() {
	UART2_DeInit();

    UART2_Init((u32)115200, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);

    /* Enable the UART2 Transmit interrupt: this interrupt is generated when the
       UART2 transmit data register is empty */
    //UART2_ITConfig(UART2_IT_TXE, ENABLE);
    /* Enable the UART2 Receive interrupt: this interrupt is generated when the
       UART2 receive data register is not empty */
    UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
}

