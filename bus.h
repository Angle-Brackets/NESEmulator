#ifndef NESEMULATOR_BUS_H
#define NESEMULATOR_BUS_H

#include <stdio.h>
#include <stdbool.h>
#include "emuconsts.h"
#include "cpu6502.h"

//Forward Declaring this so we can avoid a circular logic error.
typedef struct CPU6502 CPU6502;

/**
 * Bus for a byte addressable CPU
 */
typedef struct Bus {
    CPU6502* cpu;
    u_int8_t ram[RAM_SIZE];
} Bus;

/**
 * Clears the memory in the CPU, not necessary but recommended on start-up
 * @param bus pointer to the bus struct.
 */
void clear_memory(Bus* bus);

/**
 * Write to byte addressable RAM at a given address with given data.
 * @param addr Address 0x0000 - 0xFFFF (might change later)
 * @param data Byte of data
 */
void b_write(Bus* bus, u_int16_t addr, u_int8_t data);

/**
 * Read a byte from the given address
 * @param addr Address to read from 0x0000 - 0xFFFF (might change later)
 * @param readOnly by default should be false, says if the memory is read only.
 * @return A byte at the given valid address.
 */
u_int8_t b_read(Bus* bus, u_int16_t addr, bool readOnly);

#endif
