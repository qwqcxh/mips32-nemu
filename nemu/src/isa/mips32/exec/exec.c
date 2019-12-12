#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry special_table [64] = {
  /* b000 */ IDEX(sll,sll), EMPTY, IDEX(srl,srl), IDEX(sra,sra), IDEX(sllv,sllv), EMPTY, IDEX(srlv,srlv), IDEX(srav,srav),
  /* b001 */ IDEX(jr,jr), IDEX(jalr,jalr), IDEX(movz,movz), IDEX(movn,movn), IDEX(syscall,syscall), EMPTY, EMPTY, EMPTY,
  /* b010 */ IDEX(mfhi,mfhi), EMPTY, IDEX(mflo,mflo), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b011 */ IDEX(mult,mult), IDEX(multu,multu), IDEX(div,div), IDEX(divu,divu), EMPTY, EMPTY, EMPTY, EMPTY,
  /* b100 */ EMPTY, IDEX(addu,addu), EMPTY, IDEX(subu,subu), IDEX(and,and), IDEX(or,or), IDEX(xor,xor), IDEX(nor,nor),
  /* b101 */ EMPTY, EMPTY, IDEX(slt,slt), IDEX(sltu,sltu), EMPTY, EMPTY, EMPTY, EMPTY,
  /* b110 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b111 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
};

static OpcodeEntry special_table2 [64] = {
  /* b000 */ EMPTY, EMPTY, IDEX(mul,mul), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b001 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b010 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b011 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b100 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b101 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b110 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b111 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  
};

static OpcodeEntry regimm_table[32] = {
  /* b000 */ IDEX(bltz,bltz),IDEX(bgez,bgez),EMPTY,EMPTY,
  /* b001 */ EMPTY,EMPTY,EMPTY,EMPTY,
  /* b010 */ EMPTY,EMPTY,EMPTY,EMPTY,
  /* b011 */ EMPTY,EMPTY,EMPTY,EMPTY,
  /* b100 */ EMPTY,EMPTY,EMPTY,EMPTY,
  /* b101 */ EMPTY,EMPTY,EMPTY,EMPTY,
  /* b110 */ EMPTY,EMPTY,EMPTY,EMPTY,
  /* b111 */ EMPTY,EMPTY,EMPTY,EMPTY,
};

static make_EHelper(special) {
  idex(pc, &special_table[decinfo.isa.instr.func]);
}
static make_EHelper(regimm) {
  idex(pc, &regimm_table[decinfo.isa.instr.rt]);
}

static make_EHelper(special2){
  idex(pc, &special_table2[decinfo.isa.instr.func]);
}

static OpcodeEntry opcode_table [64] = {
  /* b000 */ EX(special), EX(regimm), IDEX(j,j), IDEX(jal,jal), IDEX(beq,beq), IDEX(bne,bne), IDEX(blez,blez), IDEX(bgtz,bgtz),
  /* b001 */ EMPTY, IDEX(addiu,addiu), IDEX(slti,slti), IDEX(sltiu,sltiu), IDEX(andi,andi), IDEX(ori,ori), IDEX(xori,xori), IDEX(IU, lui),
  /* b010 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b011 */ EMPTY, EMPTY, EMPTY, EMPTY,EX(special2), EMPTY, EMPTY, EMPTY,
  /* b100 */ IDEXW(lb,lb,1), IDEXW(lh,lh,2), IDEX(lwl,lwl), IDEXW(ld, ld, 4), IDEXW(lbu,lbu,1), IDEXW(lhu,lhu,2), IDEX(lwr,lwr), EMPTY,
  /* b101 */ IDEXW(sb,sb,1), IDEXW(sh,sh,2), IDEX(swl,swl), IDEXW(st, st, 4), EMPTY, EMPTY, IDEX(swr,swr), EMPTY,
  /* b110 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b111 */ EMPTY, EMPTY, EMPTY, EMPTY, EX(nemu_trap), EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  decinfo.width = opcode_table[decinfo.isa.instr.opcode].width;
  idex(pc, &opcode_table[decinfo.isa.instr.opcode]);
}
