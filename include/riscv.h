#ifndef DTLIB_RISCV_H
#define DTLIB_RISCV_H

#include <stdint.h>

#define DTLIB_OPCODE_LUI    0b0110111
#define DTLIB_OPCODE_AUIPC  0b0010111
#define DTLIB_OPCODE_OP     0b0110011
#define DTLIB_OPCODE_OP_IMM 0b0010011
#define DTLIB_OPCODE_JAL    0b1101111
#define DTLIB_OPCODE_JALR   0b1100111
#define DTLIB_OPCODE_BRANCH 0b1100011
#define DTLIB_OPCODE_LOAD   0b0000011
#define DTLIB_OPCODE_STORE  0b0100011

#define DTLIB_FUNC3_JALR    0b000
#define DTLIB_FUNC3_BEQ     0b000
#define DTLIB_FUNC3_BNE     0b001
#define DTLIB_FUNC3_BLT     0b100
#define DTLIB_FUNC3_BGE     0b101
#define DTLIB_FUNC3_BLTU    0b110
#define DTLIB_FUNC3_BGEU    0b111
#define DTLIB_FUNC3_LB      0b000
#define DTLIB_FUNC3_LH      0b001
#define DTLIB_FUNC3_LW      0b010
#define DTLIB_FUNC3_LBU     0b100
#define DTLIB_FUNC3_LHU     0b101
#define DTLIB_FUNC3_SB      0b000
#define DTLIB_FUNC3_SH      0b001
#define DTLIB_FUNC3_SW      0b010
#define DTLIB_FUNC3_ADD     0b000
#define DTLIB_FUNC3_SLT     0b010
#define DTLIB_FUNC3_SLTU    0b011
#define DTLIB_FUNC3_XOR     0b100
#define DTLIB_FUNC3_OR      0b110
#define DTLIB_FUNC3_AND     0b111
#define DTLIB_FUNC3_SLL     0b001
#define DTLIB_FUNC3_SRL     0b101

#define DTLIB_FUNC7_ADD     0b0000000
#define DTLIB_FUNC7_SUB     0b0100000
#define DTLIB_FUNC7_SRL     0b0000000
#define DTLIB_FUNC7_SRA     0b0100000

uint32_t sign_extend(uint32_t data, uint32_t nb_bits) {
  data &= (1 << nb_bits)-1;
  if((data >> (nb_bits-1)) & 0x1){
    data |= (((1 << (32 - (nb_bits-1))) - 1) << nb_bits);
  }
  return data;
}

uint32_t instr_r(uint32_t opcode, uint32_t rd, uint32_t func3, uint32_t rs1, uint32_t rs2, uint32_t func7) {
  uint32_t instr = 0;
  instr |= opcode & 0x7F;
  instr |= (rd & 0x1F) << 7;
  instr |= (func3 & 0x7) << 12;
  instr |= (rs1 & 0x1F) << 15;
  instr |= (rs2 & 0x1F) << 20;
  instr |= (func7 & 0x7F) << 25;
  return instr;
}

uint32_t instr_i(uint32_t opcode, uint32_t rd, uint32_t func3, uint32_t rs1, uint32_t imm) {
  uint32_t instr = 0;
  instr |= opcode & 0x7F;
  instr |= (rd & 0x1F) << 7;
  instr |= (func3 & 0x7) << 12;
  instr |= (rs1 & 0x1F) << 15;
  instr |= (imm & 0xFFF) << 20;
  return instr;
}

uint32_t instr_s(uint32_t opcode, uint32_t func3, uint32_t rs1, uint32_t rs2, uint32_t imm) {
  uint32_t instr = 0;
  instr |= opcode & 0x7F;
  instr |= (imm & 0x1F) << 7;
  instr |= (func3 & 0x7) << 12;
  instr |= (rs1 & 0x1F) << 15;
  instr |= (rs2 & 0x1F) << 20;
  instr |= ((imm >> 5) & 0x7F) << 25;
  return instr;
}

uint32_t instr_b(uint32_t opcode, uint32_t func3, uint32_t rs1, uint32_t rs2, uint32_t imm) {
  uint32_t instr = 0;
  instr |= opcode & 0x7F;
  instr |= ((imm >> 11) & 1) << 7;
  instr |= ((imm >> 1) & 0xF) << 8;
  instr |= (func3 & 0x7) << 12;
  instr |= (rs1 & 0x1F) << 15;
  instr |= (rs2 & 0x1F) << 20;
  instr |= ((imm >> 5) & 0x3F) << 25;
  instr |= ((imm >> 12) & 1) << 31;
  return instr;
}

uint32_t instr_u(uint32_t opcode, uint32_t rd, uint32_t imm) {
  uint32_t instr = 0;
  instr |= opcode & 0x7F;
  instr |= (rd & 0x1F) << 7;
  instr |= ((imm >> 12) & 0xFFFFF) << 12;
  return instr;
}

uint32_t instr_j(uint32_t opcode, uint32_t rd, uint32_t imm) {
  uint32_t instr = 0;
  instr |= opcode & 0x7F;
  instr |= (rd & 0x1F) << 7;
  instr |= ((imm >> 12) & 0xFF) << 12;
  instr |= ((imm >> 11) & 1) << 20;
  instr |= ((imm >> 1) & 0x3FF) << 21;
  instr |= ((imm >> 20) & 1) << 31;
  return instr;
}


void instr_lui(uint32_t rd, uint32_t imm) {
  return instr_u(DTLIB_OPCODE_LUI, rd, imm << 12);
  }

void instr_auipc(uint32_t rd, uint32_t imm) {
  return instr_u(DTLIB_OPCODE_AUIPC, rd, imm << 12);
}

void instr_jal(uint32_t rd, uint32_t imm) {
  return instr_j(DTLIB_OPCODE_JAL, rd, imm);
}

void instr_jalr(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_JALR, rd, DTLIB_FUNC3_JALR, rs1, imm);
}

void instr_beq(uint32_t rs1, uint32_t rs2, uint32_t imm) {
  return instr_b(DTLIB_OPCODE_BRANCH, DTLIB_FUNC3_BEQ, rs1, rs2, imm);
}

void instr_bne(uint32_t rs1, uint32_t rs2, uint32_t imm) {
  return instr_b(DTLIB_OPCODE_BRANCH, DTLIB_FUNC3_BNE, rs1, rs2, imm);
}

void instr_blt(uint32_t rs1, uint32_t rs2, uint32_t imm) {
  return instr_b(DTLIB_OPCODE_BRANCH, DTLIB_FUNC3_BLT, rs1, rs2, imm);
}

void instr_bge(uint32_t rs1, uint32_t rs2, uint32_t imm) {
  return instr_b(DTLIB_OPCODE_BRANCH, DTLIB_FUNC3_BGE, rs1, rs2, imm);
}

void instr_bltu(uint32_t rs1, uint32_t rs2, uint32_t imm) {
  return instr_b(DTLIB_OPCODE_BRANCH, DTLIB_FUNC3_BLTU, rs1, rs2, imm);
}

void instr_bgeu(uint32_t rs1, uint32_t rs2, uint32_t imm) {
  return instr_b(DTLIB_OPCODE_BRANCH, DTLIB_FUNC3_BGEU, rs1, rs2, imm);
}

void instr_lb(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_LOAD, rd, DTLIB_FUNC3_LB, rs1, imm);
}

void instr_lbu(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_LOAD, rd, DTLIB_FUNC3_LBU, rs1, imm);
}

void instr_lh(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_LOAD, rd, DTLIB_FUNC3_LH, rs1, imm);
}

void instr_lhu(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_LOAD, rd, DTLIB_FUNC3_LHU, rs1, imm);
}

void instr_lw(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_LOAD, rd, DTLIB_FUNC3_LW, rs1, imm);
}

void instr_sb(uint32_t rs1, uint32_t rs2, uint32_t imm) {
  return instr_s(DTLIB_OPCODE_STORE, DTLIB_FUNC3_SB, rs1, rs2, imm);
}

void instr_sh(uint32_t rs1, uint32_t rs2, uint32_t imm) {
  return instr_s(DTLIB_OPCODE_STORE, DTLIB_FUNC3_SH, rs1, rs2, imm);
}

void instr_sw(uint32_t rs1, uint32_t rs2, uint32_t imm) {
  return instr_s(DTLIB_OPCODE_STORE, DTLIB_FUNC3_SW, rs1, rs2, imm);
}

void instr_addi(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_OP_IMM, rd, DTLIB_FUNC3_ADD, rs1, imm);
}

void instr_slti(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_OP_IMM, rd, DTLIB_FUNC3_SLT, rs1, imm);
}

void instr_sltiu(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_OP_IMM, rd, DTLIB_FUNC3_SLTU, rs1, imm);
}

void instr_xori(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_OP_IMM, rd, DTLIB_FUNC3_XOR, rs1, imm);
}

void instr_ori(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_OP_IMM, rd, DTLIB_FUNC3_OR, rs1, imm);
}

void instr_andi(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_OP_IMM, rd, DTLIB_FUNC3_AND, rs1, imm);
}

void instr_slli(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return instr_i(DTLIB_OPCODE_OP_IMM, rd, DTLIB_FUNC3_SLL, rs1, imm);
}

void instr_srli(uint32_t rd, uint32_t rs1, uint32_t imm) {
  // set func7
  uint32_t imm_w_func7 = (imm & 0x1F) | ((1 << 6) << 25);
  return instr_i(DTLIB_OPCODE_OP_IMM, rd, DTLIB_FUNC3_SRL, rs1, imm_w_func7);
}

void instr_srai(uint32_t rd, uint32_t rs1, uint32_t imm) {
  // set func7
  uint32_t imm_w_func7 = (imm & 0x1F) | ((1 << 5) << 5);
  return instr_i(DTLIB_OPCODE_OP_IMM, rd, DTLIB_FUNC3_SRL, rs1, imm_w_func7);
}

void instr_add(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return instr_r(DTLIB_OPCODE_OP, rd, DTLIB_FUNC3_ADD, rs1, rs2, DTLIB_FUNC7_ADD);
}

void instr_sub(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return instr_r(DTLIB_OPCODE_OP, rd, DTLIB_FUNC3_ADD, rs1, rs2, DTLIB_FUNC7_SUB);
}

void instr_slt(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return instr_r(DTLIB_OPCODE_OP, rd, DTLIB_FUNC3_SLT, rs1, rs2, 0);
}

void instr_sltu(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return instr_r(DTLIB_OPCODE_OP, rd, DTLIB_FUNC3_SLTU, rs1, rs2, 0);
}

void instr_xor(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return instr_r(DTLIB_OPCODE_OP, rd, DTLIB_FUNC3_XOR, rs1, rs2, 0);
}

void instr_or(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return instr_r(DTLIB_OPCODE_OP, rd, DTLIB_FUNC3_OR, rs1, rs2, 0);
}

void instr_and(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return instr_r(DTLIB_OPCODE_OP, rd, DTLIB_FUNC3_AND, rs1, rs2, 0);
}

void instr_sll(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return instr_r(DTLIB_OPCODE_OP, rd, DTLIB_FUNC3_SLL, rs1, rs2, 0);
}

void instr_srl(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return instr_r(DTLIB_OPCODE_OP, rd, DTLIB_FUNC3_SRL, rs1, rs2, DTLIB_FUNC7_SRL);
}

void instr_sra(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return instr_r(DTLIB_OPCODE_OP, rd, DTLIB_FUNC3_SRL, rs1, rs2, DTLIB_FUNC7_SRA);
}

#endif
