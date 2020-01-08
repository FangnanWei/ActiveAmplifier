#ifndef _AlgCfg_h_
#define _AlgCfg_h_
#include "stm8s.h"
#include "Menu.h"
#if 0
typedef struct PeqCfg_t {
	char segment;
	bool active;
	uint16_t freqHz;
	uint16_t qFactor;
	char peaking;
} PeqCfg;
#endif

typedef struct FilterCfg_t {
	bool active;
	//uint16_t freqHz;
	uint16_t freqHz;
	uint8_t slope;
	char butterwoothIdx;
} FilterCfg;

typedef struct PeakLimiterCfg_t {
	uint8_t threshVp;
	uint16_t releaMs;
	bool active;
	uint8_t attackMs;
}PeakLimiterCfg;

typedef struct DspSetting_t {
	//PeqCfg peqCfg;
	FilterCfg filterCfg[2];
	bool polarity;
	PeakLimiterCfg peakLimiterCfg;
	uint16_t chDelay;
} ChSetting;

typedef struct SetIpCfg_t {
	char ipAddr[4];
	char subnetMask[4];
	char gateway[4];
} SetIpCfg;

typedef enum {
	FilterType_Hp,
	FilterType_Lp,
} FilterType;
//#pragma pack()

void AlgCfgInit();

void InputSelectSet(Item *item);
void InSelSetPacketSend(uint8_t nodeId);

void InputGainSens(uint8_t nodeId);

void InputGainSensSet(Item *item);
void InGainPacketSend(uint8_t nodeId);

void MaxOutputVolt(uint8_t nodeId);

void MaxOutputVoltSet(Item *item);
void MaxOutVoltPacketSend(uint8_t nodeId);

void MaxOutputCurt(uint8_t nodeId);

void MaxOutputCurtSet(Item *item);
void MaxOutCurtPacketSend(uint8_t nodeId);

void MaxMainsCurt(uint8_t nodeId);

void MaxMainsCurtSet(Item *item);
void MaxMainsCurtPacketSend(uint8_t nodeId);

void DspChItemIdStartGet(uint8_t nodeId);
void DspChSet(uint8_t nodeId);
void SourceSelect(Item *item);
void SrcSelPacketTx(uint8_t nodeId);

void AesGain(Item *item);
void AesGainPacketTx(uint8_t nodeId);

void FilterTypeLpSet(uint8_t nodeId);
void FilterTypeHpSet(uint8_t nodeId);

void FilterActive(Item *item);
void FilterActivePacket(uint8_t nodeId);

void FilterFreqHz(Item *item);
void FilterFreqEncKeyFunc(uint8_t nodeId);

void FilterSlope(Item *item);
void FilterSlopePacketTx(uint8_t nodeId);

void FilterButterwooth(Item *item);
void FilterButterwoothPacketTx(uint8_t nodeId);

void PolarityInPhaseSel(Item *item);
void PolarityInPhasePacketTx(uint8_t nodeId);

void PeakLimiterActiveSel(Item *item);
void PeakLimiterActivePacketTx(uint8_t nodeId);

void PeakLimiterThreshVp(Item *item);
void PeakLimiterThreshVpPacketTx(uint8_t nodeId);

void PeakLimiterAttack(Item *item);
void PeakLimiterAttackPacketTx(uint8_t nodeId);

void ChDelay(Item *item);
void ChDelayPacketTx(uint8_t nodeId);


void PeakLimiterReleaMs(Item *item);
void PeakLimiterReleaMsPacketTx(uint8_t nodeId);


void OutGroupIdx0Get(uint8_t nodeId);
void OutGroupIdx1Get(uint8_t nodeId);

void OutModeSel(Item *item);
void OutModePacketSend(uint8_t nodeId);

void DisplayMainsMeters(Item *item);
void DispalyOutputMeters(Item *item);
void DisplayTemperature(Item *item);
void DisplayVersion(Item *item);

void IpConfig(uint8_t nodeId);

void SetIpAddress(Item *item);
void SetIpAddrEncKeyPress(uint8_t nodeId);

void SetSubnetMask(Item *item);
void SetSubnetMaskEncKeyPress(uint8_t nodeId);

void SetGateway(Item *item);
void SetGatewayEncKeyPress(uint8_t nodeId);

void BigCharDisplaySet(uint8_t nodeId);

extern bool IsCharSelected;
extern char NumChar[2];
extern int8_t SelectCharPos;
extern ChSetting ChSettingVal[4];
extern uint8_t FilterTypeId;
extern char InGainVal[4];
extern uint8_t InputSelectId;
extern char MaxOutCurtVal[4];
extern char MaxMainsCurtVal;
extern char OutGroupIdx;
extern char OutMode[2];

extern char MaxOutVoltVal[4];

extern bool BigCharDisplay;
extern char *CapPrmtStr;

extern uint16_t CapMainsMetersV;
extern uint16_t CapMainsMetersA;
extern uint16_t CapOutMetersV[4];
extern uint16_t CapOutMetersA[4];
extern uint8_t CapTemperatureC;
extern uint8_t Version[2];

extern SetIpCfg SetIpCfgVal;

#endif

