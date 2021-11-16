#include <assert.h>
#include "src/MMU.h"

class MMUTest {
public:
    void testBIOSWriteRead() {
        MMU mmu;
        mmu.write(0x0000, 0xAB);
        mmu.write(0x00FF, 0xFF);
        assert(mmu.read(0x0000) == 0xAB);
        assert(mmu.read(0x00FF) == 0xFF);
    }

    void testROMWriteRead() {
        MMU mmu;
        mmu.write(0x3333, 0x33);
        mmu.write(0x7FFF, 0x7F);
        assert(mmu.read(0x3333) == 0x33);
        assert(mmu.read(0x7FFF) == 0x7F);
    }

    void testVRAMWriteRead() {
        MMU mmu;
        mmu.write(0x8888, 0x88);
        mmu.write(0x9FFF, 0x9F);
        assert(mmu.read(0x8888) == 0x88);
        assert(mmu.read(0x9FFF) == 0x9F);
    }

    void testEchoRAMWriteRead() {
        MMU mmu;
        // Write to WRAM
        mmu.write(0xC000, 0xC0);
        mmu.write(0xDDFF, 0xDD);
        // Write to Echo RAM (illegal on actual GB)
        mmu.write(0xEEEE, 0xEE);
        assert(mmu.read(0xE000) == 0xC0);
        assert(mmu.read(0xFDFF) == 0xDD);
        assert(mmu.read(0xCEEE) == 0xEE);
    }

    void testIllegalAccess() {
        MMU mmu;
        // Writing to illegal addresses currently returns the first ERAM 
        // byte and prints to console.
        mmu.write(0xFEA0, 0x33);
        assert(mmu.read(0xA000) == 0x33);
        mmu.write(0xFEFF, 0x44);
        assert(mmu.read(0xA000) == 0x44);
    }
};
