#include "cpu/exec.h"
#include "nemu.h"

extern TLBentry TLB[TLBSIZE];
make_EHelper(syscall){
    switch(decinfo.isa.instr.val>>6){
        case 1: //yield should mov pc+4 to epc and eret to epc
            rtl_li(&cpu.epc,cpu.pc+4);
            rtl_andi(&cpu.cause,&cpu.cause,0xffffff83);
            rtl_andi(&cpu.status,&cpu.status,0xfffffffd);        
            rtl_ori(&cpu.cause,&cpu.cause,0x34);
            rtl_ori(&cpu.status,&cpu.status,0x2);
            break;
        case 2: //tlb clear
            for(int i=0;i<TLBSIZE;i++)
                TLB[i].EntryHi = TLB[i].EntryLo0 = TLB[i].EntryLo1 = 0;
            printf("tlb is cleared\n");//debug
            break;
        default: //syscall should mov pc to epc and eret to epc+4
            rtl_mv(&cpu.epc,&cpu.pc);
            rtl_andi(&cpu.cause,&cpu.cause,0xffffff83);
            rtl_andi(&cpu.status,&cpu.status,0xfffffffd);          
            rtl_ori(&cpu.cause,&cpu.cause,0x20);//set cause.execode
            rtl_ori(&cpu.status,&cpu.status,0x2);
    }
    rtl_j(0x80000180);
}

make_EHelper(mfc0){
    switch(id_src->val){
        // case 8:rtl_mv(&reg_l(id_dest->reg),&cpu.badvaddr);break;
        case 10:rtl_mv(&reg_l(id_dest->reg),&cpu.entryhi);break;
        case 12:rtl_mv(&reg_l(id_dest->reg),&cpu.status);break;
        case 13:rtl_mv(&reg_l(id_dest->reg),&cpu.cause);break;
        case 14:rtl_mv(&reg_l(id_dest->reg),&cpu.epc);break;
        default: assert(0);break;
    }
}

make_EHelper(mtc0){
    switch(id_src->val){
        // case 8:rtl_mv(&cpu.badvaddr,&reg_l(id_dest->reg));break;
        case 2 :rtl_mv(&cpu.entrylo0,&reg_l(id_dest->reg));break;
        case 3 :rtl_mv(&cpu.entrylo1,&reg_l(id_dest->reg));break;
        case 12:rtl_mv(&cpu.status,&reg_l(id_dest->reg));break;
        case 13:rtl_mv(&cpu.cause,&reg_l(id_dest->reg));break;
        case 14:rtl_mv(&cpu.epc,&reg_l(id_dest->reg));break;
        default: assert(0);break;
    }
}

int randidx = TLBSIZE - 1;
make_EHelper(cop0_func){
    switch(decinfo.isa.instr.func){
        case 0x6 : {//TLBWR
            int idx=-1;
            for(int i=0;i<TLBSIZE;i++) if((TLB[i].EntryHi & 1) == 0) {idx = i;break;}
            if(idx == -1) {idx=randidx;randidx = (randidx -1 + TLBSIZE)%TLBSIZE;}
            TLB[idx].EntryHi  = cpu.entryhi | 1;
            TLB[idx].EntryLo0 = cpu.entrylo0;
            TLB[idx].EntryLo1 = cpu.entrylo1;
            break;
        }
        case 0x18: //ERET
            rtl_andi(&cpu.status,&cpu.status,0xfffffffd);
            uint32_t ex_code = (cpu.cause>>2)&0x1f;
            switch (ex_code) {
                case 2:  rtl_j(cpu.epc);break;
                case 8:  rtl_j(cpu.epc+4);break;//syscall
                case 13: rtl_j(cpu.epc); break; //YIELD
                default: assert(0);
            }
            break;
        default: assert(0);
    }
}