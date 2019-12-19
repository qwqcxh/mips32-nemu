#include <am.h>
#include <mips32.h>
#include <klib.h>
static _Context* (*user_handler)(_Event, _Context*) = NULL;

_Context* __am_irq_handle(_Context *c) {
  //debug
  // for(int i=0;i<32;i++) printf("%d ",c->gpr[i]);
  // printf("\n");
  // printf("lo is %d hi is %d cause is %d status is %d epc is %d\n",c->lo,c->hi,c->cause,c->status,c->epc);
  //over
  _Context *next = c;
  if (user_handler) {
    _Event ev = {0};
    uint32_t ex_code = (c->cause>>2)&0x1f;
    switch (ex_code) {
      case 8:  ev.event = _EVENT_SYSCALL;break;
      case 13: ev.event = _EVENT_YIELD; break;
      default: ev.event = _EVENT_ERROR; break;
    }

    next = user_handler(ev, c);
    if (next == NULL) {
      next = c;
    }
  }

  return next;
}

extern void __am_asm_trap(void);

#define EX_ENTRY 0x80000180

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize exception entry
  const uint32_t j_opcode = 0x08000000;
  uint32_t instr = j_opcode | (((uint32_t)__am_asm_trap >> 2) & 0x3ffffff);
  *(uint32_t *)EX_ENTRY = instr;
  *(uint32_t *)(EX_ENTRY + 4) = 0;  // delay slot
  *(uint32_t *)0x80000000 = instr;  // TLB refill exception
  *(uint32_t *)(0x80000000 + 4) = 0;  // delay slot

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  _Context* p = (_Context*)(stack.end - sizeof(_Context) - 40);
  p->epc = (uintptr_t)entry;
  p->gpr[29] = (uint32_t)p;
  return p;
}

void _yield() {
  asm volatile("syscall 1");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
