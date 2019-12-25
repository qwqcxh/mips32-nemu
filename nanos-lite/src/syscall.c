#include "common.h"
#include "syscall.h"
#include "proc.h"

extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

extern void naive_uload(PCB *pcb, const char *filename);

// extern char end;
// char* program_break=&end;
// extern int mm_brk(uintptr_t brk);
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_yield: _yield();c->GPRx=0;break;
    case SYS_brk: c->GPRx = mm_brk(c->GPR2); break;
      // program_break=(char*)c->GPR2; c->GPRx=0; break;
    case SYS_exit:printf("sys_exit: cpu.epc %x\n",c->epc); naive_uload(NULL,"/bin/init"); break;
    case SYS_open:printf("sys_open: cpu.epc %x\n",c->epc); c->GPRx=fs_open((char*)c->GPR2,c->GPR3,c->GPR4);break;
    case SYS_read:printf("sys_read: cpu.epc %x\n",c->epc); c->GPRx=fs_read(c->GPR2,(void*)c->GPR3,c->GPR4);break;
    case SYS_close:printf("sys_close: cpu.epc %x\n",c->epc); c->GPRx=fs_close(c->GPR2);break;
    case SYS_lseek:printf("sys_lseek: cpu.epc %x\n",c->epc); c->GPRx=fs_lseek(c->GPR2,c->GPR3,c->GPR4);break;
    case SYS_write:printf("sys_write: cpu.epc %x\n",c->epc); c->GPRx = fs_write(c->GPR2,(void*)c->GPR3,c->GPR4);break;
    case SYS_execve:printf("sys_execve: cpu.epc %x\n",c->epc); naive_uload(NULL,(char*)c->GPR2);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return c;
}
