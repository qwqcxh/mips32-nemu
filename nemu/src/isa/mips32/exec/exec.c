#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry special_table [64] = {
  /* b000 */ EX(nop), EMPTY, EMPTY, IDEX(sra,sra), IDEX(sllv,sllv), EMPTY, IDEX(srlv,srlv), EMPTY,
  /* b001 */ IDEX(jr,jr), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b010 */ IDEX(mfhi,mfhi), EMPTY, IDEX(mflo,mflo), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b011 */ EMPTY, EMPTY, IDEX(div,div), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b100 */ EMPTY, IDEX(addu,addu), EMPTY, EMPTY, IDEX(and,and), IDEX(or,or), IDEX(xor,xor), IDEX(nor,nor),
  /* b101 */ EMPTY, EMPTY, IDEX(slt,slt), IDEX(sltu,sltu), EMPTY, EMPTY, EMPTY, EMPTY,
  /* b110 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b111 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
};

static make_EHelper(special) {
  idex(pc, &special_table[decinfo.isa.instr.func]);
}

static OpcodeEntry opcode_table [64] = {
  /* b000 */ EX(special), EMPTY, EMPTY, IDEX(jal,jal), IDEX(beq,beq), IDEX(bne,bne), EMPTY, EMPTY,
  /* b001 */ EMPTY, IDEX(addiu,addiu), IDEX(slti,slti), IDEX(sltiu,sltiu), IDEX(andi,andi), EMPTY, IDEX(xori,xori), IDEX(IU, lui),
  /* b010 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b011 */ EMPTY, EMPTY, EMPTY, EMPTY, IDEX(mul,mul), EMPTY, EMPTY, EMPTY,
  /* b100 */ EMPTY, EMPTY, EMPTY, IDEXW(ld, ld, 4), IDEXW(lbu,lbu,1), EMPTY, EMPTY, EMPTY,
  /* b101 */ IDEXW(sb,sb,1), IDEXW(sh,sh,2), EMPTY, IDEXW(st, st, 4), EMPTY, EMPTY, EMPTY, EMPTY,
  /* b110 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b111 */ EMPTY, EMPTY, EMPTY, EMPTY, EX(nemu_trap), EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  decinfo.width = opcode_table[decinfo.isa.instr.opcode].width;
  idex(pc, &opcode_table[decinfo.isa.instr.opcode]);
}
