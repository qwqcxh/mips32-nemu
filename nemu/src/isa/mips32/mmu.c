#include "nemu.h"
#include "rtl/rtl.h"

TLBentry TLB[TLBSIZE];
bool tlbmiss = false;
static inline paddr_t va2pa(vaddr_t addr, bool write) {
  //TODO
  if(addr >= 0x80000000) return addr;
  uint32_t pvn = PVN(addr);
  uint32_t pfn = -1;
  for(int i=0;i<TLBSIZE;i++){
    if((TLB[i].EntryHi &1 ) && pvn == PVN(TLB[i].EntryHi)){
      pfn = (addr >> 12)&1 ? PFN(TLB[i].EntryLo1) : PFN(TLB[i].EntryLo0);
    }
  }
  if(pfn!=-1){
    return pfn + (addr&0xfff);
  }
  else{
    tlbmiss = true;
    //yield
    rtl_li(&cpu.epc,cpu.pc - 4);
    rtl_andi(&cpu.cause,&cpu.cause,0xffffff83);
    rtl_andi(&cpu.status,&cpu.status,0xfffffffd);
    rtl_ori(&cpu.cause,&cpu.cause,0x8);
    rtl_ori(&cpu.status,&cpu.status,0x2);
    rtl_li(&cpu.entryhi,addr);
    rtl_j(0x80000000);
    return -1;
  }
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  uint32_t realaddr = va2pa(addr,false);
  if(tlbmiss) {tlbmiss = false; return 0;}
  else return paddr_read(realaddr, len);
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  uint32_t realaddr = va2pa(addr, true);
  if(tlbmiss) {tlbmiss = false; return ;}
  else paddr_write(realaddr, data, len);
}
