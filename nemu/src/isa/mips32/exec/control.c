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

make_EHelper(beq){ //beq rs,rt,offset
    rtl_sext(&id_dest->val,&id_dest->val,2);
    rtl_shli(&id_dest->val,&id_dest->val,2);//get offset
    rtl_addi(&id_dest->val,&id_dest->val,cpu.pc+8);
    rtl_jrelop(RELOP_EQ,&id_src->val,&id_src2->val,id_dest->val);
    print_asm("beq %s,%s,0x%x",id_src->str,id_src2->str,id_dest->val);
}