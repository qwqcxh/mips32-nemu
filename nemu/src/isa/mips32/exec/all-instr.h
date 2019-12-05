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