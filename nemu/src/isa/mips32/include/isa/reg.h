#ifndef __MIPS32_REG_H__
#define __MIPS32_REG_H__

#include "common.h"

#define PC_START (0x80000000u + IMAGE_START)

typedef struct {
  struct {
    rtlreg_t _32;
  } gpr[32];

  rtlreg_t status,lo,hi,cause,epc;//fuck source code!!! 
  vaddr_t pc;
  rtlreg_t entryhi, entrylo0, entrylo1, index; //cp0 register
  bool INTR;

} CPU_state;

static inline int check_reg_index(int index) {
  assert(index >= 0 && index < 32);
  return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)

static inline const char* reg_name(int index, int width) {
  extern const char* regsl[];
  assert(index >= 0 && index < 32);
  return regsl[index];
}

#endif
