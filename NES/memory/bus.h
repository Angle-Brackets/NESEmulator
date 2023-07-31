#ifndef NESEMULATOR_BUS_H
#define NESEMULATOR_BUS_H

#include <stdio.h>
#include <stdbool.h>
#include "cartridge.h"
#include "../emuconsts.h"
#include "../cpu/cpu6502.h"
#include "../ppu/ppu2C02.h"

//Forward Declaring this so we can avoid a circular logic error.
typedef struct CPU6502 CPU6502;
typedef struct PPU2C02 PPU2C02;

/**
 * Bus for a byte addressable CPU
 */
typedef struct Bus {
    CPU6502* cpu;
    PPU2C02* ppu;
    Cartridge* cart;

    uint8_t controller[2];
    uint8_t controller_state[2];
    u_int8_t cpu_ram[RAM_SIZE];
    u_int32_t system_clocks; //Number of clocks the system has gone through
} Bus;

//General Bus Operations
/**
 * Clears the memory in the CPU, not necessary but recommended on start-up
 * @param bus pointer to the bus struct.
 */
void clear_memory(Bus* bus);

/**
 * Write to byte addressable RAM at a given address with given data.
 * Only the CPU is allowed to write to the bus.
 * @param addr Address 0x0000 - 0xFFFF (might change later)
 * @param data Byte of data
 */
void bus_cpu_write(Bus* bus, u_int16_t addr, u_int8_t data);

/**
 * Read a byte from the given address
 * Only the CPU is allowed to read from the bus.
 * @param addr Address to read from 0x0000 - 0xFFFF (might change later)
 * @param readOnly by default should be false, says if the memory is read only.
 * @return A byte at the given valid address.
 */
u_int8_t bus_cpu_read(Bus* bus, u_int16_t addr, bool readOnly);

//System Interactions through the Bus

/**
 * Inserts a cartridge into ROM
 * @param cartridge cartridge that was inputted.
 */
void insert_cartridge(Bus* bus, Cartridge* cartridge);

/**
 * Resets the console
 */
void bus_reset(Bus* bus);

/**
 * Runs one system clock of the console.
 */
void bus_clock(Bus* bus);


#endif
