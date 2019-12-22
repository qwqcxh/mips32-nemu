#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    uint32_t* x=(uint32_t*)0x81d90004; //debug
    printf("hello_fun pcb[1].as.ptr is %x\n",*x); //debug
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
  uint32_t* x=(uint32_t*)0x81d90004;
  printf("in schedule pcb[1].as.ptr = %x\n",*x);//debug
  current->cp = prev;
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  return current->cp;
}
