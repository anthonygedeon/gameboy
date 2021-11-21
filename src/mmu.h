// Provides 8-bit read/write to different memory regions.

#ifndef MMU_H
#define MMU_H

#include "memory.h"
#include <cstdint>

union Reg16 {

  uint16_t word;
  struct {
    uint8_t low;
    uint8_t high;
  };
};

/// Memory Management Unit
/// main focus for memory pages.
class MMU {
private:
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

  uint8_t &get(uint16_t address);

public:
  MMU();

  uint8_t read8(uint16_t address);
  void write8(uint16_t address, uint8_t data);

  uint16_t read16(uint16_t address);
  void write16(uint16_t address, uint16_t data);
};

#endif
