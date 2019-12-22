#ifndef _LedLightBar_h_
#define _LedLightBar_h_
#include "stm8s.h"

void LedLightBarInit();
void LedLevelDisplay(u8 channel, signed char dB);
void LedLightBarSetAll(u8 isAllLight);
#endif

