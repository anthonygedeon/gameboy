#include <SDL.h>

#include "mmu.h"
#include "ppu.h"

PPU::PPU(SDL_Window* window, MMU& mmu)
    :   m_window(window),
        m_bgFIFO(8), 
        m_spriteFIFO(8), 
        m_VRAM(mmu.getVRAM()), 
        m_SMEM(mmu.getSMEM()), 
        LCDC(mmu.get(0xFF40)), 
        STAT(mmu.get(0xFF41))
{
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
}

uint8_t PPU::read1(uint8_t reg, uint8_t bit) {
    return (reg & (static_cast<uint8_t>(1) << bit)) ? 1 : 0;
}
