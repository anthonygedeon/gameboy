// Holds the CPU registers and interfaces with memory through the MMU

#ifndef CPU_H
#define CPU_H

#include <array>

#include "mmu.h"
#include "opcodes.h"

// 10 CPU registers in GB.
// The actual register array are defined in the cpp file to prevent
// linking issues.

using Reg8 = uint8_t;

/**
 * @brief
 *
 */
enum eFlag: uint8_t {
    fC=0x10, // 0b00010000
    fH=0x20, // 0b00100000
    fN=0x40, // 0b01000000
    fZ=0x80  // 0b10000000
};

class CPU {
// TODO: Not sure what to put in privates here, since I don't know how opcode access, etc. is going to work.
public:
    Reg16 AF, BC, DE, HL, SP, PC;
    Reg8& A = AF.low;
    Reg8& F = AF.high;
    Reg8& B = BC.low;
    Reg8& C = BC.high;
    Reg8& D = DE.low;
    Reg8& E = DE.high;
    Reg8& H = HL.low;
    Reg8& L = HL.high;
    MMU m_mmu;
    CPU();

    void cycle();
    void decode();
    uint8_t fetch8();
    uint16_t fetch16();

    bool readFlag(eFlag flag);
    void setFlag(eFlag flag, bool value);

    static std::array<Instruction, 256> lookup_table;
};

#endif
