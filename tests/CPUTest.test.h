#include <assert.h>
#include "src/cpu.h"

class CPUTest {
public:
    void testRegisterWriteReadWord() {
        CPU cpu;
        writeWord(cpu.m_regs[A], cpu.m_regs[B], 0xFEAB);
        assert(readByte(cpu.m_regs[A]) == 0xFE);
        assert(readByte(cpu.m_regs[B]) == 0xAB);
        assert(readWord(cpu.m_regs[A], cpu.m_regs[B]) == 0xFEAB);
    }

    void testSetResetFlag() {
        CPU cpu;
        cpu.setFlag(fZ);
        cpu.setFlag(fN);
        cpu.setFlag(fH);
        cpu.setFlag(fC);
        assert(cpu.readFlag(fZ) == true);
        assert(cpu.readFlag(fN) == true);
        assert(cpu.readFlag(fH) == true);
        assert(cpu.readFlag(fC) == true);
        cpu.resetFlag(fZ);
        cpu.resetFlag(fN);
        cpu.resetFlag(fH);
        cpu.resetFlag(fC);
        assert(cpu.readFlag(fZ) == false);
        assert(cpu.readFlag(fN) == false);
        assert(cpu.readFlag(fH) == false);
        assert(cpu.readFlag(fC) == false);
    }
};
