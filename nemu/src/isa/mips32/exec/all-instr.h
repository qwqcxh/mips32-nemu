#include "cpu/exec.h"

make_EHelper(lui);

make_EHelper(ld);
make_EHelper(st);

make_EHelper(inv);
make_EHelper(nemu_trap);


/*************work******************/
make_EHelper(or);
make_EHelper(addiu);
make_EHelper(jal);
make_EHelper(jr);
make_EHelper(nop);
make_EHelper(addu);
make_EHelper(xor);
make_EHelper(sltiu);
make_EHelper(beq);
make_EHelper(bne);
make_EHelper(sltu);
make_EHelper(sra);
make_EHelper(lbu);
make_EHelper(andi);
make_EHelper(srlv);
make_EHelper(sllv);
make_EHelper(sb);
make_EHelper(xori);
make_EHelper(sh);
make_EHelper(nor);
make_EHelper(and);
make_EHelper(slt);
make_EHelper(div);
make_EHelper(mfhi);
make_EHelper(mflo);
make_EHelper(mul);
make_EHelper(slti);
make_EHelper(subu);
make_EHelper(movn);
make_EHelper(ori);
make_EHelper(multu);
make_EHelper(srl);
make_EHelper(sll);
make_EHelper(lh);
make_EHelper(lhu);
make_EHelper(lwl);
make_EHelper(lwr);
make_EHelper(mult);
make_EHelper(jalr);
make_EHelper(blez);
make_EHelper(srav);
make_EHelper(swl);
make_EHelper(swr);
make_EHelper(lb);
make_EHelper(j);
make_EHelper(divu);
make_EHelper(bgez);
make_EHelper(bltz);
make_EHelper(movz);
make_EHelper(bgtz);
make_EHelper(syscall);
make_EHelper(mfc0);
make_EHelper(mtc0);
make_EHelper(mtlo);
make_EHelper(mthi);
make_EHelper(cop0_func);