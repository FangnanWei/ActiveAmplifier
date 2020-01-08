#ifndef StringUtil_H__
#define StringUtil_H__

//#include <stdint.h>
#include <stdarg.h>

extern "C" {
#include "stm8s.h"
}

class StringUtil {
public:
	static char *StrDup(const char *s);
};
#endif