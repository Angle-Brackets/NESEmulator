#include "mapper_003.h"

static u_int8_t chr_bank_select;

bool mapper003_cpu_read(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t* data){
    if(addr >= 0x8000 && addr <= 0xFFFF){
        if(mapper->prg_banks == 1)
            *mapped_addr = addr & 0x3FFF;
        if(mapper->prg_banks == 2)
            *mapped_addr = addr & 0x7FFF;
        return true;
    }

    return false;
}

bool mapper003_cpu_write(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t data){
    if(addr >= 0x8000 && addr <= 0xFFFF){
        chr_bank_select = data & 0x03;
        *mapped_addr = addr;
    }

    return false;
}

bool mapper003_ppu_read(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr){
    if(addr < 0x2000){
        *mapped_addr = chr_bank_select * 0x2000 + addr;
        return true;
    }

    return false;
}

bool mapper003_ppu_write(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr){
    return false;
}

void mapper003_reset(MAPPER* mapper){
    chr_bank_select = 0;
}

void create_mapper_003(MAPPER* mapper, u_int8_t num_prg_banks, u_int8_t num_chr_banks){
    init_mapper(mapper, num_prg_banks, num_chr_banks);

    mapper->mapper_cpu_read = mapper003_cpu_read;
    mapper->mapper_cpu_write = mapper003_cpu_write;
    mapper->mapper_ppu_read = mapper003_ppu_read;
    mapper->mapper_ppu_write = mapper003_ppu_write;

    mapper->reset = mapper003_reset;
}