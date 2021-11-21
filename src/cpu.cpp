#include <cstdint>
#include "cpu.h"

/**
 * @brief Construct a new CPU::CPU object
 *
 */
CPU::CPU(){}

/**
 * @brief
 *
 */
void CPU::cycle()
{
}

void CPU::decode()
{
    uint8_t opcode = fetch8();
    lookup_table[opcode];
    switch (opcode)
    {
   case 0x01:
        ld(BC, fetch16());
        break;
    case 0x02:
        ldPtr(BC.word, A);
    case 0x03:
       BC.word++;
       break;
    case 0x04:
        setFlag(fH, (B & 0x0F) == 0xF);
        B++;
        setFlag(fZ, B == 0);
        setFlag(fN, false);
        break;
    case 0x05:
        setFlag(fH, (B & 0x0F) == 0x0);
        B--;
        setFlag(fZ, B == 0);
        setFlag(fN, true);
        break;
    case 0x06:
        ld(B, fetch8());
        break;
    case 0x08:
        m_mmu.write16(fetch16(), SP.word);
        break;
    case 0x09:
        setFlag(fH, ((HL.word & 0x0FFF) + (BC.word & 0x0FFF) == 0x1000));
        setFlag(fC, (HL.word + BC.word) & 0x10000);
        HL.word += BC.word;
        break;
    case 0x0A:
        A = m_mmu.read8(BC.word);
        break;
    case 0x0B:
        BC.word--;
        break;
    case 0x0C:
        setFlag(fH, (C & 0x0F) == 0xF);
        C++;
        setFlag(fZ, C == 0);
        setFlag(fN, false);
        break;
    case 0x0D:
        setFlag(fH, (C & 0x0F) == 0x0);
        C--;
        setFlag(fZ, C == 0);
        setFlag(fN, true);
        break;
    case 0x0E:
        ld(C, fetch8());
        break;
    default:
        break;
    }
}


/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t CPU::fetch8()
{
   return m_mmu.read8(PC.word++);
}

uint16_t CPU::fetch16()
{
    uint16_t temp = m_mmu.read16(PC.word);
    PC.word += 2;
    return temp;
}

/// readFlag returns a particular bit from F.
bool CPU::readFlag(eFlag flag) {
    /*
        reading the flag

        F = 0b01010000
        flag = 0b01000000

        Example:

        0b01010010
        0b01000000
        &
    =   0b01000000
        ob01010000 = original

    */
    return F & flag;
}

/// Take F and OR it with the flag value.
/// Results in a bit being set.
void CPU::setFlag(eFlag flag, bool value) {
    /*
        setting the flag

        F =    0b01010010
        flag = 0b00100000

        Example:

        0b01010010
        0b00100000
        |
    =   0b01110010
        0b01010010 = original F register
    */
   F = (value) ? F | flag : F & ~flag;
}