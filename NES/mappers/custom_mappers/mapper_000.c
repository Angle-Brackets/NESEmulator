#include "mapper_000.h"

bool mapper000_cpu_read(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t* data){
    if(addr >= 0x8000 && addr <= 0xFFFF){
        *mapped_addr = addr & (mapper->prg_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool mapper000_cpu_write(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t data){
    if(addr >= 0x8000 && addr <= 0xFFFF){
        *mapped_addr = addr & (mapper->prg_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool mapper_000_ppu_read(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr){
    if(addr >= 0x0000 && addr <= 0x1FFF){
        *mapped_addr = addr;
        return true;
    }

    return false;
}

bool mapper_000_ppu_write(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr){
    if(addr >= 0x0000 && addr <= 0x1FFF){
        if(mapper->chr_banks == 0){
            *mapped_addr = addr;
            return true;
        }
    }

    return false;
}

enum MIRROR mapper000_mirror(MAPPER* mapper){
    return HARDWARE;
}

void mapper000_reset(MAPPER* mapper){
    //Does nothing.
}

bool mapper000_irq_state(MAPPER* mapper){
    //Does nothing.
    return false;
}

void mapper000_irq_clear(MAPPER* mapper){
    //Does nothing.
}

void mapper000_scanline(MAPPER* mapper){
    //Does nothing.
}

void create_mapper_000(MAPPER* mapper, u_int8_t num_prg_banks, u_int8_t num_chr_banks){
    init_mapper(mapper, num_prg_banks, num_chr_banks);

    mapper->mapper_cpu_read = mapper000_cpu_read;
    mapper->mapper_cpu_write = mapper000_cpu_write;
    mapper->mapper_ppu_read = mapper_000_ppu_read;
    mapper->mapper_ppu_write = mapper_000_ppu_write;
    mapper->mapper_irq_state = mapper000_irq_state;
    mapper->mapper_irq_clear = mapper000_irq_clear;
    mapper->reset = mapper000_reset;
    mapper->mirror = mapper000_mirror;
    mapper->scanline = mapper000_scanline;
}

