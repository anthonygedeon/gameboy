// Provides registers consistent with GB's CPU model as well as the relevant
// functions to store and load from them (both 8-bit and 16-bit).

#ifndef REGISTERS_H 
#define REGISTERS_H

#include <cinttypes>

struct Register {
   uint8_t m_data;
};

typedef struct Register Register;

// There are 10 CPU registers to be used throughout the entire program.
// The actual registers are defined in the cc file to prevent linking issues.
enum Registers {
    A,B,C,D,E,F,H,L,SP,PC
};

uint8_t load8Bit(Register* reg);

void store8Bit(Register* reg, uint8_t data);

// 16 bit values are stored and written into two 8 bit registers.
uint16_t load16Bit(Register* reg1, Register* reg2);

void store16Bit(Register* reg1, Register* reg2, uint16_t data);

#endif
