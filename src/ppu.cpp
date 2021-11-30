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
