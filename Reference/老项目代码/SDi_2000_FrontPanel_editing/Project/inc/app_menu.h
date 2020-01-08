#ifndef _APP_MENU_H_
#define _APP_MENU_H_
#include "stm8s.h"
#include "app_i2c.h"
#include "app_gpio.h"

typedef enum {
	//�����ӽڵ�
	HAVE_CHILD 		= (uint8_t)(1<<0),  //����ҳ��

	//Ҷ�ӽ��
	HORIZ_PAGE 		= (uint8_t)(1<<1),   //ˮƽ�л�ҳ�� Ҷ�ӽ��
	IS_LEAF  	   	= (uint8_t)(1<<2),   //�Ѿ���Ҷ�ӽ��

	//����ѡ
	BIG_CHAR   		= (uint8_t)(1<<3),  //��������ʾ
	IS_VOID			= (uint8_t)(1<<4),	//����

	IS_BACK	   		= (uint8_t)(1<<5)  //���ذ�ť

}item_property_TypeDef;

typedef struct {
	bool ChannelSelect[AUDIO_CHANNEL_CNT];		//�����Ƿ�ѡ��
	bool Reset; 		  	//�Ƿ��ڸ�λ״̬   
	uint16_t Version;
	uint32_t Magic;	
}DeviceOp_TypeDef;

//�豸���� ��Ϣ  ��Ҫ�洢
typedef struct {	
	CtrlBoard_Data_TypeDef 	 CtrlBoard[CONTROL_BOARD_CNT];
	MusicData_TypeDef 			 MusicData;
	DeviceOp_TypeDef				 Op;

	uint16_t STPDT_val;
	uint16_t STDDT_val;
}DeviceParameter_TypeDef;

//�˵�����С�ṹ
struct item{
  void (*rolling_selected_item)(struct item *item, encoder_dir_TypeDef turn_dir);			//��תѡ�еĲ˵���
  void (*press_curr_item)(struct item *item);				//���µ�ǰ�Ĳ˵���

	uint8_t item_property;									//�˵�Я������Ϣ

  uint8_t item_prmt_index;								//�˵��в����� ������
  uint8_t item_page_index;								//�˵�����ҳ��ı�� ----��Ҫ��Ϊ�˷��㶨λͨ�����
  char *item_str; 										//�˵�����ʾ�ַ�������

	struct menu_page *belong_page;							//������ҳ --Ϊ�˷���������
};

//һ��ҳ��ṹ
struct menu_page {
  struct menu_page *prev_page;			
  uint8_t paren_item_index;			//��ǰ�˵���������һ��ҳ��ĸ���������		    

  struct item *items;			//ҳ��˵�����Ϣ����  ��Ҫ��̬������ͷ��ڴ�	
  uint8_t items_num;			//ҳ��˵�������				
  uint8_t top_item_id;		//��ǰҳ��ĵ�һ��item���  0 1 2 ...tems_num
  uint8_t curr_line;			//��ǰ�����к�
  uint8_t item_offset;		//��ǰ�еĲ˵�ƫ����
};

//�˵�ͷ--��¼�ߵ�ǰ�˵�ҳ��
struct menu_head {
  struct menu_page *curr_page;						//��ǰ�Ĳ˵�ҳ�� ��Ҫ��̬������ͷ��ڴ�
};

//�˵�������
struct item_table{
  void (*rolling_selected_item)(struct item *item, encoder_dir_TypeDef turn_dir);
  void (*press_curr_item)(struct item *item);
	
	uint8_t item_property;									//�˵�Я������Ϣ

  char sub_items_num;										//��ǰ�˵�������ҳ��˵�������
  char show_str[MAX_SHOW_LENGTH];							//��ʾ�Ĳ˵��ַ�������
};


//////////////////////////// �˵�����������
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


//�˵���������
static void create_items(struct menu_page *page);
struct menu_page *create_prev_page_items(struct menu_page *page);
struct menu_page *create_child_menu_page(struct menu_page *curr_page, struct item *curr_item);
void init_menu_page(struct menu_head *menu);

//�˵���ʾ����
void display_home_page(DeviceParameter_TypeDef *DevPrmts);
void display_item_str(struct item *item, uint8_t line, bool isCurr_line, bool leaf_selected);
void display_menu_page(struct menu_page *page);
void refresh_menu_page(struct menu_page *page);


//--------------�������з��������չ���APP����
//�˵�����
//�˵����Ʒ���

#endif

