#ifndef _Protocol_h_
#define _Protocol_h_

#define MaxPacketValSize	5
#define PacketHead			0x55

#if 1
typedef struct Packet_t {
	char head;
	char cmd;
	char valueLength;
	char value[MaxPacketValSize];
}Packet;
#endif

typedef enum {
	CmdList_PowerOn = 0x00,
	CmdList_Level = 0x01,
	CmdList_OutGain = 0x02,
	CmdList_Mute = 0x03,
	CmdList_InputSelect = 0x04,
	CmdList_InputGainSens = 0x05,
	CmdList_MaxOutputVolt = 0x06,
	CmdList_MaxOutputCurt = 0x07,
	CmdList_MaxMainsCurt = 0x08,
	CmdList_OutMode = 0x09,
	//CmdList_AesGain = 0x0A,

	CmdList_Peq = 0x0B,
	CmdList_LpFilter = 0x0C,
	CmdList_HpFilter = 0x0D,
	CmdList_Polarity = 0x0E,
	CmdList_PeakLImiter = 0x0F,
	//CmdList_ChGain = 0x10,
	CmdList_ChDelay = 0x11,
	CmdList_SetIpAddr = 0x12,
	CmdList_SetSubnetMask = 0x13,
	CmdList_SetGateway = 0x14,
	CmdList_MainsMeters = 0x15,
	CmdList_ChOutput = 0x16,
	CmdList_Temperature = 0x17,
	CmdList_Version = 0x18,
}CmdList;

#if 0
typedef enum {
	PeqCmdList_Segment = 0x01,
	PeqCmdList_Active = 0x02,
	PeqCmdList_Freq = 0x03,
	PeqCmdList_Gain = 0x04,
	PeqCmdList_Qfactor = 0x05,
	PeqCmdList_Peaking = 0x06,
} PeqCmdList;
#endif

typedef enum {
	FilterCmdList_Active = 0x01,
	FilterCmdList_FreqHz = 0x02,
	FilterCmdList_Slope = 0x03,
	FilterCmdList_Butterwooth = 0x04,
} FilterCmdList;

typedef enum {
	PeakLimiterCmdList_Active = 0x01,
	PeakLimiterCmdList_ThreshVp = 0x02,
	PeakLimiterCmdList_AttackMs = 0x03,
	PeakLimiterCmdList_ReleaMs = 0x04
} PeakLimiterCmdList;

void PacketInit();
void PacketRx();
void PacketTx(char cmd, char *value, char len);
void PacketTxSubCmd(char cmd, char subCmd, char *value, char valLen);
#endif

