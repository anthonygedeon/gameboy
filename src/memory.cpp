#include <cstdint>
#include "memory.h"

Memory::Memory(uint16_t startAddress = 0x0000, uint16_t endAddress = 0x3FFF)
    : m_startAddress(startAddress), m_memory(endAddress - startAddress + 1)
{
}

/**
 * @brief
 *
 * @param address
 * @return uint8_t&
 */
uint8_t& Memory::get(uint16_t address)
{
    return m_memory[address - m_startAddress];
}
