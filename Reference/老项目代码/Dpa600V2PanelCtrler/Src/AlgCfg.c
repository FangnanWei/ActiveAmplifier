#include "AlgCfg.h"
#include "Menu.h"
#include "Key.h"
#include "Encoder.h"
#include "Packet.h"
#include <string.h>

char MaxOutputVoltStr[][MAX_SHOW_LENGTH] = {
	{"CH1 010Vpeak"},
	{"CH2 010Vpeak"},
	{"CH3 010Vpeak"},
	{"CH4 010Vpeak"},
};

char MaxOutputCurtStr[][MAX_SHOW_LENGTH] = {
	{"CH1 001Apeak"},
	{"CH2 001Apeak"},
	{"CH3 001Apeak"},
	{"CH4 001Apeak"},
};

char MaxMainsCurtStr[MAX_SHOW_LENGTH] = {"001Apeak"};

const char InputSelStr[][MAX_SHOW_LENGTH] = {
	{"XLR=>Out"},
	{"XLR=>DSP=>Out"},
	{"AES=>Out"},
	{"AES=>DSP=>Out"},
	{"DTE=>Out"},
	{"DTE=>DSP=>Out"}
};

#if 0
const char PeakingStr[][MAX_SHOW_LENGTH] = {
	{"Peaking"},
	{"Low Shelving"},
	{"High Shelving"},
	{"Low pass EQ"},
	{"High pass EQ"},
	{"Bandstop"},
	{"Bandpass"},
	{"Allpass"}
};
#endif

char FilterFreqStr[MAX_SHOW_LENGTH] = {"Freq.Hz:12000"};

char ActiveStr[MAX_SHOW_LENGTH] = {"Active: OFF"};

char FilterSlopeStr[MAX_SHOW_LENGTH] = {"Slope(db): 06"};

const char FilterButterwoothStr[][MAX_SHOW_LENGTH] = {
	{"Butterwooth"},
	{"Bessel"},
	{"Link.riley"},
	//{"FIR Lin Phase"},
	//{"Hybrid FIR"}
};

const char PolarityInPhaseStr[][MAX_SHOW_LENGTH] = {
	{"In phase"},
	{"Reversed"}
};

char PeakLimiterAttackMsStr[MAX_SHOW_LENGTH] = {"Attack(mS):000"};
char PeakLimiterReleaMsStr[MAX_SHOW_LENGTH] = {"Relea.(mS):000"};

int16_t ValChangeByEncoder(int16_t value, int16_t max, int16_t min, char variable) {
	if (IsLeafSel) {
		if (IsEncoderTurnValid(TURN_Dir_L)) {
			if (value > min)
				value -= variable;
		}
		else if(IsEncoderTurnValid(TURN_Dir_R)) {
			if (value < max) {
				value += variable;
			}
		}
	}
	return value;
}

void NumInttoChar(char *itemStr, int16_t value, char unitDigitPos, char digitNum) {
	int8_t i;
	int16_t num;
	num = value;
	if (num < 0) {
		num = num * (-1);
	}

	for (i = unitDigitPos; digitNum > 0 && i >= 0; i--, digitNum--) {
		itemStr[i] = num % 10 + '0';
		num /= 10;
	}
	if (itemStr[i] == ' ')
		itemStr[i] = value < 0 ? '-' : ' ';
}

int16_t RangeAdjust(char *itemStr, int16_t value, int16_t max, int16_t min, char unitDigitPos, char digitNum, char variable) {
	value = ValChangeByEncoder(value, max, min, variable);
	NumInttoChar(itemStr, value, unitDigitPos, digitNum);
	return value;
}

void SelectSet(Item *item, uint8_t *selectId, const char (*selStr)[MAX_SHOW_LENGTH], uint8_t maxItemNum) {
	*selectId = ValChangeByEncoder((uint16_t)(*selectId), maxItemNum - 1, 0, 1);
	item->data8 = (char *)selStr[*selectId];
}

//MaxOutputVolt
char MaxOutVoltItemIdStart = 0;
void MaxOutputVolt(uint8_t nodeId) {
	MaxOutVoltItemIdStart = GetSubListFirstItemId(nodeId);
}

void MaxCurtOrVoltSet(Item *item, uint8_t itemIdStart, char *value, char (*str)[MAX_SHOW_LENGTH], int8_t max, int8_t min) {
	char channel = item->nodeId - itemIdStart;
	value[channel] = RangeAdjust(str[channel], value[channel], max, min, 6, 2, 1);
	//value[channel] = ValChangeByEncoder(value[channel], max, min, 1);
	//str[channel][6] = value[channel] % 10 + '0';
	//str[channel][5] = value[channel] / 10  + '0';
	item->data8 = str[channel];
}

char MaxOutVoltVal[4] = {10, 10, 10, 10};
void MaxOutputVoltSet(Item *item) {
	MaxCurtOrVoltSet(item, MaxOutVoltItemIdStart, MaxOutVoltVal, MaxOutputVoltStr, 70, 10);
}

void MaxOutVoltPacketSend(uint8_t nodeId) {
	char channel = nodeId - MaxOutVoltItemIdStart;
	PacketTxSubCmd(CmdList_MaxOutputVolt, channel, &(MaxOutVoltVal[channel]), 1);
}

//MaxOutputCurt
char MaxOutCurtItemIdStart = 0;
void MaxOutputCurt(uint8_t nodeId) {
	MaxOutCurtItemIdStart = GetSubListFirstItemId(nodeId);
}

char MaxOutCurtVal[4] = {1, 1, 1, 1};
void MaxOutputCurtSet(Item *item) {
	MaxCurtOrVoltSet(item, MaxOutCurtItemIdStart, MaxOutCurtVal, MaxOutputCurtStr, 35, 1);
}

void MaxOutCurtPacketSend(uint8_t nodeId) {
	char channel = nodeId - MaxOutCurtItemIdStart;
	PacketTxSubCmd(CmdList_MaxOutputCurt, channel, &(MaxOutCurtVal[channel]), 1);
}

//MaxMainsCurt
char MaxMainsCurtItemIdStart = 0;
void MaxMainsCurt(uint8_t nodeId) {
	MaxMainsCurtItemIdStart = GetSubListFirstItemId(nodeId);
}

char MaxMainsCurtVal = 1;
void MaxMainsCurtSet(Item *item) {
	//MaxCurtOrVoltSet(item, MaxMainsCurtItemIdStart, MaxMainsCurtVal, MaxMainsCurtStr, 35, 1);
	MaxMainsCurtVal = RangeAdjust(MaxMainsCurtStr, (int16_t)MaxMainsCurtVal, 35, 1, 2, 2, 1);
	item->data8 = MaxMainsCurtStr;
}

void MaxMainsCurtPacketSend(uint8_t nodeId) {
	char channel = nodeId - MaxMainsCurtItemIdStart;
	PacketTxSubCmd(CmdList_MaxMainsCurt, channel, &MaxMainsCurtVal, 1);
}

const char OutModeStr[][MAX_SHOW_LENGTH] = {
	{"Stereo"},
	{"Parallel"},
	{"Bridge"}
};

char OutGroupIdx = 0;
void OutGroupIdx0Get(uint8_t nodeId) {
	OutGroupIdx = 0;
}

void OutGroupIdx1Get(uint8_t nodeId) {
	OutGroupIdx = 1;
}

char OutMode[2] = {0, 0};
void OutModeSel(Item *item) {
	SelectSet(item, &OutMode[OutGroupIdx], OutModeStr, sizeof(OutModeStr) / MAX_SHOW_LENGTH);
}

void OutModePacketSend(uint8_t nodeId) {
	//PacketTx(CmdList_OutMode, &OutMode, 1);
	PacketTxSubCmd(CmdList_OutMode, OutGroupIdx, &OutMode[OutGroupIdx], 1);
}

void FixedPointDecAdd(char *str, char topDigitPos, char decPointPos) {
	char i;
	//if (topDigitPos < 0)
	//	return;
	for (i = topDigitPos; i < decPointPos; i++) {
		str[i] = str[i + 1];
	}
	str[i] = '.';
}

uint8_t InputSelectId = 0;
void InputSelectSet(Item *item) {
	SelectSet(item, &InputSelectId, InputSelStr, sizeof(InputSelStr) / MAX_SHOW_LENGTH);
}

void InSelSetPacketSend(uint8_t nodeId) {
	PacketTx(CmdList_InputSelect, &InputSelectId, 1);
}

uint8_t InGainChItemIdStart;
void InputGainSens(uint8_t nodeId) {
	InGainChItemIdStart = GetSubListFirstItemId(nodeId);
}

char InGainVal[4] = {24, 24, 24, 24};
//char InputGainChId;
char InputGainSensStr[][MAX_SHOW_LENGTH] = {
	{"1 36dB/00dBu"},
	{"2 36dB/00dBFS"},
	{"3 36dB/00dBu"},
	{"4 36dB/00dBFS"}
};

void InputGainSensSet(Item *item) {
	char channel = item->nodeId - InGainChItemIdStart;
	char *inputGainSensStr = InputGainSensStr[channel];
	signed char convertNum = 36;
	char addDigitNum = (InputSelectId > 1);
//	if (InGainVal[1] == 28) {
	InGainVal[channel] = ValChangeByEncoder(InGainVal[channel], 36, 24, 1);
	if (addDigitNum) {
		convertNum = 16;
		inputGainSensStr[7] = ' ';
	}
	NumInttoChar(inputGainSensStr, InGainVal[channel], 3, 2);
	NumInttoChar(inputGainSensStr, convertNum - InGainVal[channel], 8 + addDigitNum , 2);
	inputGainSensStr[6] = '/';
	inputGainSensStr[9 + addDigitNum] = 'd';
	inputGainSensStr[10 + addDigitNum] = 'B';
	if (!addDigitNum) {
		inputGainSensStr[11] = 'u';
		inputGainSensStr[12] = '\0';
	} else {
		inputGainSensStr[12] = 'F';
		inputGainSensStr[13] = 'S';
	}
	item->data8 = inputGainSensStr;
}

void InGainPacketSend(uint8_t nodeId) {
	char channel = nodeId - InGainChItemIdStart;
	PacketTxSubCmd(CmdList_InputGainSens, channel, &(InGainVal[channel]), 1);
}

//Dsp Setting
char DspSetChIdStart;

void DspChItemIdStartGet(uint8_t nodeId) {
	DspSetChIdStart = GetSubListFirstItemId(nodeId) + 2;
}

char ChannelId;
void DspChSet(uint8_t nodeId) {
	ChannelId = nodeId - DspSetChIdStart;
}

void ActiveStrChange(Item *item, bool *activeFlag) {
	if (IsLeafSel) {
		if (IsEncoderTurnValid(TURN_Dir_L) || IsEncoderTurnValid(TURN_Dir_R))
			(*activeFlag) = !(*activeFlag);
	}
	ActiveStr[8] = 'O';
	if (*activeFlag) {
		ActiveStr[9] = 'N';
		ActiveStr[10] = ' ';
	}
	else {
		ActiveStr[9] = 'F';
		ActiveStr[10] = 'F';
	}
	item->data8 = ActiveStr;
}

ChSetting ChSettingVal[4];

const ChSetting ChSettingDefaultVal = {
	{
		{
			{FALSE},
			{12000},
			{6},
			{0}
		},
		{
			{FALSE},
			{12000},
			{6},
			{0}
		}
	},
	{FALSE},
	{
		{0},
		{100},
		{FALSE},
		{20}
	},
	{000}
};

#if 0
FilterCfg FilterCfgVal = {
	{FALSE},
	{"12000"},
	{0},
	{0}
};
void ChSetPacketTx(CmdList cmd, char *value, char valLen) {
	char payload[3];
	payload[0] = ChannelId;
	payload[1] = value[0];
	if (valLen == 2)
		payload[2] = value[1];
	PacketTx(cmd, payload, valLen + 1);
}
#endif
void ChSetSubCmdPacketTx(CmdList cmd, char prmtCmd, char *value, char valLen) {
	char payload[4];
	payload[0] = ChannelId;
	payload[1] = prmtCmd;
	payload[2] = value[0];
	if (valLen == 2)
		payload[3] = value[1];
	PacketTx(cmd, payload, valLen + 2);
}

//char *ReleaMsDefault = {"100"};
void AlgCfgInit() {
	uint8_t i;
	for (i = 0; i < 4; i++) {
		InGainVal[i] = 24;
		memcpy((void *)&(ChSettingVal[i]), (const void *)&ChSettingDefaultVal, sizeof(ChSetting));
		SetIpCfgVal.ipAddr[i] = 0;
	}
	memset((void *)&SetIpCfgVal, 0, sizeof(SetIpCfg));
}

uint8_t FilterTypeId;

void FilterPacketTx(char prmtCmd, char *value, char valLen) {
	ChSetSubCmdPacketTx(FilterTypeId == ((uint8_t)FilterType_Hp) ? CmdList_HpFilter : CmdList_LpFilter, prmtCmd, value, valLen);
}

void FilterTypeLpSet(uint8_t nodeId) {
	FilterTypeId = (uint8_t)FilterType_Lp;
}

void FilterTypeHpSet(uint8_t nodeId) {
	FilterTypeId = (uint8_t)FilterType_Hp;
}

//bool *_active;
void FilterActive(Item *item) {
	//_active = &(ChSettingVal[ChannelId].filterCfg[FilterTypeId].active);
	ActiveStrChange(item, &(ChSettingVal[ChannelId].filterCfg[FilterTypeId].active));
}

void FilterActivePacket(uint8_t nodeId) {
	FilterPacketTx(FilterCmdList_Active, &(ChSettingVal[ChannelId].filterCfg[FilterTypeId].active), 1);
}

int8_t SelectCharPos = -1;
bool IsCharSelected = FALSE;
int8_t Offset = -1;
char NumChar[2] = {"0"};

void NumCharSingleAdjust(char *itemStr, char max, char topDigitPos) {
//	char i;
    if (IsCharSelected) {
        itemStr[Offset + topDigitPos] = ValChangeByEncoder(itemStr[Offset + topDigitPos], max + '0', '0', 1);
        NumChar[0] = itemStr[Offset + topDigitPos];
    }
	//for (i = 0; i < digitNum; i++) {
	//	itemStr[topDigitPos + i] = numChar[i];
	//}

}

void NumCharPosMove(char *itemStr, char digitNum, char topDigitPos) {
	if (Offset < digitNum - 1) {
		IsCharSelected = TRUE;
		Offset++;
		SelectCharPos = topDigitPos + Offset;
		NumChar[0] = itemStr[SelectCharPos];
	}
	else {
		Offset = -1;
		SelectCharPos = -1;
		IsCharSelected = FALSE;
		IsLeafSel = FALSE;
	}
}

uint16_t StrtoNum( const char *numChar, char digitNum, char topDigitPos) {
	uint16_t value = 0;
	char i;
	for (i = topDigitPos; i < digitNum + topDigitPos; i++) {
		value *= 10;
		value += numChar[i] - '0';
	}
	return value;
}

void NumtoStr(char *numChar, uint16_t value, char digitNum, char topDigitPos) {
	char i = 0, j, temp;
	for (; i < digitNum; i++) {
		numChar[i + topDigitPos] = value % 10 + '0';
		value /= 10;
	}
	//逆序
	i--;
	for(j = 0; j <= i / 2; j++)
	{
		temp = numChar[j + topDigitPos];
		numChar[j + topDigitPos] = numChar[i - j + topDigitPos];
		numChar[i - j + topDigitPos] = temp;
	}
}

//char _freqHzNumChar[6];
//uint16_t *_filterFreq;
void FilterFreqHz(Item *item) {
	char max;
	//_filterFreq = &(ChSettingVal[ChannelId].filterCfg[FilterTypeId].freqHz);
	NumtoStr(FilterFreqStr, ChSettingVal[ChannelId].filterCfg[FilterTypeId].freqHz, 5, 8);
	if (Offset == 0)
		max = 2;
	else
		max = 9;
	NumCharSingleAdjust(FilterFreqStr, max, 8);
	ChSettingVal[ChannelId].filterCfg[FilterTypeId].freqHz = StrtoNum(FilterFreqStr, 5, 8);
	item->data8 = FilterFreqStr;
}

void FilterFreqEncKeyFunc(uint8_t nodeId) {
	//uint16_t freq = 0;
	if (IsLeafSel) {
		NumCharPosMove(FilterFreqStr, 5, 8);
	}
	if (!IsLeafSel) {
		FilterPacketTx(FilterCmdList_FreqHz, (char *)&(ChSettingVal[ChannelId].filterCfg[FilterTypeId].freqHz), 2);
	}
}

//uint8_t *_filterSlop = NULL;
void FilterSlope(Item *item) {
	//_filterSlop = &(ChSettingVal[ChannelId].filterCfg[FilterTypeId].slope);
	char variable;
	uint16_t min;
	item->data8 = FilterSlopeStr;
	if (ChSettingVal[ChannelId].filterCfg[FilterTypeId].butterwoothIdx == 2) {
		variable = 12;
		min = 12;
	}
	else {
		variable = 6;
		min = 6;
	}
	ChSettingVal[ChannelId].filterCfg[FilterTypeId].slope = (uint8_t)RangeAdjust(item->data8, (int16_t)ChSettingVal[ChannelId].filterCfg[FilterTypeId].slope, 48, min, 12, 2, variable);
}

void FilterSlopePacketTx(uint8_t nodeId) {
	FilterPacketTx(FilterCmdList_Slope, &(ChSettingVal[ChannelId].filterCfg[FilterTypeId].slope), 1);
}

//char *_filterButterwoothIdx;
void FilterButterwooth(Item *item) {
	char *_filterButterwoothIdx = &(ChSettingVal[ChannelId].filterCfg[FilterTypeId].butterwoothIdx);
	//SelectSet(item, _filterButterwoothIdx, FilterButterwoothStr, sizeof(FilterButterwoothStr) / MAX_SHOW_LENGTH);
	if (IsLeafSel) {
		if (IsEncoderTurnValid(TURN_Dir_L)) {
			if ((*_filterButterwoothIdx) > 0)
				(*_filterButterwoothIdx) -= 1;
			if ((*_filterButterwoothIdx) == 2)
				ChSettingVal[ChannelId].filterCfg[FilterTypeId].slope = 12;
			else
				ChSettingVal[ChannelId].filterCfg[FilterTypeId].slope = 6;
		}
		else if(IsEncoderTurnValid(TURN_Dir_R)) {
			if ((*_filterButterwoothIdx) < (sizeof(FilterButterwoothStr) / MAX_SHOW_LENGTH - 1)) {
				(*_filterButterwoothIdx) += 1;
			}
			if ((*_filterButterwoothIdx) == 2)
				ChSettingVal[ChannelId].filterCfg[FilterTypeId].slope = 12;
			else
				ChSettingVal[ChannelId].filterCfg[FilterTypeId].slope = 6;
		}
	}
	item->data8 = (char *)FilterButterwoothStr[*_filterButterwoothIdx];
}

void FilterButterwoothPacketTx(uint8_t nodeId) {
	FilterPacketTx(FilterCmdList_Butterwooth, &(ChSettingVal[ChannelId].filterCfg[FilterTypeId].butterwoothIdx), 1);
	FilterSlopePacketTx(0);
}

//bool *_polarity;
void PolarityInPhaseSel(Item *item) {
	//_polarity = &(ChSettingVal[ChannelId].polarity);
	SelectSet(item, (uint8_t*)&(ChSettingVal[ChannelId].polarity), PolarityInPhaseStr, sizeof(PolarityInPhaseStr) / MAX_SHOW_LENGTH);
}

void PolarityInPhasePacketTx(uint8_t nodeId) {
	PacketTxSubCmd(CmdList_Polarity, ChannelId, &(ChSettingVal[ChannelId].polarity), 1);
}

#if 1
//bool *peakLimitActive;
void PeakLimiterActiveSel(Item *item) {
	//peakLimitActive = &(ChSettingVal[ChannelId].peakLimiterCfg.active);
	ActiveStrChange(item, &(ChSettingVal[ChannelId].peakLimiterCfg.active));
}

void PeakLimiterActivePacketTx(uint8_t nodeId) {
	ChSetSubCmdPacketTx(CmdList_PeakLImiter, PeakLimiterCmdList_Active, &(ChSettingVal[ChannelId].peakLimiterCfg.active), 1);
}

//uint8_t *_threshVp;
char PeakLimiterThreshVpStr[MAX_SHOW_LENGTH] = {"Thresh.Vp:000"};
void PeakLimiterThreshVp(Item *item) {
	//_threshVp = &(ChSettingVal[ChannelId].peakLimiterCfg.threshVp);
	item->data8 = PeakLimiterThreshVpStr;
	ChSettingVal[ChannelId].peakLimiterCfg.threshVp = RangeAdjust(item->data8, (int16_t)ChSettingVal[ChannelId].peakLimiterCfg.threshVp, 70, 0, 12, 3, 1);
}

void PeakLimiterThreshVpPacketTx(uint8_t nodeId) {
	ChSetSubCmdPacketTx(CmdList_PeakLImiter, PeakLimiterCmdList_ThreshVp, (char *)&(ChSettingVal[ChannelId].peakLimiterCfg.threshVp), 1);
}

//uint8_t *_attackMs;
void PeakLimiterAttack(Item *item) {
	//_attackMs = &(ChSettingVal[ChannelId].peakLimiterCfg.attackMs);
	item->data8 = PeakLimiterAttackMsStr;
	ChSettingVal[ChannelId].peakLimiterCfg.attackMs = RangeAdjust(item->data8, (int16_t)ChSettingVal[ChannelId].peakLimiterCfg.attackMs, 200, 5, 13, 3, 1);
}

void PeakLimiterAttackPacketTx(uint8_t nodeId) {
	ChSetSubCmdPacketTx(CmdList_PeakLImiter, PeakLimiterCmdList_AttackMs, (char *)&(ChSettingVal[ChannelId].peakLimiterCfg.attackMs), 1);
}

//char _releaMsNumStr[4];
//uint16_t *_releaMs;
void PeakLimiterReleaMs(Item *item) {
	//_releaMs = &(ChSettingVal[ChannelId].peakLimiterCfg.releaMs);

	NumtoStr(PeakLimiterReleaMsStr, ChSettingVal[ChannelId].peakLimiterCfg.releaMs, 3, 11);
	NumCharSingleAdjust(PeakLimiterReleaMsStr, 9, 11);
	ChSettingVal[ChannelId].peakLimiterCfg.releaMs = StrtoNum(PeakLimiterReleaMsStr, 3, 11);
	item->data8 = PeakLimiterReleaMsStr;
}

void PeakLimiterReleaMsPacketTx(uint8_t nodeId) {
//	uint16_t releaVal = 0;
	if (IsLeafSel) {
		NumCharPosMove(PeakLimiterReleaMsStr, 3, 11);
	}
	if (!IsLeafSel) {
		ChSetSubCmdPacketTx(CmdList_PeakLImiter, PeakLimiterCmdList_ReleaMs,
			(char *)&(ChSettingVal[ChannelId].peakLimiterCfg.releaMs), 2);
	}
}
#endif

char ChDelayStr[MAX_SHOW_LENGTH] = {"000 mS"};
//char _chDelayNumChar[4];
uint16_t *_chDelay;
void ChDelay(Item *item) {
	_chDelay = &(ChSettingVal[ChannelId].chDelay);
	NumtoStr(ChDelayStr, *_chDelay, 3, 0);
	NumCharSingleAdjust(ChDelayStr, 9, 0);
	*_chDelay = StrtoNum(ChDelayStr, 3, 0);
	item->data8 = ChDelayStr;
	//*_chDelayNumChar = RangeAdjust(item->data8, (int16_t)*_chDelayNumChar, 999, 0, 1, 2, 1);
}

void ChDelayPacketTx(uint8_t nodeId) {
//	uint16_t chDelayVal = 0;
	if (IsLeafSel) {
		NumCharPosMove(ChDelayStr, 3, 0);
	}
	if (!IsLeafSel) {
		PacketTxSubCmd(CmdList_ChDelay, ChannelId, (char *)_chDelay, 2);
	}
}

void BigCharDisplaySet(uint8_t nodeId) {
	BigCharDisplay = TRUE;
}

bool BigCharDisplay = FALSE;
char *CapPrmtStr = NULL;

void DisplayCapMeters(uint16_t valueV, uint16_t valueA) {
	NumtoStr(CapPrmtStr, valueV, 3, 0);
	NumtoStr(CapPrmtStr, valueA, 3, 6);
	FixedPointDecAdd(CapPrmtStr, 5, 7);
	BigCharDisplay = TRUE;
}

char DisplayMainsMetersStr[] = {"220V  095A"};
uint16_t CapMainsMetersV = 220;
uint16_t CapMainsMetersA = 100;
void DisplayMainsMeters(Item *item) {
	CapPrmtStr = DisplayMainsMetersStr;
	DisplayCapMeters(CapMainsMetersV, CapMainsMetersA);
	PageNoAdjust = TRUE;
}

char DisplayOutMetersStr[] = {"070V  057A"};
//char ChOutputStr[MAX_SHOW_LENGTH] = {"CH  Output"};
const char ChOutputStr[][MAX_SHOW_LENGTH] = {
	{"CH1  Output"},
	{"CH2  Output"},
	{"CH3  Output"},
	{"CH4  Output"}
};
uint16_t CapOutMetersV[4] = {70, 70, 70, 70};
uint16_t CapOutMetersA[4] = {60, 60, 60, 60};
void DispalyOutputMeters(Item *item) {
	static char channel = 0;
	channel = ValChangeByEncoder(channel, 3, 0, 1);
	//NumtoStr(ChOutputStr, channel + 1, 1, 2);
	item->data8 = (char *)ChOutputStr[channel];
	CapPrmtStr = DisplayOutMetersStr;
	DisplayCapMeters(CapOutMetersV[channel], CapOutMetersA[channel]);
	PageNoAdjust = FALSE;
}

char DisplayTemperatureStr[] = {"25oC 077oF"};
uint8_t CapTemperatureC = 25;
void DisplayTemperature(Item *item) {
	NumtoStr(DisplayTemperatureStr, CapTemperatureC, 2, 0);
	NumtoStr(DisplayTemperatureStr, (uint16_t)(CapTemperatureC * 1.8 + 32), 3, 5); //转化为华氏度显示
	CapPrmtStr = DisplayTemperatureStr;
	BigCharDisplay = TRUE;
	PageNoAdjust = TRUE;
}

char DisplayVersionStr[] = {" V2.03"};
uint8_t Version[2] = {1, 0};
void DisplayVersion(Item *item) {
	NumtoStr(DisplayVersionStr, Version[0], 1, 2);
	NumtoStr(DisplayVersionStr, Version[1], 2, 4);
	CapPrmtStr = DisplayVersionStr;
	BigCharDisplay = TRUE;
	PageNoAdjust = TRUE;
}

//IpCfg
void IpConfig(uint8_t nodeId) {
	Page2LineAdjust = TRUE;
}

SetIpCfg SetIpCfgVal;

char IpAddressStr[MAX_SHOW_LENGTH + 1] = {"192.168.000.010"};

void SetNetCfg(Item *item, char *displayStr, char *netCfg) {
	char i, max;
	for (i = 0; i < 4; i++) {
		NumtoStr(displayStr, netCfg[i], 3, i * 4);
	}
	if (Offset % 3 == 0) {
		max = 2;
	}
	else
		if (displayStr[Offset / 3 * 4] == '2')
			max = 5;
		else if (displayStr[Offset / 3 * 4] <= '1')
			max = 9;
	NumCharSingleAdjust(displayStr, max, Offset < 0 ? 0 : Offset / 3);
	for (i = 0; i < 4; i++) {
		netCfg[i] = StrtoNum(displayStr, 3, i * 4);
	}
	item->data8 = displayStr;

}

void SetNetCfgEncKeyPress(char *displayStr, char *netCfg) {
	if (IsLeafSel) {
		NumCharPosMove(displayStr, 12, (Offset + 1) / 3);
	}
	if (!IsLeafSel) {
		PacketTx(CmdList_SetIpAddr, netCfg, 4);
	}
}

void SetIpAddress(Item *item) {
	SetNetCfg(item, IpAddressStr, SetIpCfgVal.ipAddr);
#if 0
	char i, max;
	for (i = 0; i < 4; i++) {
		NumtoStr(IpAddressStr, SetIpCfgVal.ipAddr[i], 3, i * 4);
	}
	if (Offset % 3 == 0) {
		max = 2;
	}
	else
		max = 5;
	NumCharSingleAdjust(IpAddressStr, max, Offset < 0 ? 0 : Offset / 3);
	for (i = 0; i < 4; i++) {
		SetIpCfgVal.ipAddr[i] = StrtoNum(IpAddressStr, 3, i * 4);
	}
	item->data8 = IpAddressStr;
#endif
}

void SetIpAddrEncKeyPress(uint8_t nodeId) {
	SetNetCfgEncKeyPress(IpAddressStr, SetIpCfgVal.ipAddr);
#if 0
	if (IsLeafSel) {
		NumCharPosMove(IpAddressStr, 12, (Offset + 1) / 3);
	}
	if (!IsLeafSel) {
		PacketTx(CmdList_SetIpAddr, SetIpCfgVal.ipAddr, 4);
	}
#endif
}

char SubnetMaskStr[MAX_SHOW_LENGTH + 1] = {"255.255.255.000"};

void SetSubnetMask(Item *item) {
	SetNetCfg(item, SubnetMaskStr, SetIpCfgVal.subnetMask);
}

void SetSubnetMaskEncKeyPress(uint8_t nodeId) {
	SetNetCfgEncKeyPress(SubnetMaskStr, SetIpCfgVal.subnetMask);
}

char GatewayStr[MAX_SHOW_LENGTH + 1] = {"255.255.255.000"};

void SetGateway(Item *item) {
	SetNetCfg(item, GatewayStr, SetIpCfgVal.gateway);
}

void SetGatewayEncKeyPress(uint8_t nodeId) {
	SetNetCfgEncKeyPress(GatewayStr, SetIpCfgVal.gateway);
}

void ResetSetting(Item *item) {
	AlgCfgInit();
}

