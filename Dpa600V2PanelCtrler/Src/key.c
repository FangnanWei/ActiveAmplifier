#include "stm8s.h"
#include "key.h"
#include "Gpio.h"
#include "Encoder.h"
#include "stdlib.h"
#include "Lcd.h"
#include "GlobalVal.h"
#include "LedLightBar.h"
#include "Menu.h"
#include "Packet.h"
#include "AlgCfg.h"

void KeyInit(Key *key, const KeyPrmt *keyPrmt)
{
	key->gpioId_ = (GpioId *)&(keyPrmt->gpioId);
	key->keyId = keyPrmt->keyId;
	key->pressEvent_ = FALSE;
	key->releaseEvent_ = FALSE;
	key->pressedTimeMs_ = 0;
	key->keyPressFunc = keyPrmt->keyPressFunc;
	key->keyPresslongFunc = keyPrmt->keyPresslongFunc;
	GPIO_Init(key->gpioId_->port, key->gpioId_->pinNum, GPIO_MODE_IN_PU_NO_IT);
}

void KeyScan(Key *key, unsigned int intervalMs)
{
	volatile bool isPressed;

	isPressed = !GPIO_ReadInputPin(key->gpioId_->port, key->gpioId_->pinNum);

	if (intervalMs == key->pressedTimeMs_ && isPressed){
		key->pressEvent_ = TRUE;
	}

	if (intervalMs < key->pressedTimeMs_ && !isPressed){
    	if(key->pressedTimeMs_ < LONG_PRESS_TIMEOUT)
		    key->releaseEvent_ = TRUE;
	}

	if (isPressed){
		key->pressedTimeMs_ += intervalMs;
	}
	else{
		key->pressedTimeMs_ = 0;
	}

	if (key->pressEvent_){
		key->pressEvent_ = FALSE;
	}
    if(key->pressedTimeMs_ == LONG_PRESS_TIMEOUT) {
		if (key->keyPresslongFunc) {
			key->keyPresslongFunc(key->keyId);
			BeepEnableForMs(BeepRingMs);
		}
    }

	if (key->releaseEvent_){
		key->releaseEvent_ = FALSE;
		if (key->keyId != KeyId_EncKey || !IsHomePage) {
			if (key->keyPressFunc) {
				key->keyPressFunc(key->keyId);
				BeepEnableForMs(BeepRingMs);
			}
		}
	}
}

#if 0
void SwiKeyPressFunc(KeyId keyId) {
	char channel = keyId - KeyId_Sw1;
	if (IsRunning) {
		if (IsHomePage) {
			IsChMute[channel] = TRUE;
			//Mute(true);
			PacketTxSubCmd(CmdList_Mute, channel, (char *)&IsChMute[channel], 1);
			LcdMuteDisplay(channel);
		}
	}
}

void SwiKeyPressLongFunc(KeyId keyId) {
	char channel = keyId - KeyId_Sw1;
	if (IsHomePage && IsRunning) {
		if (!IsChMute[channel]) {
			IsChSelect[channel] = !IsChSelect[channel];
		}
		else {
			IsChMute[channel] = FALSE;
			//Mute(FALSE);
			PacketTxSubCmd(CmdList_Mute, channel, (char *)&IsChMute[channel], 1);
		}
		LcdSelectChannel(channel, IsChSelect[channel]);
	}
}
#endif

void SwiKeyPressFunc(KeyId keyId) {
	char channel = keyId - KeyId_Sw1;
	if (IsRunning && IsHomePage) {
		IsChMute[channel] = !IsChMute[channel];
		//Mute(true);
		PacketTxSubCmd(CmdList_Mute, channel, (char *)&IsChMute[channel], 1);
		if (IsChMute[channel]) {
			LcdMuteDisplay(channel);
		}
		else {
			LcdSelectChannel(channel, IsChSelect[channel]);
		}
	}
}

void SwiKeyPressLongFunc(KeyId keyId) {
	char channel = keyId - KeyId_Sw1;
	if (IsHomePage && IsRunning && !IsChMute[channel]) {
		IsChSelect[channel] = !IsChSelect[channel];
		LcdSelectChannel(channel, IsChSelect[channel]);
	}
}

bool IsLeafSel = FALSE;
void EncoderKeyPressFunc(KeyId keyId) {
	ListHead *nextList, *preList, *curList = _Menu->curList;
	uint8_t *curLine = &(curList->curLine);
	uint8_t itemOffset = *curLine + curList->startItemId;
    Item *item;
    if (curList->itemNum == 1)
        itemOffset = *curLine / 2;
	item = &curList->items[itemOffset];

	if (IsRunning) {
		if (!IsHomePage) {
			if (itemOffset < curList->itemNum) {
				nextList = CreateSubList(curList, item);
				if (nextList) {
					if (item->keyPressFunc)
						item->keyPressFunc(item->nodeId);
					free(curList->items);
					curList->items = NULL;
					_Menu->curList = nextList;
					//_Menu->curList->startItemId = 0;
					//_Menu->curList->curLine = 0;
					if (_Menu->curList->itemNum == 1 || Page2LineAdjust) {
						if (!BigCharDisplay)
							_Menu->curList->curLine++;
					}

					LcdMenuDisplay(_Menu);
				}
				else {
					if (!IsCharSelected)
						IsLeafSel = !IsLeafSel;
					if (item->keyPressFunc)
						item->keyPressFunc(item->nodeId);
					if (IsLeafSel) {
 						LcdLineDisplay(item, *curLine, FALSE, IsLeafSel);
						if (IsCharSelected)
							LcdShowString(SelectCharPos * 8, *curLine, NumChar, TRUE);
					}
					if (!IsLeafSel) {
 						LcdLineDisplay(item, *curLine, TRUE, IsLeafSel);
					}
					//if (IsCharSelected && item->keyPressFunc)
				}
				//执行当前item的cmd
			}
			else if (itemOffset >= curList->itemNum) {
				preList = curList->previousList;
				free(curList->items);
				curList->items = NULL;
				CreatePreListItems(preList);
				BigCharDisplay = FALSE;
				PageNoAdjust = FALSE;
				Page2LineAdjust = FALSE;

				if (preList) {
					_Menu->curList = preList;
					LcdMenuDisplay(_Menu);
				}
				else {
					IsHomePage = TRUE;
					LcdHomePageDisplay();
				}
			}
		}
	}
}

void EncoderKeyPressLongFunc(KeyId keyId) {
	Item *items = _Menu->curList->items;
	if (IsRunning) {
		IsHomePage = !IsHomePage;
		if (IsHomePage) {
			BigCharDisplay = FALSE;
			PageNoAdjust = FALSE;
			Page2LineAdjust = FALSE;
			IsLeafSel = FALSE;
			IsCharSelected = FALSE;
            if (items)
                free(items);
			LcdHomePageDisplay();
		}
		else {
			MenuInit(_Menu);
			LcdMenuDisplay(_Menu);
		}
	}
}

extern void PannelPower(bool isOn);
void PannelPsw(KeyId keyId) {
	IsRunning = !IsRunning;

	PannelPower(IsRunning);
}

