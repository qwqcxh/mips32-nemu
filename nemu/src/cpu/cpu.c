#include "cpu/exec.h"

CPU_state cpu;

rtlreg_t s0, s1, t0, t1, ir;

/* shared by all helper functions */
DecodeInfo decinfo;

void decinfo_set_jmp(bool is_jmp) {
  decinfo.is_jmp = is_jmp;
}

void isa_exec(vaddr_t *pc);

extern void isa_reg_display();
vaddr_t exec_once(void) {
  decinfo.seq_pc = cpu.pc;
  isa_exec(&decinfo.seq_pc);
  update_pc();
  static bool flag=false;//debug
  if(cpu.pc == 0x400263cc) flag=true;//debug
  if(flag && cpu.pc < 0x40026764 && cpu.pc >= 0x400263cc) {printf("pc: %x\n",cpu.pc);isa_reg_display();}//fprintf(stderr,"pc: 0x%x\n",cpu.pc);//debug

  return decinfo.seq_pc;
}
