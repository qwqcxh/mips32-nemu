#include "common.h"
#include "syscall.h"

extern char end;
char* program_break=&end;
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_yield: _yield();c->GPRx=0;break;
    case SYS_write:{
      int fd=c->GPR2;
      char* p=(char*)c->GPR3;
      size_t count = c->GPR4;
      if(fd==1 || fd == 0){
        for(int i=0;i<count;i++)
          _putc(p[i]);
        c->GPRx=count;
      }
      else c->GPRx=-1;
      break;
    }
    case SYS_brk: program_break=(char*)c->GPR2; c->GPRx=0; break;
    case SYS_exit: _halt(c->GPR2); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
