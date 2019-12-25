#include "nemu.h"
#include "rtl/rtl.h"

TLBentry TLB[TLBSIZE];
bool tlbmiss = false;
static inline paddr_t va2pa(vaddr_t addr, bool write) {
  //TODO
  if(addr >= 0x80000000) return addr;
  uint32_t pvn = PVN(addr);
  // printf("***************BEGIN*************\n");//debug
  // printf("%s : addr %x cpu.pc %x pvn %x\n",__FUNCTION__,addr,cpu.pc,pvn);//debug
  uint32_t pfn = -1;
  for(int i=0;i<TLBSIZE;i++){
    // printf("TLB[%d] pvn is %x\n",i,PVN(TLB[i].EntryHi));//debug
    if((TLB[i].EntryHi &1 ) && pvn == PVN(TLB[i].EntryHi)){
      pfn = (addr >> 12)&1 ? PFN(TLB[i].EntryLo1) : PFN(TLB[i].EntryLo0);
    }
  }
  if(pfn!=-1 && pfn!=0 ){
    return (pfn<<12) + (addr&0xfff);
  }
  else{
    tlbmiss = true;
    //trap to kernel
    assert(cpu.pc < 0x80000000);
    rtl_mv(&cpu.epc,&cpu.pc);
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
  if(realaddr == (uint32_t)-1) printf("%s :realaddr of addr %x miss\n",__FUNCTION__,addr);//debug
  else if(addr < 0x80000000 && cpu.pc == 0x80000000) {
    printf("%s :realaddr of addr %x is %x value is %x\n",__FUNCTION__,addr,realaddr,paddr_read(realaddr,len));//debug
    printf("****************END*****************\n");//debug
  }
  if(tlbmiss) {printf("miss addr %x at cpu.pc %x\n",addr,cpu.pc);return 0;}//tlbmiss should be set to false in rtl_lm
  else return paddr_read(realaddr, len);
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  uint32_t realaddr = va2pa(addr, true);
  if(realaddr == (uint32_t)-1) printf("%s :realaddr of addr %x miss\n",__FUNCTION__,addr);//debug
  else if(addr < 0x80000000 && cpu.pc == 0x80000000) {
    printf("%s :realaddr of addr %x is %x write_data %x\n",__FUNCTION__,addr,realaddr,data);//debug
    printf("****************END*****************\n");//debug
  }
  if(tlbmiss) {tlbmiss = false;printf("miss addr %x at cpu.pc %x\n",addr,cpu.pc); return ;}
  else paddr_write(realaddr, data, len);
}
