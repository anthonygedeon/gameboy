// Picture Processing Unit (PPU). Accesses Video RAM (VRAM) and 
// object/sprite attribute memory (SMEM) to draw to the screen. 
//
// The display is 160x144 pixels made up of tiles of 8x8 pixels.
//
// There are four modes when drawing: 
// (2) scanning sprite attributes in SMEM
// (3) drawing background and window based on scroll and window position
//     also constantly pushes new background pixels to bgFIFO as well as
//     new sprite pixels to spriteFIFO if available
// (0) H-Blank: waiting out the rest of the scanline until 
//              456 T-cycles (1 T-cycle = 4 clock cycles) 
//              have passed during the scanline.
// (1) V-Blank: waiting out 10 extra scanlines to get 59.7FPS total.
// More details at https://hacktixme.ga/GBEDG/ppu/#the-concept-of-ppu-modes

#ifndef PPU_H
#define PPU_H

#include <array>
#include <cstdint>
#include <deque> // Using a dequeue instead of queue for explicit resizing

#include <SDL.h> // For window and renderer.

#include "memory.h"
#include "mmu.h"

struct Sprite {
    uint8_t yPos;
    uint8_t xPos;
    uint8_t tileNum;
    // Flags:
    // Bit 7: 0 to always render above background.
    //        1 to only render above background color 0.
    // Bit 6: 1 to flip sprite vertically.
    // Bit 5: 1 to flip sprite horizontally.
    // Bit 4: 0 to use palette in OBP0 register. 1 to use palette in OBP1.
    // Bit 3-0: GBC only flags.
    uint8_t flags;
}

struct Pixel {
    // Pixel info:
    //      Bit 7: Unused.
    //      Bit 6: Sprite priority. GBC only.
    //      Bit 5: Copies Bit 7 in sprite attributes if this belongs to sprite.
    //      Bit 4: Palette choice. 0 for OBP0 register, 1 for OBP1 register.
    //      Bit 4-2: If GBC, value of 0-7 for palette. Else Bit 3-2 are unused.
    //      Bit 1-0: 2 bit-depth color value ignoring palette.
    uint8_t info;
}

class PPU {
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    std::array<Sprite, 10> m_sprites;
    std::deque<Pixel> m_bgFIFO;
    std::deque<Pixel> m_spriteFIFO;

    // Reference to VRAM in MMU.
    Memory& m_VRAM;

    // Reference to SMEM in MMU.
    Memory& m_SMEM;

    // LCD Control register (Reference to $FF40 in MMU):
    // Bit 7: 0 disables the PPU and turns the screen off.
    // Bit 6: Window tile map. 0 uses $9800-$9BFF. 1 uses $9C00-$9FFF.
    // Bit 5: 0 hides the window layer
    // Bit 4: Addressing method. 1 uses 8000 method. 0 uses 8800 method.
    // Bit 3: Bg tile map. 0 uses $9800-$9BFF. 1 uses $9C00-$9FFF.
    // Bit 2: Sprite size. 0 uses 1x1 tiles. 1 uses 1x2 (vertical) tiles.
    // Bit 1: 0 disables sprite drawing.
    // Bit 0: 0 disables both background and window drawing.
    // More detailed information at:
    // https://hacktixme.ga/GBEDG/ppu/#lcd-control-register-lcdc--ff40
    uint8_t& LCDC;

    // LCD Status register (Reference to $FF41 in MMU):
    // Bit 7: Unused, always 1.
    // Bit 6: 1 enables LYC==LY to trigger a STAT interrupt
    // Bit 5: 1 enables entering mode 2 (OAM scan) to trigger STAT interrupt
    // Bit 4: 1 enables entering mode 1 (VBlank) to trigger STAT interrupt
    // Bit 3: 1 enables entering mode 0 (HBlank) to trigger STAT interrupt
    // Bit 2 (Automatic): Set by the PPU if LYC==LY
    // Bit 1-0 (Automatic): Set to the following values by PPU based on mode:
    //      0: HBlank
    //      1: VBlank
    //      2: OAM Scan
    //      3: Drawing
    uint8_t& STAT;

    // Make sure default constructor is illegal.
    PPU();
    
public:
    // Set references to corresponding addresses in MMU.
    PPU(SDL_Window* window, MMU& mmu);

    // Gets time at start of frame for HBlank timing
    // and runs all modes for each scanline.
    void drawFrame();

    // Scans for sprites that render to the current scanline,
    // loads their attributes to the sprite array, and
    // pushes the corresponding pixels onto the spriteFIFO.
    void scanSprites(uint8_t scanlineNum);

    // Mixes sprite and bg FIFO, then puts those pixels on the screen.
    void drawScanline(uint8_t scanlineNum);
    
    // Waits out the rest of the scanline (456 T-cycles total)
    void doHBlank(uint8_t cyclesLeft);

    // Waits 10 scanlines (4560 T-Cycles).
    void doVBlank();

    //TODO: Should the PPU create the SDL window and renderer, and let the
    //      debugger ask for it?
}

#endif
