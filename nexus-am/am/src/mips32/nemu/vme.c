#include <am.h>
#include <mips32.h>
#include <nemu.h>
#include "klib.h"

#define PG_ALIGN __attribute((aligned(PGSIZE)))

static void* (*pgalloc_usr)(size_t) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;
  vme_enable = 1;

  return 0;
}

int _protect(_AddressSpace *as) {
  as->ptr = (PDE*)(pgalloc_usr(1));

  return 0;
}

void _unprotect(_AddressSpace *as) {
}

static _AddressSpace *cur_as = NULL;
void __am_get_cur_as(_Context *c) {
  c->as = cur_as;
}

void __am_switch(_Context *c) {
  if (vme_enable) {
    cur_as = c->as;
  }
}

void __am_tlb_refill(){
  //
  uint32_t pvn;
  __asm__ __volatile__ ("mfc0 $t0,$10;"
                        "sw   $t0,%0":"=m"(pvn)
                       );
  printf("pvn is %x\n",pvn);//debug
  return;
  // PDE* pgdir = (PDE*)cur_as->ptr;
  // uint32_t pgdir_idx = pvn >> 22 ;
  // assert(pgdir[pgdir_idx]&1);
  // PTE* pgtable = (PTE*)pgdir[pgdir_idx];
  // uint32_t 
}

int _map(_AddressSpace *as, void *va, void *pa, int prot) {
  assert((uint32_t)va % PGSIZE ==0 && (uint32_t)pa % PGSIZE ==0);
  uint32_t pdx = PDX(va);
  uint32_t ptx = PTX(va);
  PDE* pgdir = (PDE*)(as->ptr);
  PTE* pgtable = (PTE*)pgalloc_usr(1);
  pgdir[pdx] = ((PDE)pgtable | 1);
  pgtable[ptx] = ((PTE)pa | 1);
  return 0;
}

_Context *_ucontext(_AddressSpace *as, _Area ustack, _Area kstack, void *entry, void *args) {
  _Context* p = (_Context*)(ustack.end - 38*4);
  p->as = as;
  p->epc = (uint32_t)entry;
  p->gpr[29] = (uint32_t)p;
  return p;
}
