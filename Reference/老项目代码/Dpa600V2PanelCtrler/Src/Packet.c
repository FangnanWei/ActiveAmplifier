#include "Packet.h"
#include "Fifo.h"
#include "stm8s.h"
#include "stdlib.h"
#include "Lcd.h"
#include "LedLightBar.h"
#include "GlobalVal.h"
#include "Key.h"
#include "AlgCfg.h"

Packet g_Txpacket;
//Packet *TxPacket;

extern void PannelPower(bool isOn);
void PacketInit() {
	//TxPacket = &g_Txpacket;
	FifoInit();
}

char cmd, prmtCmd;
char Value[MaxPacketValSize];

void CharstoShort(uint16_t *data16, uint8_t *data8) {
	(*data16) = (*data8);
	(*data16) = (*data16) << (uint16_t)8;
	(*data16) |= *(data8 + 1);
}

void PacketRx() {
	char head, i;
	static char valLen;
	char channel;
	uint16_t *tmp;
	int availableBytes;
	FilterType filterType;
	static bool lastReadHeadOnly_ = FALSE;
	if (!lastReadHeadOnly_ && FifoAvailableBytes(&g_UartFifo) < 3)
		return;
	//FifoRead(&g_UartFifo, &head, 1);
	if (lastReadHeadOnly_ || (FifoRead(&g_UartFifo, &head, 1) && head == PacketHead)) {
		if (!lastReadHeadOnly_) {
			FifoRead(&g_UartFifo, Value, 2);
			cmd = Value[0];
		}
		valLen = Value[1];
        if (valLen > 5)
          return;
		availableBytes = FifoAvailableBytes(&g_UartFifo);
		//if (availableBytes < 0 || availableBytes > 512)
			//while(1);
			//head = 55;
		if (availableBytes < valLen) {
			lastReadHeadOnly_ = TRUE;
			return;
		}
		lastReadHeadOnly_ = FALSE;
		FifoRead(&g_UartFifo, Value, valLen);
		//return;
		channel = Value[0];
		switch (cmd) {
		case CmdList_PowerOn:
			//开关机
			IsRunning = (bool)Value[0];
			PannelPower(IsRunning);
			break;
		case CmdList_Level:
			//电平显示
			LedLevelDisplay(channel, (signed char)Value[1]);
			break;
		case CmdList_Mute:
			//静音显示
			IsChMute[channel] = (bool)Value[1];
			if (IsHomePage) {
				if (IsChMute[channel])
					LcdMuteDisplay(channel);
				else
					LcdSelectChannel(channel, IsChSelect[channel]);
			}
			break;
		case CmdList_OutGain:
			//音量(Gain)显示
			//LcdGainValDisplay(channel, Value[0]);
			OutGainVals[channel] = (signed char)Value[1];
            if (OutGainVals[channel] > 0)
                OutGainVals[channel] = 0;
            if (OutGainVals[channel] < -99)
                OutGainVals[channel] = -99;
			break;

		case CmdList_MaxOutputVolt:
			MaxOutCurtVal[channel] = Value[1];
			break;
		case CmdList_MaxOutputCurt:
			MaxOutVoltVal[channel] = Value[1];
			break;
		case CmdList_MaxMainsCurt:
			MaxMainsCurtVal = Value[0];
			break;
		case CmdList_OutMode:
			OutGroupIdx = Value[0];
			OutMode[OutGroupIdx] = Value[1];
			break;
		case CmdList_InputSelect:
			InputSelectId = Value[0];
			break;
		case CmdList_InputGainSens:
			InGainVal[channel] = Value[1];
			break;
		case CmdList_LpFilter:
		case CmdList_HpFilter:
			prmtCmd = Value[1];
			filterType = (uint8_t)((cmd == CmdList_LpFilter) ? FilterType_Lp : FilterType_Hp);
			switch (prmtCmd) {
			case FilterCmdList_Active:
				ChSettingVal[channel].filterCfg[filterType].active = Value[2];
				break;
			case FilterCmdList_FreqHz:
				CharstoShort(&(ChSettingVal[channel].filterCfg[filterType].freqHz), Value + 2);
				break;
			case FilterCmdList_Butterwooth:
				ChSettingVal[channel].filterCfg[filterType].butterwoothIdx = Value[2];
				break;
			case FilterCmdList_Slope:
				ChSettingVal[channel].filterCfg[filterType].slope = Value[2];
				break;
			}
			break;
		case CmdList_PeakLImiter:
			prmtCmd = Value[1];
			switch (prmtCmd) {
			case PeakLimiterCmdList_Active:
				ChSettingVal[channel].peakLimiterCfg.active = Value[2];
				break;
			case PeakLimiterCmdList_ThreshVp:
				ChSettingVal[channel].peakLimiterCfg.threshVp = Value[2];
				break;
			case PeakLimiterCmdList_AttackMs:
				ChSettingVal[channel].peakLimiterCfg.attackMs= Value[2];
				//CharstoShort(&(ChSettingVal[channel].peakLimiterCfg.attackMs), Value + 2);
				break;
			case PeakLimiterCmdList_ReleaMs:
				//ChSettingVal[channel].peakLimiterCfg.releaMs = Value[2] | (Value[3] << 8);
				CharstoShort(&(ChSettingVal[channel].peakLimiterCfg.releaMs), Value + 2);
				break;
			}
			break;
		case CmdList_Polarity:
			ChSettingVal[channel].polarity = (bool)Value[1];
			break;
		case CmdList_ChDelay:
			//ChSettingVal[channel].chDelay = (uint16_t)Value[1] | ((uint16_t)Value[2] << 8);
			CharstoShort(&(ChSettingVal[channel].chDelay), Value + 1);
			break;
		case CmdList_MainsMeters:
			//CapMainsMetersV = Value[0] | (Value[1] << 8);
			CharstoShort(&(CapMainsMetersV), Value);
			//CapMainsMetersA = Value[2] | (Value[3] << 8);
			CharstoShort(&(CapMainsMetersA), Value + 2);
			break;
		case CmdList_ChOutput:
			//CapOutMetersV[channel] = Value[1] | (Value[2] << 8);
			CharstoShort(&(CapOutMetersV[channel]), Value + 1);
			//CapOutMetersA[channel] = Value[3] | (Value[4] << 8);
			CharstoShort(&(CapOutMetersA[channel]), Value + 3);
			break;
		case CmdList_Temperature:
			CapTemperatureC = Value[0];
			break;
		case CmdList_Version:
			Version[0] = Value[0];
			Version[1] = Value[1];
			break;
		case CmdList_SetIpAddr:
			for (i = 0; i < 4; i++)
				SetIpCfgVal.ipAddr[i] = Value[i];
			break;
		case CmdList_SetSubnetMask:
			for (i = 0; i < 4; i++)
				SetIpCfgVal.subnetMask[i] = Value[i];
			break;
		case CmdList_SetGateway:
			for (i = 0; i < 4; i++)
				SetIpCfgVal.gateway[i] = Value[i];
			break;
		default:
            head = 55;
			break;
		}
	}
}

void PacketTx(char cmd, char *value, char valLen) {
	char i;
	char dataLen;
	if (!IsLeafSel) {
		g_Txpacket.head = PacketHead;
		g_Txpacket.cmd = cmd;
		for (i = 0; i < valLen; i++) {
			g_Txpacket.value[i] = value[i];
		}
		g_Txpacket.valueLength = valLen;
		dataLen = sizeof(g_Txpacket.head) + sizeof(g_Txpacket.cmd) + sizeof(g_Txpacket.valueLength) + valLen;
		for (i = 0; i < dataLen; i++) {
			UART2_SendData8(*(((uint8_t *)&g_Txpacket) + i));
		}
	}
}

void PacketTxSubCmd(CmdList cmd, char subCmd, char *value, char valLen) {
	char pktVal[3];
	pktVal[0] = subCmd;
	pktVal[1] = *value;
	if (valLen == 2) {
		pktVal[2] = *(value + 1);
	}
	PacketTx(cmd, pktVal, valLen + 1);
}

