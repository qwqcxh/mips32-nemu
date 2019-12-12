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

make_DHelper(lbu){  //lbu rt,offset(rs)
  decode_addr(NULL);
  decode_op_r(id_dest,decinfo.isa.instr.rt,false);
}

make_DHelper(lh){ //lh rt,offset(rs)
  decode_addr(NULL);
  decode_op_r(id_dest,decinfo.isa.instr.rt,false);
}

make_DHelper(lhu){ //lhu rt,offset(rs)
  decode_addr(NULL);
  decode_op_r(id_dest,decinfo.isa.instr.rt,false);
}

make_DHelper(lwl){ //lwl rt,offset(rs)
  decode_addr(NULL);
  decode_op_r(id_dest,decinfo.isa.instr.rt,true);
}

make_DHelper(lwr){ //lwr rt,offset(rs)
  decode_addr(NULL);
  decode_op_r(id_dest,decinfo.isa.instr.rt,true);
}

make_DHelper(sb){  //sb rt,offset(rs)
  decode_addr(NULL);
  decode_op_r(id_dest,decinfo.isa.instr.rt,true);
}

make_DHelper(sh){ //sh rt,offset(rs)
  decode_addr(NULL);
  decode_op_r(id_dest,decinfo.isa.instr.rt,true);
}

make_DHelper(or){ //or rd,rs,rt    GPR[rd]<-GRP[rs] or GPR[rt]
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

make_DHelper(jalr){ //jalr rd,rs
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
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

make_DHelper(slti){  //slti rt,rs,imm
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_i(id_src2,decinfo.isa.instr.simm,true);
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

make_DHelper(bgez){ //bgez rs,offset
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_i(id_dest,decinfo.isa.instr.imm,true);
}

make_DHelper(bltz){ //bltz rs,offset
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_i(id_dest,decinfo.isa.instr.imm,true);
}
make_DHelper(sltu){ //sltu rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(sra){ //sra rd,rt,sa
  decode_op_r(id_src,decinfo.isa.instr.rt,true);
  decode_op_i(id_src2,decinfo.isa.instr.sa,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(andi){ //andi rt,rs,imm
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_i(id_src2,decinfo.isa.instr.imm,true);
  decode_op_r(id_dest,decinfo.isa.instr.rt,false);
}

make_DHelper(srlv){ //srlv rd,rt,rs
  decode_op_r(id_src,decinfo.isa.instr.rt,true);
  decode_op_r(id_src2,decinfo.isa.instr.rs,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(sllv){ //sllv rd,rt,rs
  decode_op_r(id_src,decinfo.isa.instr.rt,true);
  decode_op_r(id_src2,decinfo.isa.instr.rs,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(srav){ //srav rd,rt,rs
  decode_op_r(id_src,decinfo.isa.instr.rt,true);
  decode_op_r(id_src2,decinfo.isa.instr.rs,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);  
}

make_DHelper(xori){ //xori rt,rs,imm
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_i(id_src2,decinfo.isa.instr.imm,true);
  decode_op_r(id_dest,decinfo.isa.instr.rt,false);
}

make_DHelper(nor){ //nor rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(and){ //and rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(slt){ //slt rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(div){ //div rs,rt  
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
}

make_DHelper(divu){ //div rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);  
}

make_DHelper(mfhi){ //mfhi rd
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(mflo){ //mflo rd
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(mul) { //mul rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(multu){ //multu rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
}

make_DHelper(mult){ //mult rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
}

make_DHelper(subu){ //subu rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false); 
}

make_DHelper(movn){ //movn rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,true);   
}

make_DHelper(movz) { //movz rd,rs,rt
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_r(id_src2,decinfo.isa.instr.rt,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,true);   
}

make_DHelper(ori){ //ori rt,rs,imm
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_i(id_src2,decinfo.isa.instr.imm,true);
  decode_op_r(id_dest,decinfo.isa.instr.rt,false);
}

make_DHelper(srl){ //srl rd,rt,sa
  decode_op_r(id_src,decinfo.isa.instr.rt,true);
  decode_op_i(id_src2,decinfo.isa.instr.sa,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
}

make_DHelper(sll){ //sll rd,rt,sa
  decode_op_r(id_src,decinfo.isa.instr.rt,true);
  decode_op_i(id_src2,decinfo.isa.instr.sa,true);
  decode_op_r(id_dest,decinfo.isa.instr.rd,false);  
}

make_DHelper(blez){ //blez rs,offset
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_i(id_src2,decinfo.isa.instr.imm,true);
}

make_DHelper(bgtz){ //bgtz rs,offset
  decode_op_r(id_src,decinfo.isa.instr.rs,true);
  decode_op_i(id_dest,decinfo.isa.instr.imm,true);
}

make_DHelper(swl){ //swl rt,offset(rs)
  decode_addr(NULL);
  decode_op_r(id_dest,decinfo.isa.instr.rt,true);
}

make_DHelper(swr){ //swr rt,offset(rs)
  decode_addr(NULL);
  decode_op_r(id_dest,decinfo.isa.instr.rt,true);
}

make_DHelper(lb){ //lb rt,offset(base)
  decode_addr(NULL);
  decode_op_r(id_dest,decinfo.isa.instr.rt,false);
}

make_DHelper(j){  //j target
  decode_op_i(id_dest,decinfo.isa.instr.jmp_target,true);
}

make_DHelper(mfc0){ //mfc0 rt,rd,sel
  decode_op_r(id_dest,decinfo.isa.instr.rt,false);
  decode_op_i(id_src,decinfo.isa.instr.rd,true);
  decode_op_i(id_src2,decinfo.isa.instr.sel,true);
  printf("mfc0 sel %d rd %d rt %d\n",id_src2->val,id_src->val,id_dest->val);//debug
}

make_DHelper(mtc0){ //mtc0 rt,rd,sel
  decode_op_i(id_src,decinfo.isa.instr.rd,true);
  decode_op_i(id_src2,decinfo.isa.instr.sel,true);
  decode_op_r(id_dest,decinfo.isa.instr.rt,true);
  printf("mtc0 sel %d rt %d rd %d\n",id_src2->val,id_src->val,id_dest->val); //debug
}