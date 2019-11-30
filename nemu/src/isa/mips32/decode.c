#include "cpu/decode.h"
#include "rtl/rtl.h"

// decode operand helper
#define make_DopHelper(name) void concat(decode_op_, name) (Operand *op, uint32_t val, bool load_val)

static inline make_DopHelper(i) { //decode_op_i(Oprand *op,uint32_t val,bool load_val)
  op->type = OP_TYPE_IMM;
  op->imm = val;
  rtl_li(&op->val, op->imm);

  print_Dop(op->str, OP_STR_SIZE, "%d", op->imm);
}

static inline make_DopHelper(r) { //decode_op_r(Oprand* op,uint32_t val,bool load_val)
  op->type = OP_TYPE_REG;
  op->reg = val;
  if (load_val) {
    rtl_lr(&op->val, op->reg, 4);
  }

  print_Dop(op->str, OP_STR_SIZE, "%s", reg_name(op->reg, 4));
}

make_DHelper(IU) { //lui rt,imm  GPR[rt]<- upper_imm|0_16
  decode_op_r(id_src, decinfo.isa.instr.rs, true);
  decode_op_i(id_src2, decinfo.isa.instr.imm, true);
  decode_op_r(id_dest, decinfo.isa.instr.rt, false);

  print_Dop(id_src2->str, OP_STR_SIZE, "0x%x", decinfo.isa.instr.imm);
}

static inline make_DHelper(addr) {
  decode_op_r(id_src, decinfo.isa.instr.rs, true);
  decode_op_i(id_src2, decinfo.isa.instr.simm, true);

  print_Dop(id_src->str, OP_STR_SIZE, "%d(%s)", id_src2->val, reg_name(id_src->reg, 4));

  rtl_add(&id_src->addr, &id_src->val, &id_src2->val);
}

make_DHelper(ld) {
  decode_addr(NULL);
  decode_op_r(id_dest, decinfo.isa.instr.rt, false);
}

make_DHelper(st) {
  decode_addr(NULL);
  decode_op_r(id_dest, decinfo.isa.instr.rt, true);
}

make_DHelper(or){ //OR rd,rs,rt    GPR[rd]<-GRP[rs] or GPR[rt]
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(addiu){ //addiu rt,rs,imm  GPR[rt]<-GPR[rs]+sign_ext_imm
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_i(id_src2,decinfo.isa.instr.imm,true);
  decode_op_r(id_dest,decinfo.isa.instr.rt,false);
}

make_DHelper(jal){ //jal target 
  decode_op_i(id_dest,decinfo.isa.instr.jmp_target,false);
}

make_DHelper(jr){ //jr rs
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
}

make_DHelper(addu){//addu rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(xor){ //xor rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);  
}

make_DHelper(sltiu){ //sltiu rt,rs,imm
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_i(id_src2,decinfo.isa.instr.imm,true);
  decode_op_r(id_dest,decinfo.isa.instr.rt,false);
}

make_DHelper(beq){ //beq rs,rt,offset
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_i(id_dest,decinfo.isa.instr.imm,true);
}

make_DHelper(bne){ //bne rs,rt,offset
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_i(id_dest,decinfo.isa.instr.imm,true);  
}

make_DHelper(sltu){ //sltu rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}