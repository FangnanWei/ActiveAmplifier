#include "packet.h"
#include "DI_TypesPublic.h"


McuMsg_t g_msgRet;
int SendUpgradeAck(uint16_t reqId)
{
	memset(&g_msgRet, 0, sizeof(g_msgRet));
	MakeMcuMsg(&g_msgRet, NULL, 0, DI_Ok, MSG_UPGRADE_Ack, reqId);
	Usart1SendData((uint8_t*)&g_msgRet, g_msgRet.header.len + sizeof(McuMsgHeader_t));
}

int SendPacket(uint8_t *buf, uint16_t len, DI_RET_e code, McuMsgType_e msgType, uint16_t reqId)
{
	memset(&g_msgRet, 0, sizeof(g_msgRet));
	MakeMcuMsg(&g_msgRet, buf, len, code, msgType, reqId);
	Usart1SendData((uint8_t*)&g_msgRet, g_msgRet.header.len + sizeof(McuMsgHeader_t));
}
