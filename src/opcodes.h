#pragma once

#include <string>
#include <functional>

struct Instruction
{
    std::string name;
    int cpu_cycle;
    std::function<void(void*, uint16_t)> instruction;
};

void nop(void* reg, uint16_t data);

// Load Instructions
void ld16(void* reg, uint16_t data);
void ld8(void* reg, uint16_t data);
void ldAr16(void* reg, uint16_t data);
void ldPtr(void* reg, uint16_t data);

// 8-bit Arithmetic & Logic Instructions
void inc8(void* reg, uint16_t data);
void dec8(void* reg, uint16_t data);

// 16-bit Arithmetic Instructions
void inc16(void* reg, uint16_t data);
void addHLr16(void* reg, uint16_t data);
void decR16(void* reg, uint8_t data);

// Bit Shift Instructions
void rlca(void* reg, uint16_t data);
void rrca(void* reg, uint16_t data);
