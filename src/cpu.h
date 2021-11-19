// Holds the CPU registers and interfaces with memory through the MMU

#ifndef CPU_H
#define CPU_H

#include "register.h"
#include "mmu.h"

// 10 CPU registers in GB.
// The actual register array are defined in the cpp file to prevent 
// linking issues.
enum eRegister {
    A,B,C,D,E,F,H,L,SP,PC
};

// 4 flags stored in the F register.
enum eFlag {
    fC=16,
    fH=32,
    fN=64,
    fZ=128
};

class CPU {
// TODO: Not sure what to put in privates here, since I don't know how opcode access, etc. is going to work.
public:
    Register m_regs[10];
    MMU m_mmu;

    CPU();

    bool readFlag(eFlag flag);
    void setFlag(eFlag flag);
    void resetFlag(eFlag flag);
};

#endif
