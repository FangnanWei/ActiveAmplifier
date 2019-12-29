#ifndef Uart_H__
#define Uart_H__

extern "C" {
#include "stm8s.h"
}

class Uart{
public:
  Uart(uint32_t baudRate) {
    UART2_DeInit();
    UART2_Init(baudRate, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
    UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
    UART2_Cmd(ENABLE);
  }
  
private:
};


#endif