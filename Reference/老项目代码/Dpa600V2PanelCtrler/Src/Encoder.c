/******************************************
                    gpio.c
                    2015-09-08          zhangfei
*******************************************/
#include "Encoder.h"
#include "timer.h"
#include "LedLightBar.h"
#include "GlobalVal.h"
#include "Lcd.h"
#include "Delay.h"
#include "Key.h"
#include "AlgCfg.h"
#include "Packet.h"

static unsigned char EncoderLeftTurnCnt = 0;
static unsigned char EncoderRightTurnCnt = 0;

void IncEncoderTurn(TurnDir_e dir)
{
	dir == TURN_Dir_L ? EncoderLeftTurnCnt++ : EncoderRightTurnCnt++;
}

void DecEncoderTurn(TurnDir_e dir)
{
	dir == TURN_Dir_L ? EncoderLeftTurnCnt-- : EncoderRightTurnCnt--;
}

unsigned char IsEncoderTurnValid(TurnDir_e dir)
{
    return 	dir == TURN_Dir_L ? EncoderLeftTurnCnt : EncoderRightTurnCnt;
}

void EncoderEc11Init()
{
    GPIO_Init(ENCODER_Port, ENCODER_A | ENCODER_B, GPIO_MODE_IN_FL_NO_IT);
    //GPIO_Init(ENCODER_Port, , GPIO_MODE_IN_FL_NO_IT);
}

void ScanEncoder()
{
    static unsigned char encoderA = 0xff;
    static unsigned char encoderB = 0xff;

	if (encoderA == 0xff)
	{
        encoderA = GPIO_ReadInputPin(ENCODER_Port, ENCODER_A);
	    encoderB = GPIO_ReadInputPin(ENCODER_Port, ENCODER_B);
	}

    if (encoderA != GPIO_ReadInputPin(ENCODER_Port, ENCODER_A))
    {
        Delay(1);
        if (encoderA != GPIO_ReadInputPin(ENCODER_Port, ENCODER_A))
        {
	        //Delay(5);
            encoderA = GPIO_ReadInputPin(ENCODER_Port, ENCODER_A);
            encoderB = GPIO_ReadInputPin(ENCODER_Port, ENCODER_B);
			IncEncoderTurn((encoderA ^ encoderB) ? TURN_Dir_L : TURN_Dir_R);
        }
    }
}

void OpWhenLeafSeled(Item *item, uint8_t line) {
	if (item->encoderSpin) {
		item->encoderSpin(item);
		if (!IsCharSelected)
			LcdLineDisplay(item, line, FALSE, IsLeafSel);
		else
			LcdShowString(SelectCharPos * 8, line, NumChar, TRUE);
			if (BigCharDisplay)
				Lcd12x32StrDisplay(CapPrmtStr);
	}
}

bool EncoderAdjust[4] = FALSE;
void EncoderLeftPro()
{
	char i;
	ListHead *curList = _Menu->curList;
	uint8_t *curLine = &curList->curLine;
	uint8_t itemOffset = *curLine + curList->startItemId;
	Item *firstItem = curList->items;
    if (curList->itemNum == 1)
        itemOffset = *curLine / 2;

	if (IsHomePage) {
		for (i = 0; i < 4; i++) {
			if (IsChSelect[i] && OutGainVals[i] > -99 && !IsChMute[i]) {
				OutGainVals[i]--;
				LcdGainValDisplay(i, OutGainVals[i]);
				//串口发送衰减增加命令到Dsp
				//PacketTxSubCmd(CmdList_OutGain, i, &OutGainVals[i], 1);
				EncoderAdjust[i] = TRUE;
			}
		}
	}
	else if (!IsLeafSel) {
		//菜单光标下移
		if (itemOffset < curList->itemNum && !PageNoAdjust) {
			LcdLineDisplay(firstItem + itemOffset, *curLine, FALSE, FALSE);

            (*curLine)++;
			itemOffset++;
			if (curList->itemNum == 1 || (curList->itemNum == 2 && *curLine == 2) || (Page2LineAdjust && itemOffset < curList->itemNum)) {
	            (*curLine)++;
                if (Page2LineAdjust)
					itemOffset++;
			}
			if (BigCharDisplay)
				(*curLine)++;
            if (itemOffset > curList->itemNum) {
					itemOffset = curList->itemNum;
           }
			if (*curLine < LcdMaxLine) {
				if (itemOffset < curList->itemNum) {
					LcdLineDisplay(firstItem + itemOffset, *curLine, TRUE, IsLeafSel);
				}
				else {
					LcdBacKDisplay(*curLine, TRUE);
				}
			}
	 		else {
				*curLine = LcdMaxLine - 1;
				curList->startItemId++;
				if (Page2LineAdjust && (*curLine + curList->startItemId < curList->itemNum - 1))
					curList->startItemId++;
				LcdMenuDisplay(_Menu);
			}
		}
	}
	else {
		OpWhenLeafSeled(firstItem + itemOffset, *curLine);
	}
}

void EncoderRightPro()
{
	char i;
	ListHead *curList = _Menu->curList;
	uint8_t *curLine = &curList->curLine;
	signed itemOffset = *curLine + curList->startItemId;
	Item *firstItem = curList->items;
    if (curList->itemNum == 1)
        itemOffset = *curLine / 2;
	if (IsHomePage) {
		for (i = 0; i < 4; i++) {
			if (IsChSelect[i] && OutGainVals[i] < 0 && !IsChMute[i]) {
				OutGainVals[i]++;
				LcdGainValDisplay(i, OutGainVals[i]);
				//串口发送衰减减少命令到Dsp
				//PacketTxSubCmd(CmdList_OutGain, i, &OutGainVals[i], 1);
				EncoderAdjust[i] = TRUE;
			}
		}
	}
	else if (!IsLeafSel) {
		//菜单光标上移
		if (itemOffset > 0 && !PageNoAdjust && !(Page2LineAdjust && itemOffset == 1)) {
			if (itemOffset >= curList->itemNum) {
				LcdBacKDisplay(*curLine, FALSE);
			}
			else {
				LcdLineDisplay(firstItem + itemOffset, *curLine, FALSE, FALSE);
			}
			if (*curLine > 0 && !(Page2LineAdjust && itemOffset < curList->itemNum && *curLine < 3)) {
				if (*curLine > 0)
					(*curLine)--;
 				itemOffset--;
				if (curList->itemNum == 1 || (curList->itemNum == 2 && *curLine == 2) || (Page2LineAdjust && itemOffset != curList->itemNum - 1)) {
		            (*curLine)--;
	                if (Page2LineAdjust && itemOffset > 0)
						itemOffset--;
				}
				if (BigCharDisplay && *curLine > 0)
					(*curLine)--;
				LcdLineDisplay(firstItem + itemOffset, *curLine, TRUE, IsLeafSel);
			}
			else if (curList->startItemId > 0) {
				if (Page2LineAdjust)
					*curLine = 1;
				else
					*curLine = 0;
				curList->startItemId--;
				if (Page2LineAdjust) {
					if (curList->startItemId % 2 && curList->startItemId > 0)
						curList->startItemId--;
                }
				LcdMenuDisplay(_Menu);
			}
		}
	}
	else {
		OpWhenLeafSeled(firstItem + itemOffset, *curLine);
	}
}

void EncoderProc()
{
    if (IsEncoderTurnValid(TURN_Dir_L))
    {
        EncoderLeftPro();
        DecEncoderTurn(TURN_Dir_L);
    }
    if (IsEncoderTurnValid(TURN_Dir_R))
    {
        EncoderRightPro();
        DecEncoderTurn(TURN_Dir_R);
    }
}

