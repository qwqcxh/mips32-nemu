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
    _yield();
  }
}

extern void naive_uload(PCB *pcb, const char *filename);
extern void context_kload(PCB *pcb, void *entry);
extern void context_uload(PCB *pcb, const char *filename) ;
void init_proc() {
  // context_kload(&pcb[0], (void *)hello_fun);
  context_uload(&pcb[0],"/bin/hello");
  context_uload(&pcb[1], "/bin/pal");
  switch_boot_pcb();
  Log("Initializing processes...");

  // load program here
  _yield();
}

_Context* schedule(_Context *prev) {
  static int count=100;
  current->cp = prev;
  if(current==&pcb[0]) current=&pcb[1];
  else if(current==&pcb[1]){
    count = (count-1+100)%100;
    if(count==0) current=&pcb[0];
  }
  else current = &pcb[1];
  // current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  return current->cp;
}
