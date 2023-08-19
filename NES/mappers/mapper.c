#include "mapper.h"

void init_mapper(MAPPER* mapper, u_int8_t num_prg_banks, u_int8_t num_chr_banks) {
    mapper->prg_banks = num_prg_banks;
    mapper->chr_banks = num_chr_banks;

    mapper->mapper_cpu_read = NULL;
    mapper->mapper_cpu_write = NULL;
    mapper->mapper_ppu_read = NULL;
    mapper->mapper_ppu_write = NULL;
    mapper->mapper_irq_state = NULL;
    mapper->mapper_irq_clear = NULL;
    mapper->mirror = NULL;
    mapper->reset = NULL;
    mapper->scanline = NULL;
}
