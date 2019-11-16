#include "nemu.h"

const char *regsl[] = {
  "$0", "at", "v0", "v1", "a0", "a1", "a2", "a3",
  "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
  "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
  "t8", "t9", "k0", "k1", "gp", "sp", "s8", "ra"
};

/*******************work********************/
void isa_reg_display() {   
  for(int i=0;i<32;i++){
    printf("%-10s%x\n",regsl[i],reg_l(i));
  }
}

uint32_t isa_reg_str2val(const char *s, bool *success) {
  return 0;
}
