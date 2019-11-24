#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_shli(&s0, &id_src2->val, 16);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template2(lui);
}

/*******work***********/
make_EHelper(or) {
  rtl_or(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm_template3(or);
}