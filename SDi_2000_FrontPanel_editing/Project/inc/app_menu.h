#ifndef _APP_MENU_H_
#define _APP_MENU_H_
#include "stm8s.h"
#include "app_i2c.h"
#include "app_gpio.h"

typedef enum {
	//带有子节点
	HAVE_CHILD 		= (uint8_t)(1<<0),  //有子页面

	//叶子结点
	HORIZ_PAGE 		= (uint8_t)(1<<1),   //水平切换页面 叶子结点
	IS_LEAF  	   	= (uint8_t)(1<<2),   //已经是叶子结点

	//不可选
	BIG_CHAR   		= (uint8_t)(1<<3),  //大字体显示
	IS_VOID			= (uint8_t)(1<<4),	//空行

	IS_BACK	   		= (uint8_t)(1<<5)  //返回按钮

}item_property_TypeDef;

typedef struct {
	bool ChannelSelect[AUDIO_CHANNEL_CNT];		//设置是否选中
	bool Reset; 		  	//是否处于复位状态   
	uint16_t Version;
	uint32_t Magic;	
}DeviceOp_TypeDef;

//设备参数 信息  需要存储
typedef struct {	
	CtrlBoard_Data_TypeDef 	 CtrlBoard[CONTROL_BOARD_CNT];
	MusicData_TypeDef 			 MusicData;
	DeviceOp_TypeDef				 Op;

	uint16_t STPDT_val;
	uint16_t STDDT_val;
}DeviceParameter_TypeDef;

//菜单行最小结构
struct item{
  void (*rolling_selected_item)(struct item *item, encoder_dir_TypeDef turn_dir);			//旋转选中的菜单行
  void (*press_curr_item)(struct item *item);				//按下当前的菜单行

	uint8_t item_property;									//菜单携带的信息

  uint8_t item_prmt_index;								//菜单行参数表 数组编号
  uint8_t item_page_index;								//菜单所在页面的编号 ----主要是为了方便定位通道编号
  char *item_str; 										//菜单行显示字符串内容

	struct menu_page *belong_page;							//所属的页 --为了方便往回找
};

//一个页面结构
struct menu_page {
  struct menu_page *prev_page;			
  uint8_t paren_item_index;			//当前菜单所属的上一级页面的父行数组编号		    

  struct item *items;			//页面菜单行信息集合  需要动态申请和释放内存	
  uint8_t items_num;			//页面菜单总数量				
  uint8_t top_item_id;		//当前页面的第一个item编号  0 1 2 ...tems_num
  uint8_t curr_line;			//当前所在行号
  uint8_t item_offset;		//当前行的菜单偏移量
};

//菜单头--记录者当前菜单页面
struct menu_head {
  struct menu_page *curr_page;						//当前的菜单页面 需要动态申请和释放内存
};

//菜单参数表
struct item_table{
  void (*rolling_selected_item)(struct item *item, encoder_dir_TypeDef turn_dir);
  void (*press_curr_item)(struct item *item);
	
	uint8_t item_property;									//菜单携带的信息

  char sub_items_num;										//当前菜单包含子页面菜单的数量
  char show_str[MAX_SHOW_LENGTH];							//显示的菜单字符串内容
};


//////////////////////////// 菜单关联方法表
void enckey_confirm_input_source(struct item *item);
void enc_change_input_source(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_output_mode(struct item *item);
void enc_change_output_mode(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_max_output_volt(struct item *item);
void enc_change_max_output_volt(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_max_output_curt(struct item *item);
void enc_change_max_output_curt(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_max_output_curt(struct item *item);
void enc_change_max_output_curt(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_temperature_gate(struct item *item);
void enc_change_temperature_gate(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_reset_setting(struct item *item);
void enc_change_reset_setting(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_mains_meters(struct item *item);
void enc_change_mains_meters_page(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_audio_output_meters(struct item *item);
void enc_change_audio_output_meters_page(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_audio_impedance(struct item *item);
void enc_change_audio_impedance_page(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_temperatures(struct item *item);
void enc_change_temperatures_page(struct item *item, encoder_dir_TypeDef turn_dir);

void enckey_confirm_hardware_info(struct item *item);
void enc_change_hardware_info_page(struct item *item, encoder_dir_TypeDef turn_dir);


void enckey_confirm_STDDT_val(struct item *item);
void enc_change_STDDT_val(struct item *item, encoder_dir_TypeDef turn_dir);
void enckey_confirm_STPDT_val(struct item *item);
void enc_change_STPDT_val(struct item *item, encoder_dir_TypeDef turn_dir);


//菜单构建方法
static void create_items(struct menu_page *page);
struct menu_page *create_prev_page_items(struct menu_page *page);
struct menu_page *create_child_menu_page(struct menu_page *curr_page, struct item *curr_item);
void init_menu_page(struct menu_head *menu);

//菜单显示方法
void display_home_page(DeviceParameter_TypeDef *DevPrmts);
void display_item_str(struct item *item, uint8_t line, bool isCurr_line, bool leaf_selected);
void display_menu_page(struct menu_page *page);
void refresh_menu_page(struct menu_page *page);


//--------------定义所有方法，最终归由APP调用
//菜单定义
//菜单控制方法

#endif

