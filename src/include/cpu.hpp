#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <functional>

class CPU {
public:
    CPU();
    
    // Memory interface (must be set externally)
    std::function<uint8_t(uint16_t)> readByte;
    std::function<void(uint16_t, uint8_t)> writeByte;
    
    // Execution
    void step();  // Execute one instruction
    void reset();
    
    // 8-bit register access
    uint8_t getA() const { return A; }
    uint8_t getB() const { return B; }
    uint8_t getC() const { return C; }
    uint8_t getD() const { return D; }
    uint8_t getE() const { return E; }
    uint8_t getH() const { return H; }
    uint8_t getL() const { return L; }
    uint8_t getF() const { return F; }
    
    void setA(uint8_t value) { A = value; }
    void setB(uint8_t value) { B = value; }
    void setC(uint8_t value) { C = value; }
    void setD(uint8_t value) { D = value; }
    void setE(uint8_t value) { E = value; }
    void setH(uint8_t value) { H = value; }
    void setL(uint8_t value) { L = value; }
    void setF(uint8_t value) { F = value & 0xF0; } // Lower 4 bits always 0
    
    // 16-bit register pair access
    uint16_t getAF() const { return (A << 8) | F; }
    uint16_t getBC() const { return (B << 8) | C; }
    uint16_t getDE() const { return (D << 8) | E; }
    uint16_t getHL() const { return (H << 8) | L; }
    uint16_t getSP() const { return SP; }
    uint16_t getPC() const { return PC; }
    
    void setAF(uint16_t value) { A = (value >> 8) & 0xFF; F = value & 0xF0; }
    void setBC(uint16_t value) { B = (value >> 8) & 0xFF; C = value & 0xFF; }
    void setDE(uint16_t value) { D = (value >> 8) & 0xFF; E = value & 0xFF; }
    void setHL(uint16_t value) { H = (value >> 8) & 0xFF; L = value & 0xFF; }
    void setSP(uint16_t value) { SP = value; }
    void setPC(uint16_t value) { PC = value; }
    
    // Flag access methods
    bool getFlagZ() const { return (F & 0x80) != 0; } // Bit 7
    bool getFlagN() const { return (F & 0x40) != 0; } // Bit 6
    bool getFlagH() const { return (F & 0x20) != 0; } // Bit 5
    bool getFlagC() const { return (F & 0x10) != 0; } // Bit 4
    
    void setFlagZ(bool value) { if (value) F |= 0x80; else F &= ~0x80; }
    void setFlagN(bool value) { if (value) F |= 0x40; else F &= ~0x40; }
    void setFlagH(bool value) { if (value) F |= 0x20; else F &= ~0x20; }
    void setFlagC(bool value) { if (value) F |= 0x10; else F &= ~0x10; }
    
private:
    // Instruction execution
    void executeInstruction(uint8_t opcode);
    void executeCBInstruction();
    
    // Helper methods
    uint16_t readWord(uint16_t address);
    void writeWord(uint16_t address, uint16_t value);
    void push(uint16_t value);
    uint16_t pop();
    
    // Instruction implementations
    // Control
    void NOP();
    void HALT();
    void STOP();
    
    // Loads - 8-bit
    void LD_r8_r8(uint8_t& dest, uint8_t src);
    void LD_r8_imm8(uint8_t& dest);
    void LD_r8_HL(uint8_t& dest);
    void LD_HL_r8(uint8_t src);
    void LD_HL_imm8();
    
    // Loads - 16-bit
    void LD_r16_imm16(uint16_t& dest);
    void LD_SP_HL();
    
    // Stack operations
    void PUSH_r16(uint16_t value);
    void POP_r16(uint16_t& dest);
    
    // Arithmetic - 8-bit
    void ADD_A_r8(uint8_t value);
    void ADC_A_r8(uint8_t value);
    void SUB_A_r8(uint8_t value);
    void SBC_A_r8(uint8_t value);
    void AND_A_r8(uint8_t value);
    void XOR_A_r8(uint8_t value);
    void OR_A_r8(uint8_t value);
    void CP_A_r8(uint8_t value);
    void INC_r8(uint8_t& reg);
    void DEC_r8(uint8_t& reg);
    
    // Arithmetic - 16-bit
    void ADD_HL_r16(uint16_t value);
    void INC_r16(uint16_t& reg);
    void DEC_r16(uint16_t& reg);
    
    // Jumps
    void JP_imm16();
    void JP_HL();
    void JP_cond_imm16(bool condition);
    void JR_imm8();
    void JR_cond_imm8(bool condition);
    
    // Calls/Returns
    void CALL_imm16();
    void CALL_cond_imm16(bool condition);
    void RET();
    void RET_cond(bool condition);
    void RETI();
    void RST(uint8_t vector);
    
    // Rotates/Shifts
    void RLCA();
    void RRCA();
    void RLA();
    void RRA();
    
    // CB Prefix Instructions
    void RLC_r8(uint8_t& reg);
    void RRC_r8(uint8_t& reg);
    void RL_r8(uint8_t& reg);
    void RR_r8(uint8_t& reg);
    void SLA_r8(uint8_t& reg);
    void SRA_r8(uint8_t& reg);
    void SWAP_r8(uint8_t& reg);
    void SRL_r8(uint8_t& reg);
    void BIT_b_r8(uint8_t bit, uint8_t value);
    void RES_b_r8(uint8_t bit, uint8_t& reg);
    void SET_b_r8(uint8_t bit, uint8_t& reg);
    
    // State
    bool halted;
    bool ime; // Interrupt Master Enable
    // 8-bit registers
    uint8_t A;  // Accumulator
    uint8_t F;  // Flags
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;
    
    // 16-bit registers
    uint16_t SP; // Stack Pointer
    uint16_t PC; // Program Counter
};

#endif // CPU_HPP
