#ifndef Menu_H__
#define Menu_H__

extern "C" {
#include "stm8s.h"
}
#include "Util/Array.h"
#include "Driver/Chip/Oled.h"
#include "Driver/Chip/RotaryEncoder.h"
#include "Driver/Chip/Key.h"

class Page;
class Item;

enum ItemType 
{
	ItemType_None		= 0x0,
	ItemType_Writable	= 0x1,
	ItemType_Visible	= 0x2,
	ItemType_IsBranch	= 0x8,
	ItemType_IsBack		= 0x10
};

//�˵�����С��Ԫ--һ�һ��------������
class Item{
public:
	Item(ItemType itemType, Item *parent, const char *fieldName, int initSize);
  	virtual void ShortPress(){}
	virtual void Turning(){}
	
	void SetFieldName(const char *fieldName);
	inline const char *GetFieldName() {return fieldName_; }
	inline uint16_t GetItemType() { return itemType_; }
	
	inline Item *GetParent() { return parent_; }
  	bool AddSon(Item *son);
	inline Item *GetSon(uint8_t index) {
		uint8_t cnt = subItems_.GetCount();
		if (index < cnt) {
			Item *item = *subItems_.Get(index);
			return item;
		}
		else {
			return NULL;
		}
	}
  	inline int GetSize() const {
		return subItems_.GetCount();
	}
protected:	
  	char	 	  *fieldName_;	//�������ַ�����
  	Array<Item *> subItems_;
	Item		  *parent_;
	uint16_t	  itemType_;
};

class BackItem : public Item{
public:
  	BackItem(Item *parent, const char *fieldName):
	  Item(ItemType_IsBack, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

//�˵�����(home/ParemSetting/OutputMode)
class OutputModeSTR4RItem : public Item{
public:
  	OutputModeSTR4RItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class OutputModeSTR8RItem : public Item{
public:
  	OutputModeSTR8RItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class ParemSettingOutputModeItem : public Item{
public:
  	OutputModeSTR4RItem STR4RItem_;
	OutputModeSTR8RItem STR8RItem_;
	BackItem			back_;
  	ParemSettingOutputModeItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 3),
	  STR4RItem_(this, "STR-4R/BRG70V"),
	  STR8RItem_(this, "STR-8R/BRG100V"),
	  back_(this, "Back"){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

//�˵�����(home/ParemSetting/InputSelect)
class InputSelectAnalogItem : public Item{
public:
  	InputSelectAnalogItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class InputSelectDanteItem : public Item{
public:
  	InputSelectDanteItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class ParemSettingInputSelectItem : public Item{
public:
  	InputSelectAnalogItem analogItem_;
  	InputSelectDanteItem  danteItem_;
  	BackItem			  back_;
	
  	ParemSettingInputSelectItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 3),
	  analogItem_(this, "Analog"),
	  danteItem_(this, "Dante"),
	  back_(this, "Back"){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

//�˵�����(home/ParemSetting/InputGain)
class InputGainPlus6dBItem : public Item{
public:
  	InputGainPlus6dBItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class InputGainPlus0dBItem : public Item{
public:
  	InputGainPlus0dBItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class InputGainSub6dBItem : public Item{
public:
  	InputGainSub6dBItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class ParemSettingInputGainItem : public Item{
public:
  	InputGainPlus6dBItem plus6dBItem_;
	InputGainPlus0dBItem plus0dBItem_;
	InputGainSub6dBItem  sub6dBItem_;
	BackItem			 back_;
	
  	ParemSettingInputGainItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 4),
	  plus6dBItem_(this, "+6dB"),
	  plus0dBItem_(this, "+0dB"),
	  sub6dBItem_(this, "-6dB"),
	  back_(this, "Back"){
	  
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

//�˵�����(home/ParemSetting/DSP)
class DSPBypassItem : public Item{
public:
  	DSPBypassItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class DSPActiveItem : public Item{
public:
  	DSPActiveItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class DSPModeItem : public Item{
public:
  	DSPModeItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};


class ParemSettingDSPItem : public Item{
public:
  	DSPBypassItem bypassItem_;
	DSPActiveItem activeItem_;
	DSPModeItem	  modeItem_;
    BackItem	  back_;
	
  	ParemSettingDSPItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 4),
	  bypassItem_(this, "Bypass"),
	  activeItem_(this, "Active"),
	  modeItem_(this, "Mode"),
	  back_(this, "Back"){
	  
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class ParemSettingItem : public Item{
public:
  	ParemSettingOutputModeItem  outputModeItem_;
	ParemSettingInputSelectItem inputSelectItem_;
	ParemSettingInputGainItem   inputGainItem_;
  	ParemSettingDSPItem			DSPItem_;
	BackItem	  				back_;
  
	ParemSettingItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 5), 
	  outputModeItem_(this, "Output Mode"),
	  inputSelectItem_(this, "Input Select"),
	  inputGainItem_(this, "Input Gain"),
	  DSPItem_(this, "DSP"),
	  back_(this, "Back"){
	  
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};


//�˵�����(home/Display)
class DisplayCHAItem : public Item{
public:
  	DisplayCHAItem(Item *parent, const char *fieldName):
	  Item(ItemType_Visible, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class DisplayCHBItem : public Item{
public:
  	DisplayCHBItem(Item *parent, const char *fieldName):
	  Item(ItemType_Visible, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class DisplayTempItem : public Item{
public:
  	DisplayTempItem(Item *parent, const char *fieldName):
	  Item(ItemType_Visible, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class DisplayItem : public Item{
public:
  	DisplayCHAItem  CHAItem_;
	DisplayCHBItem  CHBItem_;
	DisplayTempItem tempItem_;
	BackItem	  	back_;
  
	DisplayItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 4),
	  CHAItem_(this, "CHA:50Vrms"),
	  CHBItem_(this, "CHB:50Vrms"),
	  tempItem_(this, "Temp:32oC"),
	  back_(this, "Back"){
	  
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};


//�˵�����(home/SystemSetup)

//�˵�����(home/SystemSetup/SystemInfo)
class SystemInfoVersionItem : public Item{
public:
  	SystemInfoVersionItem(Item *parent, const char *fieldName):
	  Item(ItemType_Visible, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class SystemSetupSystemInfoItem : public Item{
public:
	SystemInfoVersionItem versionItem_;
	BackItem	  	      back_;
	
  	SystemSetupSystemInfoItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 2),
	  versionItem_(this, "Version:1.00"),
	  back_(this, "Back"){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

//�˵�����(home/SystemSetup/OLEDContrast)
class OLEDContrastConstrastItem : public Item{
public:
  	OLEDContrastConstrastItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class SystemSetupOLEDContrastItem : public Item{
public:
  	OLEDContrastConstrastItem constrastItem_;
	BackItem	  	          back_;
  
  	SystemSetupOLEDContrastItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 2),
	  constrastItem_(this, "Contrast:<10>"),
	  back_(this, "Back"){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

//�˵�����(home/SystemSetup/IPConfig)
class IPConfigDHCPItem : public Item{
public:
  	IPConfigDHCPItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

//�˵�����(home/SystemSetup/IPConfig/IP)
class IPIPItem : public Item{
public:
  	IPIPItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class IPMKItem : public Item{
public:
  	IPMKItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class IPGWItem : public Item{
public:
  	IPGWItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class IPConfigIPItem : public Item{
public:
  	IPIPItem  IPItem_;
  	IPMKItem  MKItem_;
	IPGWItem  GWItem_;
	BackItem  back_;
	
  	IPConfigIPItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 4),
	  IPItem_(this, "IP:192.168.000.010"),
	  MKItem_(this, "MK:255.255.255.000"),
	  GWItem_(this, "GW:192.168.000.001"),
	  back_(this, "Back"){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

//�˵�����(home/SystemSetup/IPConfig/DNS)
class DNSAutoItem : public Item{
public:
  	DNSAutoItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class DNSDNItem : public Item{
public:
  	DNSDNItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class IPConfigDNSItem : public Item{
public:
  	DNSAutoItem autoItem_;
	DNSDNItem   DNItem_;
	BackItem    back_;
  
  	IPConfigDNSItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 3),
	  autoItem_(this, "Auto<ON>"),
	  DNItem_(this, "DN:192.168.000.001"),
	  back_(this, "Back"){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class SystemSetupIPConfigItem : public Item{
public:
  	IPConfigDHCPItem DHCPItem_;
	IPConfigIPItem   IPItem_;
	IPConfigDNSItem  DNSItem_;
	BackItem    	 back_;

  	SystemSetupIPConfigItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 4),
	  DHCPItem_(this, "DHCP<ON>"),
	  IPItem_(this, "IP"),
	  DNSItem_(this, "DNS"),
	  back_(this, "Back"){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class SystemSetupLOCKItem : public Item{
public:
  	SystemSetupLOCKItem(Item *parent, const char *fieldName):
	  Item(ItemType_Writable, parent, fieldName, 0){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

class SystemSetupItem : public Item{
public:
  	SystemSetupSystemInfoItem   systemInfoItem_;
	SystemSetupOLEDContrastItem OLEDContrastItem_;
  	SystemSetupIPConfigItem     IPConfigItem_;
	SystemSetupLOCKItem			LOCKItem_;
	BackItem    	 			back_;
	  
  	SystemSetupItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 5),
	  systemInfoItem_(this, "System Info"),
	  OLEDContrastItem_(this, "OLED Contrast"),
	  IPConfigItem_(this, "IP Config"),
	  LOCKItem_(this, "LOCK<ON>"),
	  back_(this, "Back"){
	  }
  	virtual void ShortPress(){}
	virtual void Turning(){}
};

//�˵�����(root:)
class RootItem : public Item{
public:
  	ParemSettingItem paremSettingItem_;
  	DisplayItem      displayItem_;
  	SystemSetupItem	 systemSetupItem_;
	
  	RootItem(Item *parent, const char *fieldName) : 
	  Item(ItemType_IsBranch, parent, fieldName, 3),
	  paremSettingItem_(this, "Param Setting"),
	  displayItem_(this, "Display"),
	  systemSetupItem_(this, "System Setup"){
	  
	  }
  
    virtual void ShortPress(){}
	virtual void Turning(){}
};

class Menu{
public:
  	static const uint8_t MaxLineCnt = 4;
	static const uint8_t MaxLevelDeepth_ = 5;
	Menu(Oled *oled){
	  	oled_ = oled;
		rootItem_ = new RootItem(NULL, "ROOT");
		
		currItem_ = rootItem_;
		subItemCnt_ = currItem_->GetSize();
		
		level_ = 0;
		
		firstLineIndex_[level_] = 0;
		focusLine_[level_] = 0;
	}
	void ShowSubItems();
	void ShowSymbol();
	void ClearSymbol();
	
	void Rolling(RotaryEncoderType encType);   //���ձ���������--�ı���ʾ��
	void Press(KeyPressType keyType);
	
  
private:
  	Oled 	 *oled_;
	RootItem *rootItem_;
	
	Item     *currItem_;
	uint8_t  subItemCnt_;
	
	//�˵�״̬����
	uint8_t  level_;
	uint8_t  focusLine_[MaxLevelDeepth_];	  //��ǰ��ʾ���к�
	uint8_t  firstLineIndex_[MaxLevelDeepth_];//��һ�еĲ˵����š������ֵ���䣬����Ҫȫ��ˢ�£�ֻ��Ҫ���� ���
};

#endif
