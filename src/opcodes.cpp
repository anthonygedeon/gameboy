#include "cpu.h"
#include "opcodes.h"

std::array<Instruction, 256> lookup_table = {
    {"NOP", 4, nop},
    {"LD BC,%04x", 12, ld16},
    {"LD (BC),A", 8, ldPtr},
    {"INC BC", 8, inc16},
    {"INC B", 4, },
    {"DEC B", 4, },
    {"LD, B,%02x", 8, },
    {"RLCA", 4, rlca},
    {"LD (u16),SP", 20, },
    {"ADD HL,BC", 8, },
    {"LD A,(BC)", 8, },
    {"DEC BC", 8, },
    {"INC C", 4, },
    {"DEC C", 4, },
    {"LD C,%04x", 8, },
    {"RRCA", 4, rrca},
};

void nop(void* reg, uint16_t data){}

void ld8(void* reg, uint16_t data)
{
    Reg16* registr = reinterpret_cast<Reg16*>(reg);
    registr->word = data;
}

void ld16(void* reg, uint8_t data)
{
    Reg8* registr = reinterpret_cast<Reg8*>(reg);
    registr = data;
}

void ldPtr(uint16_t address, uint8_t data)
{
    m_mmu.write8(address, data);
}

void inc16(void* reg, uint16_t data)
{
    BC.word++;
}

/**
 * @brief
 *
 * @param reg
 * @param data
 */
void rlca(void* reg, uint16_t data)
{
    uint8_t msb = A >> 7;
    uint8_t rotate = (A << 1) | msb;
    A = rotate;
    setFlag(fC, msb);
    setFlag(fH, false);
    setFlag(fN, false);
    setFlag(fZ, false);
}

/**
 * @brief
 *
 * @param reg
 * @param data
 */
void rrca(void* reg, uint16_t data)
{
    uint8_t lsb = A << 7;
    uint8_t rotate = (A >> 1) | lsb;
    A = rotate;
    setFlag(fC, lsb);
    setFlag(fH, false);
    setFlag(fN, false);
    setFlag(fZ, false);
}