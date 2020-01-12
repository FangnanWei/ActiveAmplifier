/****************************************
                                iap.h
                                zhangtaolei     2015-10-22
*****************************************/
#ifndef _IAP_H_
#define _IAP_H_

/****************************************
                                include
*****************************************/
#include "McuMsg.h"
#include "DI_TypesPublic.h"
/****************************************
                                macro
*****************************************/
#define UPGRADE_PACKET_SIZE     256
#define UPGRADE_TIMEOUT        3000

/*
        a : begin address to write 
        b : point to write buffer
        c : length
*/
#define WriteUserCode(a,b,c)    FLASH_If_Write(a,b,c)

#define PACKET_SIZE     (4 + UPGRADE_PACKET_SIZE + 1)

#define CLI()      __set_PRIMASK(1)			  
#define SEI()      __set_PRIMASK(0) 

/****************************************
                                typedef
*****************************************/
typedef enum tagUpgradeState_e
{
    UPGRADE_Ok,
    UPGRADE_Timeout,
    UPGRADE_ChecksumErr,
    UPGRADE_MagicErr,

    UPGRADE_Ready,
    UPGRADE_RecvData,
    
}UpgradeState_e;

/****************************************
                                variables
*****************************************/

/****************************************
                                functions
*****************************************/
void EraseUserCode(void);
void EraseDSP_Topo(void);
int BeginUpgrade(ComFifo_t *pComFifo);
int DSP_TopoUpgrade(ComFifo_t * pComFifo);

#endif
