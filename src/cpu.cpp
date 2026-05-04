#include "./include/cpu.hpp"

CPU::CPU() 
    : A(0), F(0), B(0), C(0), D(0), E(0), H(0), L(0), SP(0), PC(0), 
      halted(false), ime(false) {
    // Initialize all registers to 0
}

void CPU::reset() {
    A = F = B = C = D = E = H = L = 0;
    SP = PC = 0;
    halted = false;
    ime = false;
}

// ============================================================================
// EXECUTION
// ============================================================================

void CPU::step() {
    if (halted) {
        return; // TODO: Wake on interrupt
    }
    
    uint8_t opcode = readByte(PC++);
    executeInstruction(opcode);
}

void CPU::executeInstruction(uint8_t opcode) {
    // Decode based on bit pattern
    uint8_t x = (opcode >> 6) & 0x03;  // Bits 7-6
    uint8_t y = (opcode >> 3) & 0x07;  // Bits 5-3
    uint8_t z = opcode & 0x07;         // Bits 2-0
    
    switch (x) {
        case 0: // Block 0
            switch (z) {
                case 0: // Misc
                    switch (y) {
                        case 0: NOP(); break;
                        case 1: LD_r16_imm16(SP); break; // LD [imm16], SP - not fully implemented
                        case 2: STOP(); break;
                        case 3: JR_imm8(); break;
                        case 4: case 5: case 6: case 7: 
                            JR_cond_imm8(y == 4 ? !getFlagZ() : y == 5 ? getFlagZ() : y == 6 ? !getFlagC() : getFlagC());
                            break;
                    }
                    break;
                    
                case 1: // LD r16, imm16 or ADD HL, r16
                    if (y & 1) { // Odd y = ADD HL, r16
                        uint16_t val = 0;
                        switch (y >> 1) {
                            case 0: val = getBC(); break;
                            case 1: val = getDE(); break;
                            case 2: val = getHL(); break;
                            case 3: val = SP; break;
                        }
                        ADD_HL_r16(val);
                    } else { // Even y = LD r16, imm16
                        switch (y >> 1) {
                            case 0: LD_r16_imm16(reinterpret_cast<uint16_t&>(B)); break; // BC
                            case 1: LD_r16_imm16(reinterpret_cast<uint16_t&>(D)); break; // DE
                            case 2: LD_r16_imm16(reinterpret_cast<uint16_t&>(H)); break; // HL
                            case 3: LD_r16_imm16(SP); break;
                        }
                    }
                    break;
                    
                case 2: // LD [r16mem], A or LD A, [r16mem]
                    {
                        uint16_t addr = 0;
                        switch (y >> 1) {
                            case 0: addr = getBC(); break;
                            case 1: addr = getDE(); break;
                            case 2: addr = getHL(); if (y == 4) setHL(addr + 1); else setHL(addr - 1); break;
                            case 3: addr = getHL(); if (y == 6) setHL(addr + 1); else setHL(addr - 1); break;
                        }
                        if (y & 1) A = readByte(addr);
                        else writeByte(addr, A);
                    }
                    break;
                    
                case 3: // INC/DEC r16
                    if (y & 1) { // DEC r16
                        switch (y >> 1) {
                            case 0: { uint16_t bc = getBC(); DEC_r16(bc); setBC(bc); } break;
                            case 1: { uint16_t de = getDE(); DEC_r16(de); setDE(de); } break;
                            case 2: { uint16_t hl = getHL(); DEC_r16(hl); setHL(hl); } break;
                            case 3: DEC_r16(SP); break;
                        }
                    } else { // INC r16
                        switch (y >> 1) {
                            case 0: { uint16_t bc = getBC(); INC_r16(bc); setBC(bc); } break;
                            case 1: { uint16_t de = getDE(); INC_r16(de); setDE(de); } break;
                            case 2: { uint16_t hl = getHL(); INC_r16(hl); setHL(hl); } break;
                            case 3: INC_r16(SP); break;
                        }
                    }
                    break;
                    
                case 4: // INC r8
                    switch (y) {
                        case 0: INC_r8(B); break;
                        case 1: INC_r8(C); break;
                        case 2: INC_r8(D); break;
                        case 3: INC_r8(E); break;
                        case 4: INC_r8(H); break;
                        case 5: INC_r8(L); break;
                        case 6: { uint8_t val = readByte(getHL()); INC_r8(val); writeByte(getHL(), val); } break;
                        case 7: INC_r8(A); break;
                    }
                    break;
                    
                case 5: // DEC r8
                    switch (y) {
                        case 0: DEC_r8(B); break;
                        case 1: DEC_r8(C); break;
                        case 2: DEC_r8(D); break;
                        case 3: DEC_r8(E); break;
                        case 4: DEC_r8(H); break;
                        case 5: DEC_r8(L); break;
                        case 6: { uint8_t val = readByte(getHL()); DEC_r8(val); writeByte(getHL(), val); } break;
                        case 7: DEC_r8(A); break;
                    }
                    break;
                    
                case 6: // LD r8, imm8
                    switch (y) {
                        case 0: LD_r8_imm8(B); break;
                        case 1: LD_r8_imm8(C); break;
                        case 2: LD_r8_imm8(D); break;
                        case 3: LD_r8_imm8(E); break;
                        case 4: LD_r8_imm8(H); break;
                        case 5: LD_r8_imm8(L); break;
                        case 6: LD_HL_imm8(); break;
                        case 7: LD_r8_imm8(A); break;
                    }
                    break;
                    
                case 7: // Rotates and misc
                    switch (y) {
                        case 0: RLCA(); break;
                        case 1: RRCA(); break;
                        case 2: RLA(); break;
                        case 3: RRA(); break;
                        // TODO: DAA, CPL, SCF, CCF
                    }
                    break;
            }
            break;
            
        case 1: // Block 1 - LD r8, r8 and HALT
            if (opcode == 0x76) {
                HALT();
            } else {
                uint8_t* dest = nullptr;
                uint8_t src_val = 0;
                
                // Determine source
                switch (z) {
                    case 0: src_val = B; break;
                    case 1: src_val = C; break;
                    case 2: src_val = D; break;
                    case 3: src_val = E; break;
                    case 4: src_val = H; break;
                    case 5: src_val = L; break;
                    case 6: src_val = readByte(getHL()); break;
                    case 7: src_val = A; break;
                }
                
                // Determine destination
                switch (y) {
                    case 0: LD_r8_r8(B, src_val); break;
                    case 1: LD_r8_r8(C, src_val); break;
                    case 2: LD_r8_r8(D, src_val); break;
                    case 3: LD_r8_r8(E, src_val); break;
                    case 4: LD_r8_r8(H, src_val); break;
                    case 5: LD_r8_r8(L, src_val); break;
                    case 6: writeByte(getHL(), src_val); break;
                    case 7: LD_r8_r8(A, src_val); break;
                }
            }
            break;
            
        case 2: // Block 2 - ALU operations
            {
                uint8_t operand = 0;
                switch (z) {
                    case 0: operand = B; break;
                    case 1: operand = C; break;
                    case 2: operand = D; break;
                    case 3: operand = E; break;
                    case 4: operand = H; break;
                    case 5: operand = L; break;
                    case 6: operand = readByte(getHL()); break;
                    case 7: operand = A; break;
                }
                
                switch (y) {
                    case 0: ADD_A_r8(operand); break;
                    case 1: ADC_A_r8(operand); break;
                    case 2: SUB_A_r8(operand); break;
                    case 3: SBC_A_r8(operand); break;
                    case 4: AND_A_r8(operand); break;
                    case 5: XOR_A_r8(operand); break;
                    case 6: OR_A_r8(operand); break;
                    case 7: CP_A_r8(operand); break;
                }
            }
            break;
            
        case 3: // Block 3
            switch (z) {
                case 0: // RET cond
                    switch (y) {
                        case 0: RET_cond(!getFlagZ()); break;
                        case 1: RET_cond(getFlagZ()); break;
                        case 2: RET_cond(!getFlagC()); break;
                        case 3: RET_cond(getFlagC()); break;
                    }
                    break;
                    
                case 1: // POP r16stk or RET
                    if (y & 1) {
                        if (y == 1) RET();
                        else if (y == 3) RETI();
                        else if (y == 5) JP_HL();
                        else if (y == 7) LD_SP_HL();
                    } else {
                        uint16_t val = pop();
                        switch (y >> 1) {
                            case 0: setBC(val); break;
                            case 1: setDE(val); break;
                            case 2: setHL(val); break;
                            case 3: setAF(val); break;
                        }
                    }
                    break;
                    
                case 2: // JP cond, imm16
                    switch (y) {
                        case 0: JP_cond_imm16(!getFlagZ()); break;
                        case 1: JP_cond_imm16(getFlagZ()); break;
                        case 2: JP_cond_imm16(!getFlagC()); break;
                        case 3: JP_cond_imm16(getFlagC()); break;
                    }
                    break;
                    
                case 3: // JP, CB prefix, and misc
                    if (y == 0) JP_imm16();
                    else if (y == 1) executeCBInstruction();
                    // TODO: Other y values
                    break;
                    
                case 4: // CALL cond, imm16
                    switch (y) {
                        case 0: CALL_cond_imm16(!getFlagZ()); break;
                        case 1: CALL_cond_imm16(getFlagZ()); break;
                        case 2: CALL_cond_imm16(!getFlagC()); break;
                        case 3: CALL_cond_imm16(getFlagC()); break;
                    }
                    break;
                    
                case 5: // PUSH r16stk or CALL
                    if (y & 1) {
                        if (y == 1) CALL_imm16();
                    } else {
                        uint16_t val = 0;
                        switch (y >> 1) {
                            case 0: val = getBC(); break;
                            case 1: val = getDE(); break;
                            case 2: val = getHL(); break;
                            case 3: val = getAF(); break;
                        }
                        push(val);
                    }
                    break;
                    
                case 6: // ALU imm8
                    {
                        uint8_t imm = readByte(PC++);
                        switch (y) {
                            case 0: ADD_A_r8(imm); break;
                            case 1: ADC_A_r8(imm); break;
                            case 2: SUB_A_r8(imm); break;
                            case 3: SBC_A_r8(imm); break;
                            case 4: AND_A_r8(imm); break;
                            case 5: XOR_A_r8(imm); break;
                            case 6: OR_A_r8(imm); break;
                            case 7: CP_A_r8(imm); break;
                        }
                    }
                    break;
                    
                case 7: // RST
                    RST(y * 8);
                    break;
            }
            break;
    }
}

void CPU::executeCBInstruction() {
    uint8_t opcode = readByte(PC++);
    uint8_t x = (opcode >> 6) & 0x03;
    uint8_t y = (opcode >> 3) & 0x07;
    uint8_t z = opcode & 0x07;
    
    // Helper to get register reference or [HL]
    auto executeOnR8 = [&](auto operation) {
        if (z == 6) {
            uint8_t val = readByte(getHL());
            operation(val);
            writeByte(getHL(), val);
        } else {
            uint8_t* reg = nullptr;
            switch (z) {
                case 0: reg = &B; break;
                case 1: reg = &C; break;
                case 2: reg = &D; break;
                case 3: reg = &E; break;
                case 4: reg = &H; break;
                case 5: reg = &L; break;
                case 7: reg = &A; break;
            }
            if (reg) operation(*reg);
        }
    };
    
    switch (x) {
        case 0: // Rotates and shifts
            switch (y) {
                case 0: executeOnR8([this](uint8_t& r) { RLC_r8(r); }); break;
                case 1: executeOnR8([this](uint8_t& r) { RRC_r8(r); }); break;
                case 2: executeOnR8([this](uint8_t& r) { RL_r8(r); }); break;
                case 3: executeOnR8([this](uint8_t& r) { RR_r8(r); }); break;
                case 4: executeOnR8([this](uint8_t& r) { SLA_r8(r); }); break;
                case 5: executeOnR8([this](uint8_t& r) { SRA_r8(r); }); break;
                case 6: executeOnR8([this](uint8_t& r) { SWAP_r8(r); }); break;
                case 7: executeOnR8([this](uint8_t& r) { SRL_r8(r); }); break;
            }
            break;
            
        case 1: // BIT
            {
                uint8_t val = (z == 6) ? readByte(getHL()) : 
                    (z == 0 ? B : z == 1 ? C : z == 2 ? D : z == 3 ? E : z == 4 ? H : z == 5 ? L : A);
                BIT_b_r8(y, val);
            }
            break;
            
        case 2: // RES
            executeOnR8([this, y](uint8_t& r) { RES_b_r8(y, r); });
            break;
            
        case 3: // SET
            executeOnR8([this, y](uint8_t& r) { SET_b_r8(y, r); });
            break;
    }
}

// ============================================================================
// HELPER METHODS
// ============================================================================

uint16_t CPU::readWord(uint16_t address) {
    uint8_t low = readByte(address);
    uint8_t high = readByte(address + 1);
    return (high << 8) | low;
}

void CPU::writeWord(uint16_t address, uint16_t value) {
    writeByte(address, value & 0xFF);
    writeByte(address + 1, (value >> 8) & 0xFF);
}

void CPU::push(uint16_t value) {
    SP -= 2;
    writeWord(SP, value);
}

uint16_t CPU::pop() {
    uint16_t value = readWord(SP);
    SP += 2;
    return value;
}

// ============================================================================
// INSTRUCTION IMPLEMENTATIONS
// ============================================================================

// Control
void CPU::NOP() {
    // Do nothing
}

void CPU::HALT() {
    halted = true;
}

void CPU::STOP() {
    // TODO: Enter low power mode?
}

// Loads - 8-bit
void CPU::LD_r8_r8(uint8_t& dest, uint8_t src) {
    dest = src;
}

void CPU::LD_r8_imm8(uint8_t& dest) {
    dest = readByte(PC++);
}

void CPU::LD_r8_HL(uint8_t& dest) {
    dest = readByte(getHL());
}

void CPU::LD_HL_r8(uint8_t src) {
    writeByte(getHL(), src);
}

void CPU::LD_HL_imm8() {
    writeByte(getHL(), readByte(PC++));
}

// Loads - 16-bit
void CPU::LD_r16_imm16(uint16_t& dest) {
    dest = readWord(PC);
    PC += 2;
}

void CPU::LD_SP_HL() {
    SP = getHL();
}

// Stack operations
void CPU::PUSH_r16(uint16_t value) {
    push(value);
}

void CPU::POP_r16(uint16_t& dest) {
    dest = pop();
}

// Arithmetic - 8-bit
void CPU::ADD_A_r8(uint8_t value) {
    uint16_t result = A + value;
    setFlagH(((A & 0xF) + (value & 0xF)) > 0xF);
    setFlagC(result > 0xFF);
    A = result & 0xFF;
    setFlagZ(A == 0);
    setFlagN(false);
}

void CPU::ADC_A_r8(uint8_t value) {
    uint8_t carry = getFlagC() ? 1 : 0;
    uint16_t result = A + value + carry;
    setFlagH(((A & 0xF) + (value & 0xF) + carry) > 0xF);
    setFlagC(result > 0xFF);
    A = result & 0xFF;
    setFlagZ(A == 0);
    setFlagN(false);
}

void CPU::SUB_A_r8(uint8_t value) {
    setFlagH((A & 0xF) < (value & 0xF));
    setFlagC(A < value);
    A -= value;
    setFlagZ(A == 0);
    setFlagN(true);
}

void CPU::SBC_A_r8(uint8_t value) {
    uint8_t carry = getFlagC() ? 1 : 0;
    uint16_t result = A - value - carry;
    setFlagH((A & 0xF) < ((value & 0xF) + carry));
    setFlagC(result > 0xFF);
    A = result & 0xFF;
    setFlagZ(A == 0);
    setFlagN(true);
}

void CPU::AND_A_r8(uint8_t value) {
    A &= value;
    setFlagZ(A == 0);
    setFlagN(false);
    setFlagH(true);
    setFlagC(false);
}

void CPU::XOR_A_r8(uint8_t value) {
    A ^= value;
    setFlagZ(A == 0);
    setFlagN(false);
    setFlagH(false);
    setFlagC(false);
}

void CPU::OR_A_r8(uint8_t value) {
    A |= value;
    setFlagZ(A == 0);
    setFlagN(false);
    setFlagH(false);
    setFlagC(false);
}

void CPU::CP_A_r8(uint8_t value) {
    setFlagZ(A == value);
    setFlagN(true);
    setFlagH((A & 0xF) < (value & 0xF));
    setFlagC(A < value);
}

void CPU::INC_r8(uint8_t& reg) {
    setFlagH((reg & 0xF) == 0xF);
    reg++;
    setFlagZ(reg == 0);
    setFlagN(false);
}

void CPU::DEC_r8(uint8_t& reg) {
    setFlagH((reg & 0xF) == 0);
    reg--;
    setFlagZ(reg == 0);
    setFlagN(true);
}

// Arithmetic - 16-bit
void CPU::ADD_HL_r16(uint16_t value) {
    uint16_t hl = getHL();
    uint32_t result = hl + value;
    setFlagH(((hl & 0xFFF) + (value & 0xFFF)) > 0xFFF);
    setFlagC(result > 0xFFFF);
    setFlagN(false);
    setHL(result & 0xFFFF);
}

void CPU::INC_r16(uint16_t& reg) {
    reg++;
}

void CPU::DEC_r16(uint16_t& reg) {
    reg--;
}

// Jumps
void CPU::JP_imm16() {
    PC = readWord(PC);
}

void CPU::JP_HL() {
    PC = getHL();
}

void CPU::JP_cond_imm16(bool condition) {
    uint16_t addr = readWord(PC);
    PC += 2;
    if (condition) {
        PC = addr;
    }
}

void CPU::JR_imm8() {
    int8_t offset = (int8_t)readByte(PC++);
    PC += offset;
}

void CPU::JR_cond_imm8(bool condition) {
    int8_t offset = (int8_t)readByte(PC++);
    if (condition) {
        PC += offset;
    }
}

// Calls/Returns
void CPU::CALL_imm16() {
    uint16_t addr = readWord(PC);
    PC += 2;
    push(PC);
    PC = addr;
}

void CPU::CALL_cond_imm16(bool condition) {
    uint16_t addr = readWord(PC);
    PC += 2;
    if (condition) {
        push(PC);
        PC = addr;
    }
}

void CPU::RET() {
    PC = pop();
}

void CPU::RET_cond(bool condition) {
    if (condition) {
        PC = pop();
    }
}

void CPU::RETI() {
    PC = pop();
    ime = true;
}

void CPU::RST(uint8_t vector) {
    push(PC);
    PC = vector;
}

// Rotates/Shifts (Accumulator versions)
void CPU::RLCA() {
    uint8_t bit7 = (A & 0x80) >> 7;
    A = (A << 1) | bit7;
    setFlagC(bit7);
    setFlagZ(false);
    setFlagN(false);
    setFlagH(false);
}

void CPU::RRCA() {
    uint8_t bit0 = A & 0x01;
    A = (A >> 1) | (bit0 << 7);
    setFlagC(bit0);
    setFlagZ(false);
    setFlagN(false);
    setFlagH(false);
}

void CPU::RLA() {
    uint8_t bit7 = (A & 0x80) >> 7;
    A = (A << 1) | (getFlagC() ? 1 : 0);
    setFlagC(bit7);
    setFlagZ(false);
    setFlagN(false);
    setFlagH(false);
}

void CPU::RRA() {
    uint8_t bit0 = A & 0x01;
    A = (A >> 1) | (getFlagC() ? 0x80 : 0);
    setFlagC(bit0);
    setFlagZ(false);
    setFlagN(false);
    setFlagH(false);
}

// CB Prefix Instructions
void CPU::RLC_r8(uint8_t& reg) {
    uint8_t bit7 = (reg & 0x80) >> 7;
    reg = (reg << 1) | bit7;
    setFlagC(bit7);
    setFlagZ(reg == 0);
    setFlagN(false);
    setFlagH(false);
}

void CPU::RRC_r8(uint8_t& reg) {
    uint8_t bit0 = reg & 0x01;
    reg = (reg >> 1) | (bit0 << 7);
    setFlagC(bit0);
    setFlagZ(reg == 0);
    setFlagN(false);
    setFlagH(false);
}

void CPU::RL_r8(uint8_t& reg) {
    uint8_t bit7 = (reg & 0x80) >> 7;
    reg = (reg << 1) | (getFlagC() ? 1 : 0);
    setFlagC(bit7);
    setFlagZ(reg == 0);
    setFlagN(false);
    setFlagH(false);
}

void CPU::RR_r8(uint8_t& reg) {
    uint8_t bit0 = reg & 0x01;
    reg = (reg >> 1) | (getFlagC() ? 0x80 : 0);
    setFlagC(bit0);
    setFlagZ(reg == 0);
    setFlagN(false);
    setFlagH(false);
}

void CPU::SLA_r8(uint8_t& reg) {
    setFlagC((reg & 0x80) != 0);
    reg <<= 1;
    setFlagZ(reg == 0);
    setFlagN(false);
    setFlagH(false);
}

void CPU::SRA_r8(uint8_t& reg) {
    uint8_t bit7 = reg & 0x80;
    setFlagC(reg & 0x01);
    reg = (reg >> 1) | bit7;
    setFlagZ(reg == 0);
    setFlagN(false);
    setFlagH(false);
}

void CPU::SWAP_r8(uint8_t& reg) {
    reg = ((reg & 0x0F) << 4) | ((reg & 0xF0) >> 4);
    setFlagZ(reg == 0);
    setFlagN(false);
    setFlagH(false);
    setFlagC(false);
}

void CPU::SRL_r8(uint8_t& reg) {
    setFlagC(reg & 0x01);
    reg >>= 1;
    setFlagZ(reg == 0);
    setFlagN(false);
    setFlagH(false);
}

void CPU::BIT_b_r8(uint8_t bit, uint8_t value) {
    setFlagZ((value & (1 << bit)) == 0);
    setFlagN(false);
    setFlagH(true);
}

void CPU::RES_b_r8(uint8_t bit, uint8_t& reg) {
    reg &= ~(1 << bit);
}

void CPU::SET_b_r8(uint8_t bit, uint8_t& reg) {
    reg |= (1 << bit);
}
