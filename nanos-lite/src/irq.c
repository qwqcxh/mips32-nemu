#include "common.h"

extern _Context* do_syscall(_Context *c);
extern _Context* schedule(_Context *prev);
static _Context* do_event(_Event e, _Context* c) {
  uint32_t* x =(uint32_t*)0x81d90004;
  _Context* ret = NULL;
  switch (e.event) {
    case _EVENT_TLBL: __am_tlb_refill();ret=c;break;
    case _EVENT_SYSCALL: do_syscall(c);break;
    case _EVENT_YIELD: ret = schedule(c);printf("schedule before %x after %x as_addr is %x and pcb[1].as.ptr is %x\n",c,ret,ret->as,*x);break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return ret;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
