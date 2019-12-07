#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  for(int i=0;i<32;i++){
    if(ref_r->gpr[i]._32!=cpu.gpr[i]._32){
      printf("%s: qemu 0x%x nemu 0x%x\n",reg_name(i,4),ref_r->gpr[i]._32,cpu.gpr[i]._32);
      return false;
    }
  }
  return true;
}

void isa_difftest_attach(void) {
}
