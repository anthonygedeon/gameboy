#include "registers.h"

Register _REG[10];

uint8_t load8Bit(Register* reg) {
    return reg->m_data; 
}

void store8Bit(Register* reg, uint8_t data) {
    reg->m_data = data;
}

uint16_t load16Bit(Register* reg1, Register* reg2) {
    return (static_cast<uint16_t>(reg1->m_data) << 8) ^ 
            static_cast<uint16_t>(reg2->m_data); 
}

void store16Bit(Register* reg1, Register* reg2, uint16_t data) {
    reg1->m_data = static_cast<uint8_t>((0xFF00 & data) >> 8);
    reg2->m_data = static_cast<uint8_t>(0x00FF & data);
}
