#include "cpu/exec.h"

make_EHelper(jal){
    rtl_li(&ir,cpu.pc+8);
    rtl_sr(31,&ir,4);//save pc+8 to $ra
    rtl_j(((cpu.pc+4)&0xf0000000)|(id_dest->val << 2));
    print_asm("jal  0x%x",cpu.pc);
}

make_EHelper(jr){
    rtl_jr(&id_src->val);
    print_asm("jr  %s",reg_name(id_src->reg,4));
}