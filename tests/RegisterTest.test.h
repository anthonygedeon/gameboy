#include <assert.h>

#include "src/register.h"

class RegisterTest {
public:
    void testByteStore() {
        Register reg;
        writeByte(reg, 0xDF);
        assert(reg.m_data == 0xDF);
    }

    void testByteLoad() {
        Register reg{ 0x8B };
        assert(readByte(reg) == 0x8B);
    }

    void testWordStore() {
        Register reg1;
        Register reg2;
        writeWord(reg1, reg2, 0x20BF);
        assert(reg1.m_data == 0x20);
        assert(reg2.m_data == 0xBF);
    }

    void testWordStoreLoad() {
        Register reg1;
        Register reg2;
        writeWord(reg1, reg2, 0x20BF);
        assert(readWord(reg1, reg2) == 0x20BF);
    }

    void testByteLoadAfterWordStore() {
        Register reg1;
        Register reg2;
        writeWord(reg1, reg2, 0x20BF);
        assert(readByte(reg1) == 0x20);
        assert(readByte(reg2) == 0xBF);
    }
};
