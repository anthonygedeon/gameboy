#include <stdio.h>
#include <cstdint>
#include "mmu.h"

MMU::MMU()
        // BIOS addresses are usable after the BIOS runs, which is why
        // it overlaps with ROM.
    :   inBIOS(true),
        m_BIOS  (0x0000, 0x00FF),
        // GB ROM is 0000-3FFFF, but cartridge ROM can use 4000-7FFF.
        m_ROM   (0x0000, 0x7FFF),
        m_VRAM  (0x8000, 0x9FFF),
        m_ERAM  (0xA000, 0xBFFF),
        m_WRAM  (0xC000, 0xDFFF),
        m_SMEM  (0xFE00, 0xFE9F),
        m_IO    (0xFF00, 0xFF7F),
        m_ZRAM  (0xFF80, 0xFFFF)
{
}

uint8_t& MMU::get(uint16_t address) {
    switch(address & 0xF000) {
        // BIOS/ROM
        case 0x0000: 
            if (inBIOS && ((address & 0x0F00) == 0x0000)) {
                return m_BIOS.get(address);
            }
            // Intentional fall-through.
        // GB ROM
        case 0x1000: case 0x2000: case 0x3000:
            return m_ROM.get(address);
        // Cartridge ROM
        case 0x4000: case 0x5000: case 0x6000: case 0x7000:
            return m_ROM.get(address);
        // GPU VRAM
        case 0x8000: case 0x9000:
            return m_VRAM.get(address);
        // Cartridge (External) RAM
        case 0xA000: case 0xB000:
            return m_ERAM.get(address);
        // Working RAM
        case 0xC000: case 0xD000:
            return m_WRAM.get(address);
        // Shadow RAM + sprite attribute mem + IO mem + zero-page RAM
        case 0xF000:
            switch (address & 0x0F00) {
                case 0x0E00:
                    if ((address & 0x00F0) <= 0x0090) {
                        // Intentional fall-through, catches 0xF000-0xFDFF.
                    } else {
                        // TODO: HANDLE AND LOG ILLEGAL ADDRESS ACCESS.
                        printf("Illegal memory access: %X\n", address);
                        return m_ERAM.get(0xA000);
                    }
                case 0x0F00:
                    if ((address & 0x00F0) <= 0x0070) {
                        return m_IO.get(address);
                    } else {
                        return m_ZRAM.get(address);
                    }
            }
            // Intentional fall-through, catches 0xF000-0xFDFF.
        // Shadow RAM
        case 0xE000:
            return m_WRAM.get(address - 0x2000);
    }

    // TODO: Log any memory access holes.
    printf("Memory access fell all the way through: %X\n", address);
    return m_ERAM.get(0xA000);
}

uint8_t MMU::read(uint16_t address) {
    return get(address);
}

void MMU::write(uint16_t address, uint8_t data) {
    get(address) = data;
}
