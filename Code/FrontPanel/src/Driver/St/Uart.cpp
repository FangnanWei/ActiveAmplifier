#include "Driver/St/Uart.h"


void Uart::SendData(uint8_t *buff, uint16_t length) {
  uint16_t i = 0;

  while (i != length) {
    UART3_SendData8(*(buff + i));
    while(UART3_GetFlagStatus(UART3_FLAG_TXE)  == RESET);
    i++;
  }
}