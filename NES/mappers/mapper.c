#include "mapper.h"

/**
 * These are all private definitions for default behavior for a mapper, think of it like an
 * overridable/virtual function in an OOP-Language.
 *
 * The 4 read/write functions for the CPU and PPU are not provided because if you're not implementing them, something
 * is definitely wrong.
 */

static enum MIRROR default_mirror(MAPPER* mapper){
    return HARDWARE;
}

static void default_reset(MAPPER* mapper){
    //Does nothing.
}

static bool default_irq_state(MAPPER* mapper){
    //Does nothing.
    return false;
}

static void default_irq_clear(MAPPER* mapper){
    //Does nothing.
}

void default_scanline(MAPPER* mapper){
    //Does nothing.
}

void init_mapper(MAPPER* mapper, u_int8_t num_prg_banks, u_int8_t num_chr_banks) {
    mapper->prg_banks = num_prg_banks;
    mapper->chr_banks = num_chr_banks;

    mapper->mapper_cpu_read = NULL;
    mapper->mapper_cpu_write = NULL;
    mapper->mapper_ppu_read = NULL;
    mapper->mapper_ppu_write = NULL;

    mapper->mapper_irq_state = default_irq_state;
    mapper->mapper_irq_clear = default_irq_clear;
    mapper->mirror = default_mirror;
    mapper->reset = default_reset;
    mapper->scanline = default_scanline;
}
