/**************************************************************************
	File		: uart.c
	Author		: TaoLei Zhang
	Date		: 2018-2-5
***************************************************************************/

/**************************************************************************
    								include
***************************************************************************/
#include "uart.h"

/**************************************************************************
    								macro
***************************************************************************/

/**************************************************************************
    								typedef
***************************************************************************/

/**************************************************************************
    								variables
***************************************************************************/
ComFifo_t g_ComFifo;
ComFifo_t g_ComFifo_2;		/* for uart2 */

volatile unsigned char g_Usart1RevBuf[USART_REV_BUF_SIZE] = {0};
volatile unsigned char g_Usart2RevBuf[USART_REV_BUF_SIZE] = {0};

const uint32_t BAUD_RATE_TABLE[] = {
	115200,
	57600,
	38400,
	19200,
	9600,
	4800,
	2400
};

DI_UartConfig_t g_Usart2Cfg = {
/*
	9600,
	8,
	1,
	MCU_UART_CHECK_None,
	MCU_UART_RS232
*/
	9600,
	8,
	1,
	MCU_UART_CHECK_None,
	MCU_UART_RS232
};

/**************************************************************************
    								functions
***************************************************************************/
void USART1_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

  USART_Cmd(USART1, DISABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* uart1 rx dma */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* usart 1 dma config */
  DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_Usart1RevBuf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = USART_REV_BUF_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel5,&DMA_InitStructure);
  DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
  DMA_Cmd(DMA1_Channel5, ENABLE);

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

  /* usart1 interrupt config */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART1, USART_IT_TC, DISABLE);
  USART_ITConfig(USART1, USART_IT_IDLE,ENABLE);

  /* enable usart1 dma receive */
  USART_DMACmd(USART1, USART_DMAReq_Rx,ENABLE);

  USART_Cmd(USART1, ENABLE);
}

void USART2_Configuration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 
  NVIC_InitTypeDef NVIC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

  USART_Cmd(USART2, DISABLE);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 

  /* uart2 rx dma */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure); 

  /* usart 2 dma config */
  DMA_DeInit(DMA1_Channel6);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_Usart2RevBuf;  
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  
  DMA_InitStructure.DMA_BufferSize = USART_REV_BUF_SIZE;  
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;  
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
  DMA_Init(DMA1_Channel6,&DMA_InitStructure);  
  DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);  
  DMA_Cmd(DMA1_Channel6, ENABLE); 

 /********************以下为USART2配置**************************/
  //必须分步前后使能，不然无法进入中断
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

 //不能用原来的配置
 //Usart2Cfg(g_Usart2Cfg);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);

  /* usart2 interrupt config */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure); 
  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART2, USART_IT_TC, DISABLE);
  USART_ITConfig(USART2, USART_IT_IDLE,ENABLE);

  /* enable usart2 dma receive */
  USART_DMACmd(USART2, USART_DMAReq_Rx,ENABLE); 
  
  USART_Cmd(USART2, ENABLE); 
}

//注意：中断处理函数由于是汇编调用，必须写成C的形式，不然向量表找不到
extern"C" {

void USART1_IRQHandler(void)
{
	uint32_t temp;
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		temp = USART1->SR;
		temp = USART1->DR;

		DMA_Cmd(DMA1_Channel5,DISABLE);
		temp = USART_REV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);
		WriteComFifo(&g_ComFifo, (char *)g_Usart1RevBuf, temp);
		DMA_SetCurrDataCounter(DMA1_Channel5, USART_REV_BUF_SIZE);
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}

void USART2_IRQHandler(void)  
{
    uint32_t temp;
    if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        //清USART_IT_IDLE标志
        temp = USART2->SR;
        temp = USART2->DR; 
        
        DMA_Cmd(DMA1_Channel6,DISABLE);  
        temp = USART_REV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6); 
        WriteComFifo(&g_ComFifo_2, (char *)g_Usart2RevBuf, temp);
        DMA_SetCurrDataCounter(DMA1_Channel6, USART_REV_BUF_SIZE);  
        DMA_Cmd(DMA1_Channel6, ENABLE);  
    }
}

void DMA1_Channel5_IRQHandler(void)
{
	uint32_t temp;
	if (DMA_GetITStatus(DMA1_IT_GL5) != RESET)
	{
		DMA_Cmd(DMA1_Channel5, DISABLE);
		temp = USART_REV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);
		WriteComFifo(&g_ComFifo, (char *)g_Usart1RevBuf, temp);
		DMA_ClearITPendingBit(DMA1_IT_GL5);
		DMA_SetCurrDataCounter(DMA1_Channel5, USART_REV_BUF_SIZE);
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}

void DMA1_Channel6_IRQHandler(void)
{
    uint32_t temp;
    if (DMA_GetITStatus(DMA1_IT_GL6) != RESET)
    {
        DMA_Cmd(DMA1_Channel6, DISABLE);
        temp = USART_REV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6); 
        WriteComFifo(&g_ComFifo_2, (char *)g_Usart2RevBuf, temp);
        DMA_ClearITPendingBit(DMA1_IT_GL6);
        DMA_SetCurrDataCounter(DMA1_Channel6, USART_REV_BUF_SIZE);  
        DMA_Cmd(DMA1_Channel6, ENABLE);
    }
}
}
void Usart1SendData(uint8_t *send_buff,unsigned short length)
{
	unsigned int i = 0;
	for(i = 0;i < length;i ++)
	{
		while((USART1->SR&0X40)==0);
		USART1->DR = send_buff[i];
	}
	while((USART1->SR&0X40)==0);
}

void Usart2SendData(uint8_t *send_buff,unsigned short length)
{
 	unsigned int i = 0;
	for(i = 0;i < length;i ++)
	{	
	    while((USART2->SR&0X40)==0);	
		USART2->DR = send_buff[i];
	}	
    while((USART2->SR&0X40)==0);	
}

unsigned short Usart1RecvData(unsigned char *recvBuf, unsigned short length)
{
	return ReadComFifo(&g_ComFifo, (char *)recvBuf, length);
}

unsigned short Usart2RecvData(unsigned char *recvBuf, unsigned short length)
{
    return ReadComFifo(&g_ComFifo_2, (char *)recvBuf, length);
}


