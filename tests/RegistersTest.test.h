#include <stdio.h>
#include <assert.h>

#include "src/registers.h"

extern Register _REG[10];

class RegistersTest {
public:
    void test8BitStore() {
        Register reg;
        store8Bit(&reg, 0xDF);
        assert(reg.m_data == 0xDF);
    }

    void test8BitLoad() {
        Register reg{ 0x8B };
        assert(load8Bit(&reg) == 0x8B);
    }

    void test16BitStore() {
        Register reg1;
        Register reg2;
        store16Bit(&reg1, &reg2, 0x20BF);
        assert(reg1.m_data == 0x20);
        assert(reg2.m_data == 0xBF);
    }

    void test16BitStoreLoad() {
        Register reg1;
        Register reg2;
        store16Bit(&reg1, &reg2, 0x20BF);
        assert(load16Bit(&reg1, &reg2) == 0x20BF);
    }

    void test8BitLoadAfter16BitStore() {
        Register reg1;
        Register reg2;
        store16Bit(&reg1, &reg2, 0x20BF);
        assert(load8Bit(&reg1) == 0x20);
        assert(load8Bit(&reg2) == 0xBF);
    }

    void test8BitLoadAfter16BitStoreUsingNamedRegisters() {
        store16Bit(&_REG[A], &_REG[B], 0x20BF);
        assert(load8Bit(&_REG[A]) == 0x20);
        assert(load8Bit(&_REG[B]) == 0xBF);
    }
};
