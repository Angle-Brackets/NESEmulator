#ifndef NESEMULATOR_PPU2C02_H
#define NESEMULATOR_PPU2C02_H

#include <stdio.h>
#include <stdbool.h>

#include "../memory/cartridge.h"

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
    u_int8_t tblName[2][1024]; //VRAM nametable information
    u_int8_t tblPalette[32]; //Palette Memory
} PPU2C02;

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
void ppu_connect_cartridge(PPU2C02* ppu, const Cartridge* cartridge);

/**
 * Performs a system tick on the PPU
 */
void ppu_tick(PPU2C02* ppu);

#endif //NESEMULATOR_PPU2C02_H
