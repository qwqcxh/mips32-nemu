#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_shli(&s0, &id_src2->val, 16);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm("lui  %s,%s",id_dest->str,id_src2->str);
}

/*******work***********/
make_EHelper(or) { //or rd,rs,rt
  rtl_or(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm("or  %s,%s,%s",id_dest->str,id_src->str,id_src2->str);
}

make_EHelper(addiu){ //addiu rt,rs,imm  GPR[rt] <- GPR[rs] + sext[imm]
  rtl_sext(&s0,&id_src2->val,2);
  rtl_add(&reg_l(id_dest->reg),&id_src->val,&s0);
  print_asm("addiu  %s,%s,%d",id_dest->str,id_src->str,s0);
}