/**
  ******************************************************************************
  * @file    flash_if.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    07/16/2010
  * @brief   This file provides high level routines to manage internal Flash
  *          programming (erase and write).
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

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{
  FLASH_Unlock();
}

void FLASH_if_UnInit(void)
{
    FLASH_Lock();
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  StartSector: start of user flash area
  * @retval 0 if success, -1 if error
  */
int8_t FLASH_If_Erase(uint32_t start, uint32_t end, uint32_t pageSize)
{
  uint32_t FlashAddress;

  FlashAddress = start;

  while (FlashAddress <= end)
  {
    if (FLASH_ErasePage(FlashAddress) == FLASH_COMPLETE)
    {
      FlashAddress += pageSize;
    }
    else
    {
      return -1;
    }
  }
  return 0;
}

/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned)
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer (unit is 32-bit word)
  * @retval None
  */
void FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint16_t DataLength)
{
  uint32_t i = 0;

  for (i = 0; i < DataLength; i++)
  {
    if (*FlashAddress <= (DSP_END_ADDRESS-4))
    {
      if (FLASH_ProgramWord(*FlashAddress, *(uint32_t*)(Data + i)) == FLASH_COMPLETE)
      {
        *FlashAddress += 4;
      }
      else
      {
        return;
      }
    }
    else
    {
      return;
    }
  }
}

int8_t FLASH_CFG_Erase(uint32_t Page_Address)
{
  if ((Page_Address >= USER_CFG_FIRST_PAGE_ADDRESS)
    && (Page_Address <= USER_CFG_LAST_PAGE_ADDRESS))
  {
    if (FLASH_ErasePage(Page_Address) == FLASH_COMPLETE)
    {
      return 0;
    }
  }
  return -1;
}

void FLASH_CFG_Write(uint32_t Page_Address, uint32_t* Data ,uint16_t DataLength)
{
  uint32_t i = 0;
  uint32_t FlashAddress;

  if ((Page_Address >= USER_CFG_FIRST_PAGE_ADDRESS)
    && (Page_Address <= USER_CFG_LAST_PAGE_ADDRESS))
  {
    FlashAddress = Page_Address;

    for (i = 0; i < DataLength; i++)
    {
      if (FlashAddress <= (USER_CFG_END_ADDRESS-4))
      {
        if (FLASH_ProgramWord(FlashAddress, *(uint32_t*)(Data + i)) == FLASH_COMPLETE)
        {
          FlashAddress += 4;
        }
        else
        {
          return;
        }
      }
      else
      {
        return;
      }
    }
  }
}


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
