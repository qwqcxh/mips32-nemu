#include "nemu.h"
#include "monitor/diff-test.h"

#define RAMDISK_START (0x83000000u)
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  for(int i=0;i<32;i++){
    if(ref_r->gpr[i]._32!=cpu.gpr[i]._32){
      printf("%s: qemu 0x%x nemu 0x%x\n",reg_name(i,4),ref_r->gpr[i]._32,cpu.gpr[i]._32);
      return false;
    }
  }
  return true;
}

extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
#define RAMDISK_SIZE ((&ramdisk_end) - (&ramdisk_start))
void isa_difftest_attach(void) {
  // ref_difftest_memcpy_from_dut(PC_START, guest_to_host(IMAGE_START), img_size);
  char *begin = guest_to_host(0);
  ref_difftest_memcpy_from_dut(RAMDISK_START, begin, RAMDISK_SIZE);
  ref_difftest_setregs(&cpu);
}
