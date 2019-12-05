#include "cpu/exec.h"

make_EHelper(ld) {
  rtl_lm(&s0, &id_src->addr, decinfo.width);
  rtl_sr(id_dest->reg, &s0, 4);

  switch (decinfo.width) {
    case 4: print_asm_template2(lw); break;
    case 2: print_asm_template2(lhu); break;
    case 1: print_asm_template2(lbu); break;
    default: assert(0);
  }
}

make_EHelper(st) {
  rtl_sm(&id_src->addr, &id_dest->val, decinfo.width);

  switch (decinfo.width) {
    case 4: print_asm_template2(sw); break;
    case 2: print_asm_template2(sh); break;
    case 1: print_asm_template2(sb); break;
    default: assert(0);
  }
}

make_EHelper(lbu) { //lbu rt,offset(rs)
  rtl_lm(&reg_l(id_dest->reg),&id_src->addr,decinfo.width);
  print_asm("lbu %s,%s",id_dest->str,id_src->str);
}

make_EHelper(lh){  //lh rt,offset(rs)
  rtl_lm(&s0,&id_src->addr,decinfo.width);
  rtl_sext(&reg_l(id_dest->reg),&s0,decinfo.width);
  print_asm("lh %s,%s",id_dest->str,id_src->str);
}

make_EHelper(lhu){ //lhu rt,offset(rs)
  rtl_lm(&reg_l(id_dest->reg),&id_src->addr,decinfo.width);
  print_asm("lhu %s,%s",id_dest->str,id_src->str);
}

make_EHelper(lwl){ //lwl rt,offset(rs)
  int bytes=(id_src->addr&3)+1;
  rtl_andi(&s0,&id_src->addr,0xfffffffc);
  rtl_lm(&s0,&s0,bytes);
  if(bytes==4) rtl_mv(&reg_l(id_dest->reg),&s0);
  else{
    rtl_shri(&reg_l(id_dest->reg),&reg_l(id_dest->reg),8*bytes);
    rtl_shli(&reg_l(id_dest->reg),&reg_l(id_dest->reg),8*bytes);
    rtl_or(&reg_l(id_dest->reg),&reg_l(id_dest->reg),&s0);
  }
  print_asm("lwl %s,%s",id_dest->str,id_src->str);
}

make_EHelper(lwr){ //lwr rt,offset(rs)
  int bytes=4-(id_src->val&3);
  rtl_lm(&s0,&id_src->addr,bytes);
  rtl_shli(&s0,&s0,8*(4-bytes));
  if(bytes==4) rtl_mv(&reg_l(id_dest->reg),&s0);
  else{
    rtl_shli(&reg_l(id_dest->reg),&reg_l(id_dest->reg),bytes*8);
    rtl_shri(&reg_l(id_dest->reg),&reg_l(id_dest->reg),bytes*8);
    rtl_or(&reg_l(id_dest->reg),&reg_l(id_dest->reg),&s0);
  }
  print_asm("lwr %s,%s",id_dest->str,id_src->str);
}

make_EHelper(sb){ //sb rt,offset(rs)
  rtl_sm(&id_src->addr,&id_dest->val,decinfo.width);
  print_asm("sb %s,%s",id_dest->str,id_src->str);
}

make_EHelper(sh){ //sh rt,offset(rs)
  rtl_sm(&id_src->addr,&id_dest->val,decinfo.width);
  print_asm("sh %s,%s",id_dest->str,id_src->str);
}

make_EHelper(swl){ //swl rt,offset(rs)
  int bytes=(id_src->addr&3)+1;
  rtl_shri(&s0,&id_dest->val,8*(4-bytes));
  rtl_andi(&s1,&id_src->addr,0xfffffffc);
  rtl_sm(&s1,&s0,bytes);
  print_asm("swl %s,%s",id_dest->str,id_src->str);
}

make_EHelper(swr){ //swr rt,offset(rs)
  int bytes=4-(id_src->addr&3);
  rtl_sm(&id_src->addr,&id_dest->val,bytes);
  print_asm("swr %s,%s",id_dest->str,id_src->str);
}