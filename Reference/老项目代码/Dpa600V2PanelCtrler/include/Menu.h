#ifndef _Menu_h_
#define _Menu_h_
#include "stm8s.h"

//#define MAX_LINE_LENGTH                                      15
#define MAX_SHOW_LENGTH                                      15
//#define DISPLAY_LEFT_OFFSET                                  0
//#define  MAX_MENU_NUMS                                       256
//#define STATUS_DISPLAY_POS                                   10
//#define InvalidCmd				0

typedef struct ListHead_t {
	struct ListHead_t *previousList;
	struct Item_t *items;
	uint8_t preItemNodeId;
	uint8_t itemNum;
	uint8_t startItemId;
	uint8_t curLine;
} ListHead;

typedef struct Item_t {
	ListHead *nextList;
	//char data8[MAX_SHOW_LENGTH];
	char *data8;
	void (*encoderSpin)(struct Item_t *item);
	void (*keyPressFunc)(uint8_t nodeId);
	uint8_t nodeId;
} Item;

typedef struct ItemCfg_t {
	void (*encoderSpin)(Item *item);
	void (*keyPressFunc)(uint8_t nodeId);
	char subListItemNum;
	char data8[MAX_SHOW_LENGTH];
} ItemPrmt;

typedef struct Menu_t {
	//uint32_t timeToSync_;
	//uint32_t timeToHome_;
	//uint8_t maxNodeNum_;
	ListHead *curList;
	//char currentIp_[MAX_SHOW_LENGTH];
	//char currentScene_[MAX_SHOW_LENGTH];
}Menu;

extern const ItemPrmt ItemPrmtDpa600V2[];
extern bool PageNoAdjust;
extern bool Page2LineAdjust;
extern char	StartLine;

void MenuInit(Menu *menu);
ListHead *CreatePreListItems(ListHead *listHead);
ListHead *CreateSubList(ListHead *listHead, Item *item);
uint8_t GetSubListFirstItemId(uint8_t curNodeId);

#endif
