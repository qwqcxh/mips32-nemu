#include "rtl/rtl.h"
#include <setjmp.h>

void raise_intr(uint32_t NO, vaddr_t epc) {  //no use in mips32 because there is no IDT in mips 23333333
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
}

#define IRQ_TIMER 0           // for mips32
bool isa_query_intr(void) {
 if ( cpu.INTR && (cpu.status&2)) {
    cpu.INTR = false;
    rtl_mv(&cpu.epc,&cpu.pc);
    rtl_andi(&cpu.cause,&cpu.cause,0xffffff83);
    rtl_andi(&cpu.status,&cpu.status,0xfffffffd);
    rtl_ori(&cpu.cause,&cpu.cause,0); //execode of interrupt is 0 
    rtl_ori(&cpu.status,&cpu.status,0x2); //close interrupt
    rtl_j(0x80000180);
    return true;
  }
  return false;
}

jmp_buf intr_buf;

void longjmp_raise_intr(uint32_t NO) {
  longjmp(intr_buf, NO + 1);
}
