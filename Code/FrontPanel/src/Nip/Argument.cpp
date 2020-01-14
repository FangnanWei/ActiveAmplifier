#include "Nip/Argument.h"
#include "Util/StringUtil.h"
#include <stdlib.h>

void Argument::SetFieldName(const char *fieldName) {
	if (fieldName==NULL)
		fieldName = "";
	if (fieldName_)
		delete[] fieldName_;
	fieldName_ = StringUtil::StrDup(fieldName);
}

Argument::Argument(BranchArg *parent, const char *fieldName) {
	this->parent_ = parent;
	this->fieldName_ = NULL;
	SetFieldName(fieldName);

	if (parent!=NULL)
		parent->AddSon(this);//提示NullErrorMsg可能在构造前被使用
}

Argument::~Argument() {
	delete[] fieldName_;
}


bool BranchArg::AddSon(Argument *arg) { 

	return true;
}