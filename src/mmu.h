// Provides 8-bit read/write to different memory regions.

#ifndef MMU_H
#define MMU_H

#include <cstdint>
#include "memory.h"

class MMU {
    bool inBIOS;

    Memory m_BIOS;
    Memory m_ROM;
    // Virtual RAM for GPU.
    Memory m_VRAM;
    // Extended RAM from memory banks on certain cartridges.
    Memory m_ERAM;
    // Internal working RAM in the GB itself.
    Memory m_WRAM;
    // Sprite attribute information.
    Memory m_SMEM;
    Memory m_IO;
    // Zero-page RAM (high-speed).
    Memory m_ZRAM;

    uint8_t& get(uint16_t address);
public:
    MMU();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);
};

#endif
