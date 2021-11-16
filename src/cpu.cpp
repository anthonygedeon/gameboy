#include <cstdint>
#include "cpu.h"
#include "register.h"

CPU::CPU(){}

bool CPU::readFlag(eFlag flag) {
    return readByte(m_regs[F]) & static_cast<uint8_t>(flag);
}

void CPU::setFlag(eFlag flag) {
    writeByte(m_regs[F], readByte(m_regs[F]) | static_cast<uint8_t>(flag));
}

void CPU::resetFlag(eFlag flag) {
    writeByte(m_regs[F], 
              readByte(m_regs[F]) & (0xFF ^ static_cast<uint8_t>(flag)));
}
