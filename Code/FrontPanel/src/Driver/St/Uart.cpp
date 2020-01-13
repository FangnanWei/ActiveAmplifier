#include "Driver/St/Uart.h"


void Uart::SendData(uint8_t *buff, uint16_t length) {
	for (uint16_t i = 0; i < length; i++) {
		UART3_SendData8(buff[i]);
		while (!UART3_GetFlagStatus(UART3_FLAG_TXE));
	}
}