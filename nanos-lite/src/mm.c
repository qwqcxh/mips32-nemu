#include "memory.h"
#include "am.h"

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  memset(p,0,nr_page*PGSIZE);
  printf("pf now is %x\n",pf);//debug
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk, intptr_t increment) {
  void* loaddr =(void*) (brk & (-PGSIZE));
  void* hiaddr =(void*) ((brk+increment+PGSIZE-1) & (-PGSIZE));
  for(void*va=loaddr;va<hiaddr;va+=PGSIZE){
    if(_map(NULL,va,0,0) == 0){ //try to map loaddr,it may already be mapped
      void* pa = new_page(1);
      _map(NULL,va,pa,0);
    }
  }
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
