#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_shli(&s0, &id_src2->val, 16);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(lui);
}

/*******work***********/
make_EHelper(or) {
  rtl_or(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm_template3(or);
}

make_EHelper(addiu){ //addiu rt,rs,imm  GPR[rt] <- GPR[rs] + sext[imm]
  rtl_sext(&s0,&id_src2->val,2);
  rtl_add(&reg_l(id_dest->reg),&id_src->val,&s0);
  print_asm_template3(addiu);
}