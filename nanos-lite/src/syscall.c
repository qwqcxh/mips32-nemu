#include "common.h"
#include "syscall.h"
#include "proc.h"

extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

extern void naive_uload(PCB *pcb, const char *filename);

extern char end;
char* program_break=&end;
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_yield: _yield();c->GPRx=0;break;
    case SYS_brk: program_break=(char*)c->GPR2; c->GPRx=0; break;
    case SYS_exit: _halt(c->GPR2);break;//naive_uload(NULL,"/bin/init"); break;
    case SYS_open:c->GPRx=fs_open((char*)c->GPR2,c->GPR3,c->GPR4);break;
    case SYS_read:c->GPRx=fs_read(c->GPR2,(void*)c->GPR3,c->GPR4);break;
    case SYS_close:c->GPRx=fs_close(c->GPR2);break;
    case SYS_lseek:c->GPRx=fs_lseek(c->GPR2,c->GPR3,c->GPR4);break;
    case SYS_write:c->GPRx = fs_write(c->GPR2,(void*)c->GPR3,c->GPR4);break;
    case SYS_execve:naive_uload(NULL,(char*)c->GPR2);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
