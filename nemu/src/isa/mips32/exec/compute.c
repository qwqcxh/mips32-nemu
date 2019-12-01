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

make_EHelper(addu){ //addu rd,rs,rt GPR[rd]<-GPR[rs]+GRP[rt]
  rtl_add(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm("addu %s,%s,%s",id_dest->str,id_src->str,id_src2->str);
}

make_EHelper(xor){ //xor rd,rs,rt GPR[rd]<-GPR[rs]+GPR[rt]
  rtl_xor(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm("xor %s,%s,%s",id_dest->str,id_src->str,id_src2->str);
}

make_EHelper(sltiu){ //sltiu rt,rs,imm  GPR[rt]<-(GPR[rs]<imm)
  rtl_sext(&id_src2->val,&id_src2->val,2);
  rtl_setrelop(RELOP_LTU,&s0,&id_src->val,&id_src2->val);
  rtl_li(&ir,1);
  rtl_li(&id_dest->val,0);
  rtl_mux(&reg_l(id_dest->reg),&s0,&ir,&id_dest->val);
  print_asm("sltiu %s,%s,%u",id_dest->str,id_src->str,id_src2->val);
}

make_EHelper(sltu){ //sltu rd,rs,rt GPR[rd]<-(GPR[rs]<GPR[rt])
  rtl_setrelop(RELOP_LTU,&s0,&id_src->val,&id_src2->val);
  rtl_li(&ir,1);
  rtl_li(&id_dest->val,0);
  rtl_mux(&reg_l(id_dest->reg),&s0,&ir,&id_dest->val);
  print_asm("sltu %s,%s,%s",id_dest->str,id_src->str,id_src2->str); 
}

make_EHelper(sra){ //sra rd,rt,sa  GPR[rd]<-(GPR[rt]>> sa)
  rtl_sar(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm("sra %s,%s,0x%x",id_dest->str,id_src->str,id_src2->val);
}

make_EHelper(andi){ //andi rt,rs,imm GPR[rt]<-(GPR[rs] and zero-extended(imm))
  rtl_and(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm("andi %s,%s,0x%x",id_dest->str,id_src->str,id_src2->val);
}

make_EHelper(srlv){ //srlv rd,rt,rs 
  rtl_andi(&id_src2->val,&id_src2->val,0x1f);
  rtl_shr(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm("srlv %s,%s,%s",id_dest->str,id_src->str,id_src2->str);
}

make_EHelper(sllv){ //sllv rd,rt,rs
  rtl_andi(&id_src2->val,&id_src2->val,0x1f);
  rtl_shl(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm("sllv %s,%s,%s",id_dest->str,id_src->str,id_src2->str);
}

make_EHelper(xori){ //xori rt,rs,imm
  rtl_xor(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm("xori %s,%s,0x%x",id_dest->str,id_src->str,id_src2->val);
}

make_EHelper(nor){ //nor rd,rs,rt
  rtl_or(&s0,&id_src->val,&id_src2->val);
  rtl_not(&reg_l(id_dest->reg),&s0);
  print_asm("nor %s,%s,%s",id_dest->str,id_src->str,id_src2->str);
}

make_EHelper(and){ //and rd,rs,rt
  rtl_and(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
  print_asm("and %s,%s,%s",id_dest->str,id_src->str,id_src2->str);
}

make_EHelper(slt){ //slt rd,rs,rt
  rtl_setrelop(RELOP_LT,&s0,&id_src->val,&id_src2->val);
  rtl_li(&ir,1);
  rtl_li(&id_dest->val,0);
  rtl_mux(&reg_l(id_dest->reg),&s0,&ir,&id_dest->val);
  print_asm("slt %s,%s,%s",id_dest->str,id_src->str,id_src2->str);
}