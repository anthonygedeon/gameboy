// Provides 8-bit read/write to a chunk of memory.

#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <vector>

class Memory {
    uint16_t m_startAddress;
    std::vector<uint8_t> m_memory;
public:
    Memory(uint16_t startAddress, uint16_t endAddress);

    // Automatically converts 'memory map' address to local address.
    // TODO: It may be a much better idea to convert memory in the MMU
    // instead, as that would completely invalidate this class.
    uint8_t& get(uint16_t address);
};

#endif
