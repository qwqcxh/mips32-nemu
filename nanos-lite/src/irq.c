#include "common.h"

extern _Context* do_syscall(_Context *c);
extern _Context* schedule(_Context *prev);
static _Context* do_event(_Event e, _Context* c) {
  _Context* ret = NULL;
  void* x = (void*)0x81d8f000;
  switch (e.event) {
    case _EVENT_TLBL: c->epc-=4;__am_tlb_refill();ret=c;break;
    case _EVENT_SYSCALL: do_syscall(c);break;
    case _EVENT_YIELD: ret = schedule(c);printf("schedule before %x after %x and pcb[1].cp %x\n",c,ret,*(uint32_t*)x);break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return ret;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
