#include "Util/StringUtil.h"

//#include <ctype.h>
//#include <errno.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <stdint.h>
//#include <stdarg.h>

//StringUtil::StrDup()��Ҫ�ǿ����ַ���s��һ���������ɺ�������ֵ���أ�����������Լ����ڴ�ռ�
//��DSP string.h��δ�ṩ�˺���
char *StringUtil::StrDup(const char *s) {
	if (s==NULL)
		return NULL;
	char *t = new char[strlen(s) + 1];
	if (t==NULL)
		return NULL;
	strcpy(t, s);
	return t;
}