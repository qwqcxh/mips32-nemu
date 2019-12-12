#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  uintptr_t gpr[32],lo,hi,cause,status,epc;
  struct _AddressSpace *as;
};

#define GPR1 gpr[2]
#define GPR2 gpr[4]
#define GPR3 gpr[5]
#define GPR4 gpr[6]
#define GPRx gpr[2]

#endif
