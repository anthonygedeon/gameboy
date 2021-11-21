#include "register.h"

uint8_t readByte(Register reg) {
    return reg.m_data; 
}

void writeByte(Register& reg, uint8_t data) {
    reg.m_data = data;
}

uint16_t readWord(Register reg1, Register reg2) {
    return (static_cast<uint16_t>(reg1.m_data) << 8) ^ 
            static_cast<uint16_t>(reg2.m_data); 
}

void writeWord(Register& reg1, Register& reg2, uint16_t data) {
    reg1.m_data = static_cast<uint8_t>((0xFF00 & data) >> 8);
    reg2.m_data = static_cast<uint8_t>(0x00FF & data);
}
