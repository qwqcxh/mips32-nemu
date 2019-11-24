#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry special_table [64] = {
  /* b000 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b001 */ IDEX(jr,jr), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b010 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b011 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b100 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, IDEX(or,or), EMPTY, EMPTY,
  /* b101 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b110 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b111 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
};

static make_EHelper(special) {
  idex(pc, &special_table[decinfo.isa.instr.func]);
}

static OpcodeEntry opcode_table [64] = {
  /* b000 */ EX(special), EMPTY, EMPTY, IDEX(jal,jal), EMPTY, EMPTY, EMPTY, EMPTY,
  /* b001 */ EMPTY, IDEX(addiu,addiu), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, IDEX(IU, lui),
  /* b010 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b011 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b100 */ EMPTY, EMPTY, EMPTY, IDEXW(ld, ld, 4), EMPTY, EMPTY, EMPTY, EMPTY,
  /* b101 */ EMPTY, EMPTY, EMPTY, IDEXW(st, st, 4), EMPTY, EMPTY, EMPTY, EMPTY,
  /* b110 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b111 */ EMPTY, EMPTY, EMPTY, EMPTY, EX(nemu_trap), EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  //print instrtions value to debug
  printf("current ins is 0x%x with opcode 0x%x\n",decinfo.isa.instr.val,decinfo.isa.instr.opcode);//debug
  decinfo.width = opcode_table[decinfo.isa.instr.opcode].width;
  idex(pc, &opcode_table[decinfo.isa.instr.opcode]);
}
