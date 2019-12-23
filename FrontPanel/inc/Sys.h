#ifndef Sys_H__
#define Sys_H__

extern "C" {
#include "stm8s.h"
#include "stm8s_it.h"
}

class Sys{
public:
  static void CpuInit();
  static void DelayMs(uint32_t ms);
  static void DelaySecond(uint32_t second);
  static void BeepRingForMs(uint32_t ms);
};

#endif