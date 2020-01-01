#include "Util/StringUtil.h"

//#include <ctype.h>
//#include <errno.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <stdint.h>
//#include <stdarg.h>

//StringUtil::StrDup()主要是拷贝字符串s的一个副本，由函数返回值返回，这个副本有自己的内存空间
//在DSP string.h中未提供此函数
char *StringUtil::StrDup(const char *s) {
	if (s==NULL)
		return NULL;
	char *t = new char[strlen(s) + 1];
	if (t==NULL)
		return NULL;
	strcpy(t, s);
	return t;
}