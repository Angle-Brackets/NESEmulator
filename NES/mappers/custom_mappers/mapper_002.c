#include "mapper_002.h"

static u_int8_t prg_bank_lo;
static u_int8_t prg_bank_hi;

bool mapper002_cpu_read(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t* data){
    if(addr >= 0x8000 && addr <= 0xBFFF){
        *mapped_addr = prg_bank_lo * 0x4000 + (addr & 0x3FFF);
        return true;
    }
    if(addr >= 0xC000 && addr <= 0xFFFF){
        *mapped_addr = prg_bank_hi * 0x4000 + (addr & 0x3FFF);
        return true;
    }

    return false;
}

bool mapper002_cpu_write(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t data){
    if(addr >= 0x8000 && addr <= 0xFFFF){
        prg_bank_lo = data & 0x0F;
    }

    return false;
}

bool mapper_002_ppu_read(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr){
    if(addr < 0x2000){
        *mapped_addr = addr;
        return true;
    }

    return false;
}

bool mapper_002_ppu_write(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr){
    if(addr < 0x2000){
        if(mapper->chr_banks == 0){
            *mapped_addr = addr;
            return true;
        }
    }

    return false;
}

void mapper002_reset(MAPPER* mapper){
    prg_bank_lo = 0x00;
    prg_bank_hi = mapper->prg_banks - 1;
}

enum MIRROR mapper002_mirror(MAPPER* mapper){
    return HARDWARE;
}

bool mapper002_irq_state(MAPPER* mapper){
    //Does nothing.
    return false;
}

void mapper002_irq_clear(MAPPER* mapper){
    //Does nothing.
}

void mapper002_scanline(MAPPER* mapper){
    //Does nothing.
}

void create_mapper_002(MAPPER* mapper, u_int8_t num_prg_banks, u_int8_t num_chr_banks){
    init_mapper(mapper, num_prg_banks, num_chr_banks);

    mapper->mapper_cpu_read = mapper002_cpu_read;
    mapper->mapper_cpu_write = mapper002_cpu_write;
    mapper->mapper_ppu_read = mapper_002_ppu_read;
    mapper->mapper_ppu_write = mapper_002_ppu_write;
    mapper->mapper_irq_state = mapper002_irq_state;
    mapper->mapper_irq_clear = mapper002_irq_clear;
    mapper->reset = mapper002_reset;
    mapper->mirror = mapper002_mirror;
    mapper->scanline = mapper002_scanline;
}

