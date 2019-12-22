#ifndef _GlobalVal_h_
#define _GlobalVal_h_
#include "Menu.h"

extern bool IsRunning;
extern bool IsHomePage;
extern bool IsChSelect[4];
extern bool IsChMute[4];
extern signed char OutGainVals[4];
extern Menu *_Menu;

#define GainValMin -99
#define GainValMax 0
#endif