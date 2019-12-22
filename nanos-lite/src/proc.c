#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  // _AddressSpace* x=(_AddressSpace*)0x81d90004; //debug
  // printf("hello_fun pcb[1].as.ptr is %x pcb[1].cp is %x\n",x->ptr,*y); //debug
  int sp;
  uint32_t* y=(uint32_t*)0x81d90000;//debug
  __asm__ __volatile__ ("sw $sp,%0":"=m"(sp));
  printf("fp in hello fun is %x and pcb[1].cp is %x\n",sp,*y);//debug
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

extern void naive_uload(PCB *pcb, const char *filename);
extern void context_kload(PCB *pcb, void *entry);
extern void context_uload(PCB *pcb, const char *filename) ;
void init_proc() {
  context_kload(&pcb[0], (void *)hello_fun);
  context_uload(&pcb[1], "/bin/init");
  switch_boot_pcb();
  printf("&pcb[0] = %x &pcb[1] = %x pcb[0].cp=%x pcb[1].cp=%x\n",&pcb[0],&pcb[1],pcb[0].cp,pcb[1].cp);//debug
  Log("Initializing processes...");

  // load program here
  // naive_uload(&pcb_boot,"/bin/init");
  _yield();
}

_Context* schedule(_Context *prev) {
  _AddressSpace* x=(_AddressSpace*)0x81d90004; //debg
  uint32_t* y=(uint32_t*)0x81d90000;//debug
  printf("in schedule pcb[1].as.ptr = %x pcb[1].cp is %x\n",x->ptr,*y);//debug
  current->cp = prev;
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  return current->cp;
}
