#ifndef NESEMULATOR_PPU2C02_H
#define NESEMULATOR_PPU2C02_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../memory/cartridge.h"
#include "graphics/sprite.h"

//Addresses that the PPU can read from through the CPU
#define CONTROL 0x0000
#define MASK 0x0001
#define STATUS 0x0002
#define OAM_ADDRESS 0x0003
#define OAM_DATA 0x0004
#define SCROLL 0x0005
#define PPU_ADDRESS 0x0006
#define PPU_DATA 0x0007

typedef struct PPU2C02 {
    Cartridge* cart;

    //PPU memory
    uint8_t tblName[2][1024]; //VRAM nametable information
    uint8_t  tblPattern[2][4096];
    uint8_t tblPalette[32]; //Palette Memory
    SDL_Color palette[0x40]; //Palette of NES
    uint8_t address_latch; //Tells us when we're writing to the low or high byte
    uint8_t ppu_data_buffer; //Buffered byte from reading from PPU
    uint16_t ppu_address; //Created address when reading or writing.
    sprite_t* sprite_screen; //Full screen output
    sprite_t* sprite_nametable[2];
    sprite_t* sprite_patterntable[2];

    //Debug Information
    bool frame_complete;
    int16_t cycle;
    int16_t scanline;

    //PPU Registers
    union
    {
        struct
        {
            uint8_t unused : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vertical_blank : 1;
        };

        uint8_t reg;
    } status;

    union
    {
        struct
        {
            uint8_t grayscale : 1;
            uint8_t render_background_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_background : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_red : 1;
            uint8_t enhance_green : 1;
            uint8_t enhance_blue : 1;
        };

        uint8_t reg;
    } mask;

    union PPUCTRL
    {
        struct
        {
            uint8_t nametable_x : 1;
            uint8_t nametable_y : 1;
            uint8_t increment_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_background : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1; // unused
            uint8_t enable_nmi : 1;
        };

        uint8_t reg;
    } control;
} PPU2C02;

/**
 * Initializes the PPU
 */
PPU2C02* initialize_ppu();

/**
 * Communicates with the main bus to read bytes through the PPU
 * @param addr Address in memory
 * @param read_only If the memory is read-only, default is false.
 * @return memory read from
 */
u_int8_t ppu_cpu_read(PPU2C02* ppu, u_int16_t addr, bool read_only);
/**
 * Communicates with the main bus to write bytes through the PPU
 * @param addr Address in memory
 * @param data data to write to in memory
 */
void ppu_cpu_write(PPU2C02* ppu, u_int16_t addr, u_int8_t data);

/**
 * Communicates with the PPU (Picture Processing Unit) to read bytes
 * @param addr Address to read from
 * @param read_only If the memory is read only, default is false
 * @return The memory read from
 */
u_int8_t ppu_read(PPU2C02* ppu, u_int16_t addr, bool read_only);

/**
 * Communicates with the PPU to write bytes
 * @param addr Address to write to
 * @param data data to write to in memory
 */
void ppu_write(PPU2C02* ppu, u_int16_t addr, u_int8_t data);

/**
 * Connects the cartridge to the PPU
 * @param cartridge
 */
void ppu_connect_cartridge(PPU2C02* ppu, Cartridge* cartridge);

/**
 * Performs a system clock on the PPU
 */
void ppu_clock(PPU2C02* ppu);

sprite_t* get_pattern_table(PPU2C02* ppu, uint8_t i, uint8_t palette);

SDL_Color color_from_pal_ram(PPU2C02* ppu, uint8_t palette, uint8_t pixel);

#endif //NESEMULATOR_PPU2C02_H
