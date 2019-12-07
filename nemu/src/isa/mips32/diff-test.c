#include "nemu.h"
#include "monitor/diff-test.h"
#include "cpu/decode.h"
extern DecodeInfo decinfo;
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  uint32_t opcode=decinfo.isa.instr.opcode;
  uint32_t func=decinfo.isa.instr.func;
  if((opcode>=2&&opcode<=7) || (func>=8&&func<=9)){
    difftest_skip_dut(1,2);
    return true;
  }
  for(int i=0;i<32;i++){
    if(ref_r->gpr[i]._32!=cpu.gpr[i]._32)
      return false;
  }
  return true;
}

void isa_difftest_attach(void) {
}
