#ifndef packet_h_
#define packet_h_

#include <stdint.h>
#include "McuMsg.h"

int SendUpgradeAck(uint16_t reqId);
int SendPacket(uint8_t *buf, uint16_t len, DI_RET_e code, McuMsgType_e msgType, uint16_t reqId);

#endif

