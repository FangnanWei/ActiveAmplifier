#include "App/Menu.h"
#include "Util/StringUtil.h"
#include <stdlib.h>

Item::Item(ItemType itemType, Item *parent, const char *fieldName, int initSize) : 
  subItems_(initSize, 0)
{
  	this->itemType_ = itemType;
	this->parent_ = parent;
	this->fieldName_ = NULL;
	SetFieldName(fieldName);
	
	if (parent != NULL)
		parent->AddSon(this);
}

void Item::SetFieldName(const char *fieldName) {
	if (fieldName==NULL)
		fieldName = "";
	if (fieldName_)
		delete[] fieldName_;
	fieldName_ = StringUtil::StrDup(fieldName);
}
	
bool Item::AddSon(Item *son) {
	if (son==NULL)
		return false;
	uint8_t pos = subItems_.GetCount();
	if (!subItems_.Add(son, pos)) {
		return false;
	}
	return true;
}

void Menu::ShowSubItems() {
	oled_->Clear();
	uint8_t index = firstLineIndex_[level_];
	for (uint8_t line = 0; line < MaxLineCnt; line++) {
		Item *son = currItem_->GetSon(index);
		if (son) {
			oled_->ShowString(0, line*2, (const uint8_t *)son->GetFieldName());
		}
		else {
			break;
		}
		index++;
	}
	ShowSymbol();
}

//在当前行上显示
void Menu::ShowSymbol() {
	uint8_t flag[2]  = {95 + ' ', 0};	
	oled_->ShowString((16-1)*8, focusLine_[level_]*2, flag);  
}

void Menu::ClearSymbol() {
	uint8_t clear[2] = {' ', 0};
	oled_->ShowString((16-1)*8, focusLine_[level_]*2, clear);
}

void Menu::Rolling(RotaryEncoderType encType) {
  	//计算出新的 firstLineIndex_ 和 focusLine_ 《先不考虑类型有效无效》
	if(encType == RotaryEncoder_LeftTurn) {
		if (focusLine_[level_] != 0) {
		  	ClearSymbol();
			focusLine_[level_]--;
			ShowSymbol();
		}
		else if ((focusLine_[level_] == 0) && (firstLineIndex_[level_] != 0)){
			firstLineIndex_[level_]--;
			ShowSubItems();
		}
	}
	else {
	  if ((focusLine_[level_] < (MaxLineCnt - 1)) && ((firstLineIndex_[level_] + focusLine_[level_]) < (subItemCnt_ - 1))) {
		ClearSymbol();
		focusLine_[level_]++;
		ShowSymbol();
	  }
	  else if ((focusLine_[level_] == (MaxLineCnt - 1)) && ((firstLineIndex_[level_] + focusLine_[level_]) < (subItemCnt_ - 1))){
		firstLineIndex_[level_]++;
		ShowSubItems();
	  }
	}
}

void Menu::Press(KeyPressType keyType) {
	if (keyType == KeyPressType_Short) {
		uint8_t currIndex = firstLineIndex_[level_] + focusLine_[level_];
		Item *son = currItem_->GetSon(currIndex);
		if (son){
			uint16_t itemType = son->GetItemType();
			switch (itemType) {
			case ItemType_IsBranch:
				currItem_ = son;
				subItemCnt_ = son->GetSize();
				level_++;
				firstLineIndex_[level_] = 0;
				focusLine_[level_] = 0;	
				ShowSubItems();
			  break;
			case ItemType_Writable:
			  	son->ShortPress();
			  break;
			case ItemType_IsBack:
			  	Item *parent = currItem_->GetParent();
			  	currItem_ = parent;
				subItemCnt_ = parent->GetSize();
				level_--;
				ShowSubItems();
			  break;
			default:
			  break;
			}
		}
	}
	else {

	}
}



