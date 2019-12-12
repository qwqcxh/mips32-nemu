#include "cpu/exec.h"

make_EHelper(syscall){
    rtl_mv(&cpu.epc,&cpu.pc);
    rtl_andi(&cpu.cause,&cpu.cause,0xffffff83);
    rtl_andi(&cpu.status,&cpu.status,0xfffffffd);
    switch(decinfo.isa.instr.val>>6){
        case 1:
            rtl_ori(&cpu.cause,&cpu.cause,0x34);
            rtl_ori(&cpu.status,&cpu.status,0x2);
            break;
        default:
            rtl_ori(&cpu.cause,&cpu.cause,0x20);//set cause.execode
            rtl_ori(&cpu.status,&cpu.status,0x2);
    }
    rtl_j(0x80000180);
}

make_EHelper(mfc0){
    switch(id_src->val){
        case 8:rtl_mv(&reg_l(id_dest->reg),&cpu.badvaddr);break;
        case 12:rtl_mv(&reg_l(id_dest->reg),&cpu.status);break;
        case 13:rtl_mv(&reg_l(id_dest->reg),&cpu.cause);break;
        case 14:rtl_mv(&reg_l(id_dest->reg),&cpu.epc);break;
        default: assert(0);break;
    }
}

make_EHelper(mtc0){
    switch(id_src->val){
        case 8:rtl_mv(&cpu.badvaddr,&reg_l(id_dest->reg));break;
        case 12:rtl_mv(&cpu.status,&reg_l(id_dest->reg));break;
        case 13:rtl_mv(&cpu.cause,&reg_l(id_dest->reg));break;
        case 14:rtl_mv(&cpu.epc,&reg_l(id_dest->reg));break;
        default: assert(0);break;
    }
}

make_EHelper(cop0_func){
    switch(decinfo.isa.instr.func){
        case 0x18: //ERET
            rtl_andi(&cpu.status,&cpu.status,0xfffffffd);
            rtl_j(cpu.epc+4);
            break;
        default: assert(0);
    }
}