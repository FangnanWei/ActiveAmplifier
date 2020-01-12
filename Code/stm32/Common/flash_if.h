/**
  ******************************************************************************
  * @file    flash_if.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    07/16/2010
  * @brief   Header for flash_if.c module
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_flash.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Flash Addresses*/
#define FLASH_PAGE_SIZE   0x800


#define Bootloader_Addr              			((uint32_t)0x08000000)
#define USER_FLASH_FIRST_PAGE_ADDRESS 			0x08010000				/* 64k */
#define USER_FLASH_LAST_PAGE_ADDRESS   			0x0803f800				
#define USER_FLASH_END_ADDRESS         			0x0803fFFF


#define DSP_FIRST_PAGE_ADDRESS					0x08040000				/* 256k */
#define DSP_LAST_PAGE_ADDRESS					0x08043800
#define DSP_END_ADDRESS							0x08043FFF				/* size 16k */

#define USER_CFG_FIRST_PAGE_ADDRESS 			0x0807C000
#define USER_CFG_LAST_PAGE_ADDRESS   			0x0807F800
#define USER_CFG_END_ADDRESS         			0x0807FFFF

#define UPDATE_DSP_TOPO_FLASH_FLAG_ADDRESS		0x0807E000				/* DSP Topo升级标记 */
#define UPDATE_FLASH_FLAG_ADDRESS   			0x0807E800  				/* 升级标志地址 */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FLASH_If_Write(__IO uint32_t* Address, uint32_t* Data, uint16_t DataLength);
int8_t FLASH_If_Erase(uint32_t start, uint32_t end, uint32_t pageSize);
void FLASH_If_Init(void);
void FLASH_if_UnInit(void);

int8_t FLASH_CFG_Erase(uint32_t Page_Address);
void FLASH_CFG_Write(uint32_t Page_Address, uint32_t* Data ,uint16_t DataLength);

#endif /* __FLASH_IF_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
