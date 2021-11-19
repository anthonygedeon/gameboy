// Provides 8/16-bit read/write to registers.

#ifndef REGISTER_H 
#define REGISTER_H

#include <cstdint>

struct Register {
   uint8_t m_data;
};

uint8_t readByte(Register reg);

void writeByte(Register& reg, uint8_t data);

// 16 bit values are writed and written into two 8 bit registers.
uint16_t readWord(Register reg1, Register reg2);

void writeWord(Register& reg1, Register& reg2, uint16_t data);

#endif
