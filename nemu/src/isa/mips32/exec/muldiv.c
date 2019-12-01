#include "cpu/exec.h"

make_EHelper(div){ //div rs,rt LO<-GPR[rs]/GPR[rt] HI<-GPR[rs]%GPR[rt]
    rtl_idiv_q(&lo,&id_src->val,&id_src2->val);
    rtl_idiv_r(&hi,&id_src->val,&id_src2->val);
}

make_EHelper(mfhi){ //mfhi rd   GPR[rd]<-HI
    rtl_mv(&reg_l(id_dest->reg),&hi);
    print_asm("mfhi %s",id_dest->str);
}

make_EHelper(mflo){ //mflo rd GPR[rd]<-LO
    rtl_mv(&reg_l(id_dest->reg),&lo);
    print_asm("mflo %s",id_dest->str);
}