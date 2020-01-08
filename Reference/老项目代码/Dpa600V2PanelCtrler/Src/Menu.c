#include "Menu.h"
#include <string.h>
#include <stdlib.h>
#include "AlgCfg.h"
#include "Led.h"
#include "Gpio.h"

const ItemPrmt ItemPrmtDpa600V2[] = {
	{NULL, NULL, 4, ""},

	//Level 1
	{NULL, NULL, 5, "AMP Settings"},
	{NULL, DspChItemIdStartGet, 7, "DSP Settings"},
	{NULL, NULL, 1, "NET Settings"},
	{NULL, NULL, 4, "Display"},

	//Level 2
	//AMP Settings
	{NULL, MaxOutputVolt, 4, "Max output volt"},
	{NULL, MaxOutputCurt, 4, "Max output curt"},
	{NULL, MaxMainsCurt, 1, "Max mains curt"},
	{NULL, OutGroupIdx0Get, 1, "Output mode 1"},
	{NULL, OutGroupIdx1Get, 1, "Output mode 2"},

	//DSP Settings
	{NULL, NULL, 1, "Input select"},
	{NULL, InputGainSens, 4, "Input gain/sens"},
	{NULL, DspChSet, 5, "CH1 Settings"},
	{NULL, DspChSet, 5, "CH2 Settings"},
	{NULL, DspChSet, 5, "CH3 Settings"},
	{NULL, DspChSet, 5, "CH4 Settings"},
	{NULL, NULL, 0, "Reset Setting"},

	//NET Settings
	{NULL, IpConfig, 6, "Set address"},

	//Display
	{NULL, BigCharDisplaySet, 1, "Mains meters"},
	{NULL, BigCharDisplaySet, 1, "Output meters"},
	{NULL, BigCharDisplaySet, 1, "Temperature"},
	{NULL, BigCharDisplaySet, 1, "Hardware info"},

	//level 3
	//Max output volt
	{MaxOutputVoltSet, MaxOutVoltPacketSend, 0, "CH1 070Vpeak"},
	{MaxOutputVoltSet, MaxOutVoltPacketSend, 0, "CH2 070Vpeak"},
	{MaxOutputVoltSet, MaxOutVoltPacketSend, 0, "CH3 070Vpeak"},
	{MaxOutputVoltSet, MaxOutVoltPacketSend, 0, "CH4 070Vpeak"},

	//Max output curt
	{MaxOutputCurtSet, MaxOutCurtPacketSend, 0, "CH1 001Apeak"},
	{MaxOutputCurtSet, MaxOutCurtPacketSend, 0, "CH2 001Apeak"},
	{MaxOutputCurtSet, MaxOutCurtPacketSend, 0, "CH3 001Apeak"},
	{MaxOutputCurtSet, MaxOutCurtPacketSend, 0, "CH4 001Apeak"},

	//Max mains curt
	{MaxMainsCurtSet, MaxMainsCurtPacketSend, 0, "010Apeak"},

	//OutMode 1
	{OutModeSel, OutModePacketSend, 0, "Stereo"},
	//OutMode 2
	{OutModeSel, OutModePacketSend, 0, "Stereo"},

	//Input select
	{InputSelectSet, InSelSetPacketSend, 0, "XLR=>Out"},

	//Input gain/sens
	{InputGainSensSet, InGainPacketSend, 0, "1 36dB/00dBu"},
	{InputGainSensSet, InGainPacketSend, 0, "2 36dB/00dBu"},
	{InputGainSensSet, InGainPacketSend, 0, "3 36dB/00dBu"},
	{InputGainSensSet, InGainPacketSend, 0, "4 36dB/00dBu"},

	//CH1 Settings
	//{NULL, NULL, 6, "PEQ (16)"},
	{NULL, FilterTypeLpSet, 4, "LP filter"},
	{NULL, FilterTypeHpSet, 4, "HP filter"},
	{NULL, NULL, 4, "Peak limiter"},
	{NULL, NULL, 1, "Polarity"},
	{NULL, NULL, 1, "CH delay (mS)"},

	//CH2 Settings
	//{NULL, NULL, 6, "PEQ (16)"},
	{NULL, FilterTypeLpSet, 4, "LP filter"},
	{NULL, FilterTypeHpSet, 4, "HP filter"},
	{NULL, NULL, 4, "Peak limiter"},
	{NULL, NULL, 1, "Polarity"},
	{NULL, NULL, 1, "CH delay (mS)"},

	//CH3 Settings
	//{NULL, NULL, 6, "PEQ (16)"},
	{NULL, FilterTypeLpSet, 4, "LP filter"},
	{NULL, FilterTypeHpSet, 4, "HP filter"},
	{NULL, NULL, 4, "Peak limiter"},
	{NULL, NULL, 1, "Polarity"},
	{NULL, NULL, 1, "CH delay (mS)"},

	//CH4 Settings
	//{NULL, NULL, 6, "PEQ (16)"},
	{NULL, FilterTypeLpSet, 4, "LP filter"},
	{NULL, FilterTypeHpSet, 4, "HP filter"},
	{NULL, NULL, 4, "Peak limiter"},
	{NULL, NULL, 1, "Polarity"},
	{NULL, NULL, 1, "CH delay (mS)"},
#if 1
	//Set address
	{NULL, NULL, 0, "IP address:"},
	{SetIpAddress, SetIpAddrEncKeyPress, 0, "192.168.000.010"},
	{NULL, NULL, 0, "Subnet mask:"},
	{SetSubnetMask, SetSubnetMaskEncKeyPress, 0, "255.255.255.000"},
	{NULL, NULL, 0, "Gateway:"},
	{SetGateway, SetGatewayEncKeyPress, 0, "192.168.000.001"},
#endif
	//Mains meters
	{DisplayMainsMeters, NULL, 0, "Mains meters"},

	//Output meters
	{DispalyOutputMeters, NULL, 0, "CH4 Output"},

	//Temperature
	{DisplayTemperature, NULL, 0, "Temperature"},

	//Hardware info
	{DisplayVersion, NULL, 0, "Hardware info"},

	//Level4
	//CH1 Settings
#if 0
	//PEQ (16)
	{NULL, NULL, 0, "PEQ-#: 16"},
	{NULL, NULL, 0, "Active: OFF"},
	{NULL, NULL, 0, "Freq.Hz:12000"},
	{NULL, NULL, 0, "Gain(dB): -15"},
	{NULL, NULL, 0, "Qfactor: 07.0"},
	{NULL, NULL, 0, "Peaking"},
#endif
	//LP filter
	{FilterActive, FilterActivePacket, 0, "Active: OFF"},
	{FilterFreqHz, FilterFreqEncKeyFunc, 0, "Freq.Hz:12000"},
	{FilterSlope, FilterSlopePacketTx, 0, "Slope(db): 06"},
	{FilterButterwooth, FilterButterwoothPacketTx, 0, "Butterwooth"},

	//HP filter
	{FilterActive, FilterActivePacket, 0, "Active: OFF"},
	{FilterFreqHz, FilterFreqEncKeyFunc, 0, "Freq.Hz:12000"},
	{FilterSlope, FilterSlopePacketTx, 0, "Slope(db): 06"},
	{FilterButterwooth, FilterButterwoothPacketTx, 0, "Butterwooth"},
#if 1
	//Peak limiter
	{PeakLimiterActiveSel, PeakLimiterActivePacketTx, 0, "Active: OFF"},
	{PeakLimiterThreshVp, PeakLimiterThreshVpPacketTx, 0, "Thresh.Vp:070"},
	{PeakLimiterAttack, PeakLimiterAttackPacketTx, 0, "Attack(mS):999"},
	{PeakLimiterReleaMs, PeakLimiterReleaMsPacketTx, 0, "Relea.(mS):999"},
#endif
	//Polarity
	{PolarityInPhaseSel, PolarityInPhasePacketTx, 0, "In phase"},

	//CH delay (mS)
	{ChDelay, ChDelayPacketTx, 0, "00 mS"},

	//CH2 Settings
#if 0
	//PEQ (16)
	{NULL, NULL, 0, "PEQ-#: 16"},
	{NULL, NULL, 0, "Active: OFF"},
	{NULL, NULL, 0, "Freq.Hz:12000"},
	{NULL, NULL, 0, "Gain(dB): -15"},
	{NULL, NULL, 0, "Qfactor: 07.0"},
	{NULL, NULL, 0, "Peaking"},
#endif
	//LP filter
	{FilterActive, FilterActivePacket, 0, "Active: OFF"},
	{FilterFreqHz, FilterFreqEncKeyFunc, 0, "Freq.Hz:12000"},
	{FilterSlope, FilterSlopePacketTx, 0, "Slope(db): 06"},
	{FilterButterwooth, FilterButterwoothPacketTx, 0, "Butterwooth"},

	//HP filter
	{FilterActive, FilterActivePacket, 0, "Active: OFF"},
	{FilterFreqHz, FilterFreqEncKeyFunc, 0, "Freq.Hz:12000"},
	{FilterSlope, FilterSlopePacketTx, 0, "Slope(db): 06"},
	{FilterButterwooth, FilterButterwoothPacketTx, 0, "Butterwooth"},
#if 1
	//Peak limiter
	{PeakLimiterActiveSel, PeakLimiterActivePacketTx, 0, "Active: OFF"},
	{PeakLimiterThreshVp, PeakLimiterThreshVpPacketTx, 0, "Thresh.Vp:070"},
	{PeakLimiterAttack, PeakLimiterAttackPacketTx, 0, "Attack(mS):999"},
	{PeakLimiterReleaMs, PeakLimiterReleaMsPacketTx, 0, "Relea.(mS):999"},
#endif

	//Polarity
	{PolarityInPhaseSel, PolarityInPhasePacketTx, 0, "In phase"},

	//CH delay (mS)
	{ChDelay, ChDelayPacketTx, 0, "00 mS"},

	//CH3 Settings
#if 0
	//PEQ (16)
	{NULL, NULL, 0, "PEQ-#: 16"},
	{NULL, NULL, 0, "Active: OFF"},
	{NULL, NULL, 0, "Freq.Hz:12000"},
	{NULL, NULL, 0, "Gain(dB): -15"},
	{NULL, NULL, 0, "Qfactor: 07.0"},
	{NULL, NULL, 0, "Peaking"},
#endif

	//LP filter
	{FilterActive, FilterActivePacket, 0, "Active: OFF"},
	{FilterFreqHz, FilterFreqEncKeyFunc, 0, "Freq.Hz:12000"},
	{FilterSlope, FilterSlopePacketTx, 0, "Slope(db): 06"},
	{FilterButterwooth, FilterButterwoothPacketTx, 0, "Butterwooth"},

	//HP filter
	{FilterActive, FilterActivePacket, 0, "Active: OFF"},
	{FilterFreqHz, FilterFreqEncKeyFunc, 0, "Freq.Hz:12000"},
	{FilterSlope, FilterSlopePacketTx, 0, "Slope(db): 06"},
	{FilterButterwooth, FilterButterwoothPacketTx, 0, "Butterwooth"},

#if 1
	//Peak limiter
	{PeakLimiterActiveSel, PeakLimiterActivePacketTx, 0, "Active: OFF"},
	{PeakLimiterThreshVp, PeakLimiterThreshVpPacketTx, 0, "Thresh.Vp:070"},
	{PeakLimiterAttack, PeakLimiterAttackPacketTx, 0, "Attack(mS):999"},
	{PeakLimiterReleaMs, PeakLimiterReleaMsPacketTx, 0, "Relea.(mS):999"},
#endif

	//Polarity
	{PolarityInPhaseSel, PolarityInPhasePacketTx, 0, "In phase"},
	//CH delay (mS)
	{ChDelay, ChDelayPacketTx, 0, "00 mS"},

	//CH4 Settings
	//PEQ (16)
#if 0
	{NULL, NULL, 0, "PEQ-#: 16"},
	{NULL, NULL, 0, "Active: OFF"},
	{NULL, NULL, 0, "Freq.Hz:12000"},
	{NULL, NULL, 0, "Gain(dB): -15"},
	{NULL, NULL, 0, "Qfactor: 07.0"},
	{NULL, NULL, 0, "Peaking"},
#endif
	//LP filter
	{FilterActive, FilterActivePacket, 0, "Active: OFF"},
	{FilterFreqHz, FilterFreqEncKeyFunc, 0, "Freq.Hz:12000"},
	{FilterSlope, FilterSlopePacketTx, 0, "Slope(db): 06"},
	{FilterButterwooth, FilterButterwoothPacketTx, 0, "Butterwooth"},

	//HP filter
	{FilterActive, FilterActivePacket, 0, "Active: OFF"},
	{FilterFreqHz, FilterFreqEncKeyFunc, 0, "Freq.Hz:12000"},
	{FilterSlope, FilterSlopePacketTx, 0, "Slope(db): 06"},
	{FilterButterwooth, FilterButterwoothPacketTx, 0, "Butterwooth"},
#if 1
	//Peak limiter
	{PeakLimiterActiveSel, PeakLimiterActivePacketTx, 0, "Active: OFF"},
	{PeakLimiterThreshVp, PeakLimiterThreshVpPacketTx, 0, "Thresh.Vp:070"},
	{PeakLimiterAttack, PeakLimiterAttackPacketTx, 0, "Attack(mS):999"},
	{PeakLimiterReleaMs, PeakLimiterReleaMsPacketTx, 0, "Relea.(mS):999"},
#endif

	//Polarity
	{PolarityInPhaseSel, PolarityInPhasePacketTx, 0, "In phase"},

	//CH delay (mS)
	{ChDelay, ChDelayPacketTx, 0, "00 mS"},
};

uint8_t GetSubListFirstItemId(uint8_t curNodeId) {
	char i, subListFirstItemId = 0;

	for  (i = 0; i < curNodeId; i++) {
		subListFirstItemId += ItemPrmtDpa600V2[i].subListItemNum;
	}
	subListFirstItemId++;
	return subListFirstItemId;
}

static void CreateItems(ListHead *listHead) {
	Item *item;
	uint8_t itemId;
	ItemPrmt *itemPrmt;
	listHead->items = (Item *)malloc(sizeof(Item) * listHead->itemNum);
	if (listHead->items == NULL) {
		LedEnable(TRUE);
		BeepEnable();
		while(1);
	}
	itemId = GetSubListFirstItemId(listHead->preItemNodeId);
	for(int i = 0; i < listHead->itemNum; i++) {
		item = &(listHead->items[i]);
		itemPrmt = (ItemPrmt *)&ItemPrmtDpa600V2[itemId];
		item->data8 = itemPrmt->data8;
		item->encoderSpin = itemPrmt->encoderSpin;
		item->keyPressFunc = itemPrmt->keyPressFunc;
		item->nextList = NULL;
		item->nodeId = itemId;
		itemId++;
	}
}

ListHead *CreatePreListItems(ListHead *listHead) {
	if (listHead->items != NULL)
		return listHead;
	CreateItems(listHead);
    return listHead;
}

bool PageNoAdjust = FALSE;
bool Page2LineAdjust = FALSE;
ListHead *CreateSubList(ListHead *listHead, Item *item) {
	ListHead *p = NULL;
	uint8_t itemNum;
	uint8_t nodeId = item ? item->nodeId : 0;
	if (nodeId >= sizeof(ItemPrmtDpa600V2) / sizeof(ItemPrmt))
		return NULL;
	itemNum = ItemPrmtDpa600V2[nodeId].subListItemNum;
	if (itemNum <= 0)
		return NULL;
	if (!listHead || (item && !item->nextList)) {
		p = (ListHead *)malloc(sizeof(ListHead));
		if (p == NULL) {
			LedEnable(TRUE);
			BeepEnable();
			while(1);
		}
		p->previousList = listHead;
		p->preItemNodeId = nodeId;
		p->itemNum = itemNum;
//		p->startItemId = 0;
//		p->curLine = 0;
		if (item)
			item->nextList = p;
		else
			listHead = p;
	}
	else if (item && item->nextList) {
		p = item->nextList;
	}
	else
		p = listHead;
	p->startItemId = 0;
	p->curLine = 0;

	CreateItems(p);
	return p;
}

void MenuInit(Menu *menu)
{
	//menu->timeToHome_ = 0;
    //menu->timeToSync_ = 0;
	while (menu->curList && menu->curList->previousList) {
		menu->curList = menu->curList->previousList;
	}
	menu->curList = CreateSubList(menu->curList, NULL);
}

