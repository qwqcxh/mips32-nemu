#include "common.h"
#include "am.h" 
extern _Context* do_syscall(_Context *c);
extern _Context* schedule(_Context *prev);
static _Context* do_event(_Event e, _Context* c) {
  _AddressSpace* x =(_AddressSpace*)0x81d90004;//debug
  uint32_t* y=(uint32_t*)0x81d90000;//debug
  _Context* ret = NULL;
  switch (e.event) {
    case _EVENT_TLBL: __am_tlb_refill();ret=c;break;
    case _EVENT_SYSCALL: do_syscall(c);break;
    case _EVENT_YIELD: ret = schedule(c);printf("schedule before %x after %x as_addr is %x and pcb[1].as.ptr is %x pcb[1].cp is %x\n",c,ret,ret->as,x->ptr,*y);break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return ret;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
