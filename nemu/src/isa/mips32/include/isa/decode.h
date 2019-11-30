#ifndef __MIPS32_DECODE_H__
#define __MIPS32_DECODE_H__

typedef union {
  struct {
    int32_t  simm   : 16;
//  uint32_t rt     :  5;
//  uint32_t rs     :  5;
//  uint32_t opcode :  6;
  };
  struct {
    uint32_t imm    : 16;
//  uint32_t rt     :  5;
//  uint32_t rs     :  5;
//  uint32_t opcode :  6;
  };
  struct {
    uint32_t jmp_target : 26;
    uint32_t opcode     :  6;
  };
  struct {
    uint32_t func   : 6;
    uint32_t sa     : 5;
    uint32_t rd     : 5;
    uint32_t rt     : 5;
    uint32_t rs     : 5;
//  uint32_t opcode : 6;
  };
  uint32_t val;
} Instr;


struct ISADecodeInfo {
  Instr instr;
};

make_DHelper(IU);
make_DHelper(ld);
make_DHelper(st);
/*******work*************/
make_DHelper(or);
make_DHelper(addiu);
make_DHelper(jal);
make_DHelper(jr);
make_DHelper(addu);
make_DHelper(xor);
make_DHelper(sltiu);
make_DHelper(beq);
make_DHelper(bne);
make_DHelper(sltu);
#endif
