#include "cpu/exec.h"

make_EHelper(div){ //div rs,rt LO<-GPR[rs]/GPR[rt] HI<-GPR[rs]%GPR[rt]
    rtl_idiv_q(&cpu.lo,&id_src->val,&id_src2->val);
    rtl_idiv_r(&cpu.hi,&id_src->val,&id_src2->val);
    print_asm("div %s,%s",id_src->str,id_src2->str);
}

make_EHelper(divu){ //divu rs,rt
    rtl_div_q(&cpu.lo,&id_src->val,&id_src2->val);
    rtl_div_r(&cpu.hi,&id_src->val,&id_src2->val);
    print_asm("div %s,%s",id_src->str,id_src2->str);
}

make_EHelper(mfhi){ //mfhi rd   GPR[rd]<-HI
    rtl_mv(&reg_l(id_dest->reg),&cpu.hi);
    print_asm("mfhi %s",id_dest->str);
}

make_EHelper(mflo){ //mflo rd GPR[rd]<-LO
    rtl_mv(&reg_l(id_dest->reg),&cpu.lo);
    print_asm("mflo %s",id_dest->str);
}

make_EHelper(mul){ //mul rd,rs,rt
    rtl_mul_lo(&reg_l(id_dest->reg),&id_src->val,&id_src2->val);
    print_asm("mul %s,%s,%s",id_dest->str,id_src->str,id_src2->str);
}