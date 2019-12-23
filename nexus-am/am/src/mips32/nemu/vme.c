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
  printf("pgdir is %x!!!\n",as->ptr); //debug
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
  __asm__ __volatile__ ("mfc0 $k0,$10;"
                        "sw   $k0,%0":"=m"(pvn)
                       );
  PDE* pgdir = (PDE*)cur_as->ptr;
  // printf("cur_as is %x and as->ptr is %x\n",cur_as,cur_as->ptr);
  uint32_t pdx = PDX(pvn) ;
  assert(pgdir[pdx]&1);
  PTE* pgtable = (PTE*)(pgdir[pdx] & 0xfffffffe);
  uint32_t ptxlo0 = PTX(pvn) & 0xfffffffe;
  uint32_t ptxlo1 = PTX(pvn) | 1;
  uint32_t ptx    = PTX(pvn); //for test
  uint32_t entrylo0 = pgtable[ptxlo0];
  uint32_t entrylo1 = pgtable[ptxlo1];
  uint32_t realpage = pgtable[ptx];//for test
  printf("TLB REFILL: entrylo0 %x entrylo1 %x miss pvn %x phsicalpage %x\n",entrylo0,entrylo1,pvn,realpage);//debug
  assert((entrylo0&1)||(entrylo1&1));
  __asm__ __volatile__ ("lw $k0,%0;"
                        "mtc0 $k0,$2;"
                        "lw $k1,%1;"
                        "mtc0 $k1,$3;"
                        "tlbwr"::"m"(entrylo0),"m"(entrylo1)
                       );
  return;
}

void __am_tlb_clear(){
  __asm__ __volatile__ ("syscall 2");
}

int _map(_AddressSpace *as, void *va, void *pa, int prot) {
  printf("_map as->ptr: %x va: %x pa: %x\n",as->ptr,va,pa);//debug
  assert((uint32_t)va % PGSIZE ==0 && (uint32_t)pa % PGSIZE ==0);
  if(as==NULL) as=cur_as;//not sure
  uint32_t pdx = PDX(va);
  uint32_t ptx = PTX(va);
  PDE* pgdir = (PDE*)(as->ptr);
  if(pgdir[pdx]&1){ //pgtable already exists
    PTE*pgtable  = (PTE*)((pgdir[pdx]>>12)<<12);
    if(pgtable[ptx]&1) return 1;//this va is mapped
    pgtable[ptx] = ((PTE)pa | 1);
  }
  else{
    PTE* pgtable = (PTE*)pgalloc_usr(1);
    pgdir[pdx] = ((PDE)pgtable | 1);
    pgtable[ptx] = ((PTE)pa | 1);
  }
  return 0;
}

_Context *_ucontext(_AddressSpace *as, _Area ustack, _Area kstack, void *entry, void *args) {
  _Context* p = (_Context*)(ustack.end - 4 - sizeof(_Context));
  p->as  = as;
  p->epc = (uint32_t)entry;
  p->gpr[29] = (uint32_t)p;
  return p;
}
