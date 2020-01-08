#include "app.h"
#include "app_menu.h"

#include "app_tim.h"
#include "app_beep.h"


#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define MAGIC_NUMBER  0x1155AAFF

struct device_global_prmt g_dev = {
	.IsHomePage 			= TRUE,
	.IsPowerOn 			= TRUE,

	.is_leaf_select 		= FALSE,
	.refresh_menu 			= FALSE,
	.reset_dev 			= FALSE,
	.need_save_prmt 		= FALSE
};

char control_board_err_str[][MAX_SHOW_LENGTH] = {
	{"AMP 1#   "},
	{"AMP 2#   "},
	{"AMP 1# 2# "},
	{"ERROR !   "}
};

bool CtrlBoardRun[CONTROL_BOARD_CNT] = {FALSE, FALSE}; //控制板运行状态

DeviceParameter_TypeDef g_prmt;
struct menu_head *_Menu;

extern const uint16_t SOFT_VERSION;
extern Power_HandlerTypeDef hPower;

void proc_key_between_home_and_menu_page(void)
{
	struct menu_page *prev_page;

	//if(dev_key.is_long_press[4]) {

	    //g_dev_prmt.IsHomePage = (bool)!g_dev_prmt.IsHomePage;

		//所有参数恢复默认
		g_dev.is_leaf_select = FALSE;
		g_dev.refresh_menu = FALSE;
		g_dev.reset_dev = FALSE;
		g_dev.need_save_prmt = FALSE;

	    if (g_dev.IsHomePage) {
			//释放菜单项内存   -----考虑不释放    菜单页面同时也要释放
	        if (_Menu->curr_page->items) {
	            free(_Menu->curr_page->items);
				_Menu->curr_page->items = NULL;
			}

		   //要把当前层及以上层的菜单都释放完毕
			while (_Menu->curr_page) {
				prev_page = _Menu->curr_page->prev_page;// 记录该堆指针
				free(_Menu->curr_page);
				_Menu->curr_page = NULL;
				_Menu->curr_page = prev_page;
			}

	        display_home_page(&g_prmt);
	    } else {
	        init_menu_page(_Menu);    //需要重新初始化菜单
	        display_menu_page(_Menu->curr_page);
	    }

		//dev_key.is_long_press[4] = FALSE;
	//}
}

void proc_item_leaf_selected(struct item *item, uint8_t line, encoder_dir_TypeDef turn_dir)
{
	if (item->rolling_selected_item) {
		item->rolling_selected_item(item, turn_dir);

		display_item_str(item, line, TRUE, TRUE);
	}
}

void proc_encoder_left_turn_menu(struct menu_page *page)
{
	signed  prev_offset = page->item_offset;//暂存菜单偏移量
	uint8_t prev_line = page->curr_line;
	uint8_t prev_top_item_id = page->top_item_id; //根据该值是否变化决定是否翻页

	//当前所在行不菜单的第一行	还可以向上移动
	if (!g_dev.is_leaf_select) {
		if (prev_offset > 0) {
			page->item_offset--;

			while (page->items[page->item_offset].item_property & (BIG_CHAR|IS_VOID)) { //找到不属于大写字符的那一行为止
				page->item_offset--;
				if (page->item_offset == 0) {
					break;
				}
			}

			if (page->items[page->item_offset].item_property & (BIG_CHAR|IS_VOID)) {
				page->item_offset = prev_offset;
			}

			if ((prev_offset - page->item_offset) == 1) {
				if (page->curr_line > 0) {
					page->curr_line--;
				}
			}
			else if ((prev_offset - page->item_offset) > 1) {
				page->curr_line = 0;
			}

			if (page->item_offset < prev_top_item_id) { //现在的偏移位置在之前的首行之前
				page->top_item_id = page->item_offset;
			}

			if (page->top_item_id == prev_top_item_id) {//不翻页
				display_item_str(page->items + prev_offset, prev_line, FALSE, FALSE);					//刷新当前行 去除旗标
				display_item_str(page->items + page->item_offset, page->curr_line, TRUE, FALSE);
			} else { //翻页
				display_menu_page(_Menu->curr_page);
			}
		}
	}
	else {
		proc_item_leaf_selected(&page->items[page->item_offset], page->curr_line, TURN_LEFT);
	}
}

void proc_encoder_right_turn_menu(struct menu_page *page)
{
	signed prev_offset = page->item_offset;//暂存菜单偏移量
	uint8_t prev_line = page->curr_line;
	uint8_t prev_top_item_id = page->top_item_id; //根据该值是否变化决定是否翻页

	if (!g_dev.is_leaf_select) {
		if (prev_offset < (page->items_num - 1)) {
			//偏移量更新
			page->item_offset++;
			while (page->items[page->item_offset].item_property & (BIG_CHAR|IS_VOID)) { //找到不属于大写字符的那一行为止
				page->item_offset++;
				if (page->item_offset == (page->items_num - 1)) {
					break;
				}
			}

			if (page->items[page->item_offset].item_property & (BIG_CHAR|IS_VOID)) {
				page->item_offset = prev_offset;
			}

			if ((page->item_offset - prev_offset) == 1) {
				if (page->curr_line < (MAX_SHOW_LINE - 1)) {
					page->curr_line++;
				}
			}
			else if ((page->item_offset - prev_offset) > 1) {
				page->curr_line = MAX_SHOW_LINE - 1;
			}

			if ((page->item_offset - page->curr_line) > prev_top_item_id) {
				page->top_item_id = page->item_offset - page->curr_line;
			}

			if (page->top_item_id == prev_top_item_id) {//不翻页
				display_item_str(page->items + prev_offset, prev_line, FALSE, FALSE); 					//刷新当前行 去除旗标
	            display_item_str(page->items + page->item_offset, page->curr_line, TRUE, FALSE);
			} else { //翻页
				display_menu_page(_Menu->curr_page);
			}
		}
	}
	else {
		proc_item_leaf_selected(&page->items[page->item_offset], page->curr_line, TURN_RIGHT);
	}
}

void proc_encoder_left_turn_home(void)
{
	uint8_t channel = 0;

	for (; channel < 4; channel++) 
	{
		if (g_prmt.Op.ChannelSelect[channel] &&  (!g_prmt.MusicData.Mute[channel])) {

			if (g_prmt.MusicData.CurrGain[channel] > MIN_GAIN_VAL) {
				g_prmt.MusicData.CurrGain[channel]--;
			}
			Display_ShowHomepageGainValue(channel, g_prmt.MusicData.CurrGain[channel]);
			MusicCtrl_SetChannelGain(channel, g_prmt.MusicData.CurrGain[channel]);
			//set_auduo_gain(channel, dev_prmt.OutGainVals[channel]);
			g_dev.need_save_prmt = TRUE;
		}
	}
}

void proc_encoder_right_turn_home(void)
{
	//右边转动 增大音量
	uint8_t channel = 0;

	for (; channel < 4; channel++) 
	{
		if (g_prmt.Op.ChannelSelect[channel] &&  (!g_prmt.MusicData.Mute[channel])) 
		{

			if (g_prmt.MusicData.CurrGain[channel] < MAX_GAIN_VAL) 
			{
				g_prmt.MusicData.CurrGain[channel]++;
			}
			Display_ShowHomepageGainValue(channel, g_prmt.MusicData.CurrGain[channel]);

			MusicCtrl_SetChannelGain(channel, g_prmt.MusicData.CurrGain[channel]);
			g_dev.need_save_prmt = TRUE;
		}
	}
}

void proc_key_menu_page(void)
{
	struct menu_page *child_page, *prev_page, *curr_page = _Menu->curr_page;
	 struct item *item;

	 item = &curr_page->items[curr_page->item_offset];

	 if (item->item_property & IS_BACK) {	 //返回上一级
		 prev_page = curr_page->prev_page;			 //将菜单列表赋值给上一级菜单

		 free(curr_page->items);				 //释放当前菜单项
		 curr_page->items = NULL;
		 free(curr_page);						 //释放当前菜单页
		 curr_page = NULL;

		 create_prev_page_items(prev_page); 		 //构建父菜单项

		 g_dev.refresh_menu = FALSE;

		 //if (g_dev_prmt.clean_hardware_info) {
		 	//for (uint8_t group_id = 0; group_id < CONTROL_BOARD_CNT; group_id++) {
			//	fill_control_board_config_value(group_id,CLEAN_HARDWARE_INFO);
		 //	}
			//g_dev_prmt.clean_hardware_info = FALSE;
		 //}

		 if (prev_page) {							 //如果父菜单构建成功,刷新菜单显示父菜单,否则显示HOME界面
			 _Menu->curr_page = prev_page;
			 display_menu_page(_Menu->curr_page);
		 } else {
			 g_dev.IsHomePage = TRUE;
			 display_home_page(&g_prmt);
		 }
	 }
	 else {

		 if (item->item_property & HAVE_CHILD) {
			 child_page = create_child_menu_page(curr_page, item);

			 if (item->press_curr_item) {
			 	 item->press_curr_item(item);
			 }
			 free(curr_page->items);
			 curr_page->items = NULL;
			 _Menu->curr_page = child_page;
			 display_menu_page(_Menu->curr_page);//显示换成刷新
			 //refresh_menu_page(_Menu->curr_page);
		 }
		 else if (item->item_property & (IS_LEAF|HORIZ_PAGE)) {

			 g_dev.is_leaf_select = (bool)!g_dev.is_leaf_select;

			 if (item->press_curr_item) {
				 item->press_curr_item(item);   //进去和退出都执行一次
				 //dev_prmt_table.reset = 1;
				 //dev_parameter_table_write();  //TODO
			 }

			 display_item_str(item, curr_page->curr_line, TRUE, g_dev.is_leaf_select);
		 }
	 }
}

void DeviceParameter_CleanVoidValue(DeviceParameter_TypeDef *DevPrmts)
{
	//将仪表采样参数清除掉
	for (uint8_t i = 0; i < CONTROL_BOARD_CNT; i++) {
		DevPrmts->CtrlBoard[i].Meters.PowerVoltage = 0;
		DevPrmts->CtrlBoard[i].Meters.PowerCurrent = 0;
		
		DevPrmts->CtrlBoard[i].Meters.DeviceTemperature = 0;
		
		DevPrmts->CtrlBoard[i].Meters.ExtiHFPCnt = 0;
		DevPrmts->CtrlBoard[i].Meters.ExtiUVPCnt = 0;
		DevPrmts->CtrlBoard[i].Meters.ExtiDCPCnt = 0;
		DevPrmts->CtrlBoard[i].Meters.ExtiOCP1Cnt = 0;
		DevPrmts->CtrlBoard[i].Meters.ExtiOCP2Cnt = 0;

		DevPrmts->CtrlBoard[i].Meters.AudioCh1Voltage = 0;
		DevPrmts->CtrlBoard[i].Meters.AudioCh1Current = 0;
		DevPrmts->CtrlBoard[i].Meters.AudioCh1Impedence = 0;

		DevPrmts->CtrlBoard[i].Meters.AudioCh2Voltage = 0;
		DevPrmts->CtrlBoard[i].Meters.AudioCh2Current = 0;
		DevPrmts->CtrlBoard[i].Meters.AudioCh2Impedence = 0;
	}
}

void DeviceParameter_ResetValue(DeviceParameter_TypeDef *DevPrmts)
{
	uint8_t i = 0;

	for (i = 0; i < AUDIO_CHANNEL_CNT; i++) 
	{
		DevPrmts->MusicData.CurrGain[i] = 0;    //0 -80
		DevPrmts->MusicData.Mute[i] = FALSE;
		DevPrmts->Op.ChannelSelect[i] = FALSE;
	}

	for (i = 0; i < CONTROL_BOARD_CNT; i++) 
	{
		DevPrmts->MusicData.InputSource[i] = XLR;
		DevPrmts->MusicData.OutputMode[i] = STEREO;
		DevPrmts->CtrlBoard[i].Config.OVVLimit = 70; //35~70
		DevPrmts->CtrlBoard[i].Config.OCVLimit = 30; //5~30
		DevPrmts->CtrlBoard[i].Config.TemperatureGate = 40;
	}

	//dev_prmt.magic = 0;	//不去配置它

	DevPrmts->Op.Reset = FALSE;
	DevPrmts->STPDT_val = 2;   //1~100
	DevPrmts->STDDT_val = 15;   //1~100
}

void DeviceParameter_InitAndConfig(void)
{
	uint8_t channel = 0;
	uint8_t BoardIndex = 0;
	uint8_t repeat_cnt = 0;
	uint8_t  BoardRuningCnt = 0;
	uint16_t BoardStatusData = BOARD_BEING_CONFIG;
	uint8_t err_str_index = 0;
	Display_HandlerTypeDef hDisplay;

	//读取EEPROM设备参数表
	get_dev_parameter_table((uint8_t *)&g_prmt, sizeof(DeviceParameter_TypeDef));

	if ((g_prmt.Op.Magic != MAGIC_NUMBER) || (g_prmt.Op.Version!= SOFT_VERSION)) 
	{
		DeviceParameter_ResetValue(&g_prmt);

		g_prmt.Op.Magic = MAGIC_NUMBER;
		g_prmt.Op.Version = SOFT_VERSION;
	}
	
	//仪表类数据先清除干净 并保存一次
	DeviceParameter_CleanVoidValue(&g_prmt);
	save_dev_parameter_table((uint8_t *)&g_prmt, sizeof(DeviceParameter_TypeDef));

	//系统先全部静音
	for (channel = 0; channel < AUDIO_CHANNEL_CNT; channel++) 
	{
		MusicCtrl_SetChannelMute(channel);
	}
	
	//获取控制板状态 设定标志
	while (repeat_cnt < 5)
	{
		for (BoardIndex = 0; BoardIndex < CONTROL_BOARD_CNT; BoardIndex++) 
		{
			if (CtrlBoardRun[BoardIndex] != TRUE)
			{
				BoardStatusData = BOARD_BEING_CONFIG;
				CtrlerBoard_ReadNewData(BoardIndex, I2C_DATA_DEVICE_STATUS, &BoardStatusData);
				if (BoardStatusData == BOARD_RUNNING)
				{
					CtrlBoardRun[BoardIndex] = TRUE;
					BoardRuningCnt++;
				}
			}
		}

		if (BoardRuningCnt == CONTROL_BOARD_CNT) 
		{
			break;
		}

		repeat_cnt++;
		DelayNopMs(500);
	}
	
	//配置正常的控制板设备参数
	for (BoardIndex = 0; (BoardIndex < CONTROL_BOARD_CNT) && (CtrlBoardRun[BoardIndex] != FALSE); BoardIndex++) 
	{
#if 1	
		CtrlerBoard_ConfigWithNewData(BoardIndex, I2C_DATA_TEMP_GATE, g_prmt.CtrlBoard[BoardIndex].Config.TemperatureGate);
		CtrlerBoard_ConfigWithNewData(BoardIndex, I2C_DATA_OVV_LIMIT, g_prmt.CtrlBoard[BoardIndex].Config.OVVLimit);
		CtrlerBoard_ConfigWithNewData(BoardIndex,I2C_DATA_OCV_LIMIT, g_prmt.CtrlBoard[BoardIndex].Config.OCVLimit);
		//CtrlerBoard_ConfigWithNewData(BoardIndex, I2C_DATA_LIMIT_STDDT, dev_prmt.STDDT_val);
		//CtrlerBoard_ConfigWithNewData(BoardIndex, I2C_DATA_LIMIT_STPDT, dev_prmt.STPDT_val);
#else
	  //读取控制板的配置参数  填充到前面板
#endif		
	}

	//配置通道输入源和输出模式
	MusicCtrl_SetInputSourceAndOutputMode(&g_prmt.MusicData);
	
	//非静音通道淡入
	for (channel = 0; channel < AUDIO_CHANNEL_CNT; channel++) 
	{
		if (g_prmt.MusicData.Mute[channel] != TRUE) 
		{
			MusicCtrl_SetChannelFadein(channel, g_prmt.MusicData.CurrGain[channel]);
		}
	}
	
	//显示错误模块信息
	if (BoardRuningCnt != CONTROL_BOARD_CNT)
	{
		if (BoardRuningCnt == 0) //两个错误
		{
			err_str_index = 2;
		}
		else
		{
			if (CtrlBoardRun[0] != TRUE)
			{
				err_str_index = 0;
			}
			else
			{
				err_str_index = 1;
			}
		}
		Display_CleanWholeAreaToBlack();
		
		hDisplay.HorizontalStart = 4;
		hDisplay.Str = control_board_err_str[err_str_index];
		hDisplay.Reverse = FALSE;
		hDisplay.Line = 1;
		Display_ShowString(&hDisplay);

		hDisplay.Line = 2;
		hDisplay.Str = control_board_err_str[3];
		Display_ShowString(&hDisplay);

		DelayNopMs(3000);
	}
}

//3初始化和显示页面 菜单/home界面
void Interface_InitAndDisplay(void)
{
	_Menu = (struct menu_head *)malloc(sizeof(struct menu_head));
  _Menu->curr_page= NULL;

	_Menu->curr_page = create_child_menu_page(_Menu->curr_page, NULL);

	display_home_page(&g_prmt);

#if 0

	if (g_dev_prmt.IsPowerOn) 
	{
		if (g_dev_prmt.IsHomePage) 
		{
			display_home_page(&dev_prmt);
		}
		else 
		{
			if (_Menu->curr_page)
			{
				display_menu_page(_Menu->curr_page);
			}
		}
	}
#endif	
}

//4数据更新和菜单更新
void updata_info(void)
{
	//刷新菜单
	if (g_dev.refresh_menu) 
	{
		if (!g_dev.IsHomePage) 
		{
			refresh_menu_page(_Menu->curr_page);
		}
	}

	//复位参数
	if (g_prmt.Op.Reset && g_dev.reset_dev) 
	{
		g_prmt.Op.Reset = FALSE;
		

		DeviceParameter_ResetValue(&g_prmt);
		g_prmt.Op.Magic = MAGIC_NUMBER;

		//save_dev_parameter_table((uint8_t *)&dev_prmt, sizeof(struct device_prmt));
		g_dev.reset_dev = FALSE;
		g_dev.need_save_prmt = TRUE;

		Screen_SetInToSleepMode();
		DelayNopMs(1000);
		Screen_QuitOutSleepMode();
	}

	//保存参数
	if (g_dev.need_save_prmt) 
	{
		save_dev_parameter_table((uint8_t *)&g_prmt, sizeof(DeviceParameter_TypeDef));

		g_dev.need_save_prmt = FALSE;
	}
}

uint8_t CtrlerBoard_ConfigWithNewData(uint8_t group_id, I2C_Slave_CmdTypeDef Cmd, uint16_t Data)
{
	CtrlerBoard_DataPackTypeDef DataPack;
	uint8_t ret;
	switch (Cmd)
	{
		case I2C_DATA_TEMP_GATE:
		case I2C_DATA_OVV_LIMIT:
		case I2C_DATA_OCV_LIMIT:
		case I2C_DATA_LIMIT_STDDT:
		case I2C_DATA_LIMIT_STPDT:		
		case I2C_DATA_LIMIT_RESTM:
		case I2C_CMD_MASTER:
			DataPack.Cmd = Cmd;
			DataPack.Data = Data;
		ret =	I2C_ControlBoard_SendData(group_id, &DataPack);
			break;
		default:
			break;
	}
	return ret;
}

void CtrlerBoard_ReadNewData(uint8_t group_id, I2C_Slave_CmdTypeDef Cmd, uint16_t *Data)
{
	CtrlerBoard_DataPackTypeDef DataPack;
	uint8_t ret = 1;

	switch (Cmd)
	{
		case I2C_DATA_POWER_VOLTAGE:
		case I2C_DATA_POWER_CURRENT:

		case I2C_DATA_DEVICE_TRMPERATURE:
		case I2C_DATA_AUDIO_CH1_VOLTAGE:
		case I2C_DATA_AUDIO_CH1_CURRENT:
		case I2C_DATA_AUDIO_CH1_IMPEDANCE:
		case I2C_DATA_AUDIO_CH2_VOLTAGE:
		case I2C_DATA_AUDIO_CH2_CURRENT:
		case I2C_DATA_AUDIO_CH2_IMPEDANCE:
		case I2C_DATA_DEVICE_STATUS:
			DataPack.Cmd = Cmd;
			ret = I2C_ControlBoard_GetData(group_id, &DataPack);
			break;
		default:
			break;
	}

	if (ret == 0)
	{
		(*Data) = DataPack.Data;
	}
}

//编码器代码驱动源
void Encoder_PressorTurnAction(Encoder_HandlerTypeDef *hEncoder)
{
	if (hEncoder->RightTurn) 
	{
		if (g_dev.IsHomePage) 
		{
			proc_encoder_right_turn_home();
		}
		else 
		{
			proc_encoder_right_turn_menu(_Menu->curr_page);
		}
		hEncoder->RightTurn = FALSE;
		g_dev.need_save_prmt = TRUE;
	}
	else if (hEncoder->LeftTurn) 
	{
		if (g_dev.IsHomePage) 
		{
			proc_encoder_left_turn_home();
		}
		else 
		{
			proc_encoder_left_turn_menu(_Menu->curr_page);
		}
		hEncoder->LeftTurn = FALSE;
		g_dev.need_save_prmt = TRUE;
	}
}

void EnckSwit_PressorPressAction(Key_HandlerTypeDef *EnckSwit)
{
	if (EnckSwit->LongPress != FALSE) 
	{
		//BEEP_BSP_SetOn();
		BEEP_BSP_SetOnAwhile(250);
		g_dev.IsHomePage = (bool)!g_dev.IsHomePage;
		proc_key_between_home_and_menu_page();
		EnckSwit->LongPress = FALSE;
		//BEEP_BSP_SetOff();
	}

	if (g_dev.IsHomePage != TRUE)
	{
		if (EnckSwit->ShortPress != FALSE) //编码器按键
		{
			BEEP_BSP_SetOn();
			proc_key_menu_page();
			EnckSwit->ShortPress = FALSE;
			BEEP_BSP_SetOff();
		}
	}
}

void GeneralSwit_PressorPressAction(Keys_TypeDef *hKeys)
{
	uint8_t GenoralKeyIndex;
	uint8_t channel;
	
	for (GenoralKeyIndex = 0, channel = 0; (GenoralKeyIndex < GENERAL_SWITCH_KEY_CNT) && (channel < AUDIO_CHANNEL_CNT); GenoralKeyIndex++, channel++)
	{
		if (hKeys->GeneralSwit[GenoralKeyIndex].ShortPress != FALSE) 
		{
			hKeys->GeneralSwit[GenoralKeyIndex].ShortPress = FALSE;
			BEEP_BSP_SetOn();
			
			g_prmt.MusicData.Mute[channel] = (bool)!g_prmt.MusicData.Mute[channel];
			if (g_prmt.MusicData.Mute[channel]) 
			{
				Display_ShowHomepageMute(channel);
				//淡出静音
				//set_audio_fadeout(channel,dev_prmt.OutGainVals[channel], 8);
				MusicCtrl_SetChannelFadeout(channel, g_prmt.MusicData.CurrGain[channel]);
			} 
			else 
			{
				Display_ShowHomepageChannel(channel, g_prmt.Op.ChannelSelect[channel]);
				//淡入
				MusicCtrl_SetChannelFadein(channel,  g_prmt.MusicData.CurrGain[channel]);
				//set_audio_fadein(channel,dev_prmt.OutGainVals[channel], 8);
			}
			BEEP_BSP_SetOff();
			g_dev.need_save_prmt = TRUE;
		}
	
		if ((hKeys->GeneralSwit[GenoralKeyIndex].LongPress != FALSE) && (!g_prmt.MusicData.Mute[channel]))	//长按	通道未静音
		{ 
			hKeys->GeneralSwit[GenoralKeyIndex].LongPress = FALSE;
			BEEP_BSP_SetOn();
			g_prmt.Op.ChannelSelect[channel] = (bool)!g_prmt.Op.ChannelSelect[channel];
	
			Display_ShowHomepageChannel(channel, g_prmt.Op.ChannelSelect[channel]);//直接更新
			BEEP_BSP_SetOff();
			g_dev.need_save_prmt = TRUE;
		}
	} 
}

void PowerSwit_PressorPressAction(Key_HandlerTypeDef *PowerSwit)
{
	if (PowerSwit->LongPress != FALSE) 
	{
		g_dev.IsPowerOn = (bool)!g_dev.IsPowerOn;//切换电源状态

		if (g_dev.IsPowerOn) 
		{
			hPower.IsOn = TRUE;
			Power_SetLedStatus(&hPower);
			for (uint8_t channel = 0; channel < AUDIO_CHANNEL_CNT; channel++) 
			{
				if (g_prmt.MusicData.Mute[channel]) 
				{
					MusicCtrl_SetChannelMute(channel);
					//set_auduo_mute(channel);
				}
				else 
				{
					//set_audio_fadein(channel,dev_prmt.OutGainVals[channel], 8);
					MusicCtrl_SetChannelFadein(channel,  g_prmt.MusicData.CurrGain[channel]);
				}
			}
			Screen_QuitOutSleepMode();
		}
		else 
		{
			hPower.IsOn = FALSE;
			Power_SetLedStatus(&hPower);
			for (uint8_t channel = 0; channel < AUDIO_CHANNEL_CNT; channel++) 
			{
				if (g_prmt.MusicData.Mute[channel]) 
				{
					//set_auduo_mute(channel);
					MusicCtrl_SetChannelMute(channel);
				}
				else 
				{
					MusicCtrl_SetChannelFadeout(channel, g_prmt.MusicData.CurrGain[channel]);
				}
			}
			if (!g_dev.IsHomePage) 
			{
				g_dev.IsHomePage = TRUE;
				proc_key_between_home_and_menu_page();
			}
			Screen_SetInToSleepMode();
		}
		PowerSwit->LongPress = FALSE;

		BEEP_BSP_SetOnAwhile(250);
	}
}
