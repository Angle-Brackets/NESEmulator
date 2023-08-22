#include "mapper_004.h"

//Internal variables
static u_int8_t target_reg = 0x00;
static bool prg_bank_mode = false;
static bool chr_inversion = false;
static enum MIRROR mirrormode = HORIZONTAL;

static u_int32_t registers[8];
static u_int32_t chr_bank[8];
static u_int32_t prg_bank[4];

static bool irq_active = false;
static bool irq_enable = false;
static bool irq_update = false;
static u_int16_t irq_counter = 0x0000;
static u_int16_t irq_reload = 0x0000;
static u_int8_t* vRAMStatic = NULL;

bool mapper004_cpu_read(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t* data){
    if(addr >= 0x6000 && addr <= 0x7FFF){
        *mapped_addr = 0xFFFFFFFF;
        *data = vRAMStatic[addr & 0x1FFF];
        return true;
    }

    if(addr >= 0x8000 && addr <= 0x9FFF){
        *mapped_addr = prg_bank[0] + (addr & 0x1FFF);
        return true;
    }
    if(addr >= 0xA000 && addr <= 0xBFFF){
        *mapped_addr = prg_bank[1] + (addr & 0x1FFF);
        return true;
    }
    if(addr >= 0xC000 && addr <= 0xDFFF){
        *mapped_addr = prg_bank[2] + (addr & 0x1FFF);
        return true;
    }
    if(addr >= 0xE000 && addr <= 0xFFFF){
        *mapped_addr = prg_bank[3] + (addr & 0x1FFF);
        return true;
    }

    return false;
}

bool mapper004_cpu_write(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t data){
    if(addr >= 0x6000 && addr <= 0x7FFF){
        *mapped_addr = 0xFFFFFFFF;
        vRAMStatic[addr & 0x1FFF] = data;
        return true;
    }
    if(addr >= 0x8000 && addr <= 0x9FFF){
        //Bank select
        if(!(addr & 0x0001)){
            target_reg = data & 0x07;
            prg_bank_mode = (data & 0x40);
            chr_inversion = (data & 0x80);
        }
        else{
            registers[target_reg] = data;
            if(chr_inversion){
                chr_bank[0] = registers[2] * 0x0400;
                chr_bank[1] = registers[3] * 0x0400;
                chr_bank[2] = registers[4] * 0x0400;
                chr_bank[3] = registers[5] * 0x0400;
                chr_bank[4] = (registers[0] & 0xFE) * 0x0400;
                chr_bank[5] = registers[0] * 0x0400 + 0x0400;
                chr_bank[6] = (registers[1] & 0xFE) * 0x0400;
                chr_bank[7] = registers[1] * 0x0400 + 0x0400;
            }
            else{
                chr_bank[0] = (registers[0] & 0xFE) * 0x0400;
                chr_bank[1] = registers[0] * 0x0400 + 0x0400;
                chr_bank[2] = (registers[1] & 0xFE) * 0x0400;
                chr_bank[3] = registers[1] * 0x0400 + 0x0400;
                chr_bank[4] = registers[2] * 0x0400;
                chr_bank[5] = registers[3] * 0x0400;
                chr_bank[6] = registers[4] * 0x0400;
                chr_bank[7] = registers[5] * 0x0400;
            }

            if(prg_bank_mode){
                prg_bank[2] = (registers[6] & 0x3F) * 0x2000;
                prg_bank[0] = (mapper->prg_banks * 2 - 2) * 0x2000;
            }
            else{
                prg_bank[0] = (registers[6] & 0x3F) * 0x2000;
                prg_bank[2] = (mapper->prg_banks * 2 - 2) * 0x2000;
            }

            prg_bank[1] = (registers[7] & 0x3F) * 0x2000;
            prg_bank[3] = (mapper->prg_banks * 2 - 1) * 0x2000;
        }

        return false;
    }

    if(addr >= 0xA000 && addr <= 0xBFFF){
        if(!(addr & 0x0001)){
            //Mirroring
            if(data & 0x01){
                mirrormode = HORIZONTAL;
            }
            else{
                mirrormode = VERTICAL;
            }
        }
        else{
            //Ram protection that is not implemented
        }
        return false;
    }

    if(addr >= 0xC000 && addr <= 0xDFFF){
        if(!(addr & 0x0001)){
            irq_reload = data;
        }
        else{
            irq_counter = 0x0000;
        }
        return false;
    }

    if(addr >= 0xE000 && addr <= 0xFFFF){
        if(!(addr & 0x0001)){
            irq_enable = false;
            irq_active = false;
        }
        else{
            irq_enable = true;
        }
        return false;
    }

    return false;
}

bool mapper_004_ppu_read(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr){
    if(addr >= 0x0000 && addr <= 0x03FF){
        *mapped_addr = chr_bank[0] + (addr & 0x03FF);
        return true;
    }
    if(addr >= 0x0400 && addr <= 0x07FF){
        *mapped_addr = chr_bank[1] + (addr & 0x03FF);
        return true;
    }
    if(addr >= 0x0800 && addr <= 0x0BFF){
        *mapped_addr = chr_bank[2] + (addr & 0x03FF);
        return true;
    }
    if(addr >= 0x0C00 && addr <= 0x0FFF){
        *mapped_addr = chr_bank[3] + (addr & 0x03FF);
        return true;
    }
    if(addr >= 0x1000 && addr <= 0x13FF){
        *mapped_addr = chr_bank[4] + (addr & 0x03FF);
        return true;
    }
    if(addr >= 0x1400 && addr <= 0x17FF){
        *mapped_addr = chr_bank[5] + (addr & 0x03FF);
        return true;
    }
    if(addr >= 0x1800 && addr <= 0x1BFF){
        *mapped_addr = chr_bank[6] + (addr & 0x03FF);
        return true;
    }
    if(addr >= 0x1C00 && addr <= 0x1FFF){
        *mapped_addr = chr_bank[7] + (addr & 0x03FF);
        return true;
    }

    return false;
}

bool mapper_004_ppu_write(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr){
    return false;
}

enum MIRROR mapper004_mirror(MAPPER* mapper){
    return mirrormode;
}

void mapper004_reset(MAPPER* mapper){
    target_reg = 0x00;
    prg_bank_mode = false;
    chr_inversion = false;
    mirrormode = HORIZONTAL;

    irq_active = false;
    irq_enable = false;
    irq_update = false;
    irq_counter = 0x0000;
    irq_reload = 0x0000;

    for(int32_t i = 0; i < 4; i++){
        prg_bank[i] = 0;
    }
    for(int32_t i = 0; i < 8; i++){
        chr_bank[i] = 0;
        registers[i] = 0;
    }

    prg_bank[0] = 0 * 0x2000;
    prg_bank[1] = 1 * 0x2000;
    prg_bank[2] = (mapper->prg_banks * 2 - 2) * 0x2000;
    prg_bank[3] = (mapper->prg_banks * 2 - 1) * 0x2000;
}

bool mapper004_irq_state(MAPPER* mapper){
    return irq_active;
}

void mapper004_irq_clear(MAPPER* mapper){
    irq_active = false;
}

void mapper004_scanline(MAPPER* mapper){
    if(irq_counter == 0){
        irq_counter = irq_reload;
    }
    else{
        irq_counter--;
    }

    if(irq_counter == 0 && irq_enable){
        irq_active = true;
    }
}

void create_mapper_004(MAPPER* mapper, u_int8_t num_prg_banks, u_int8_t num_chr_banks){
    init_mapper(mapper, num_prg_banks, num_chr_banks);
    vRAMStatic = calloc(32 * 1024, sizeof(uint8_t));

    mapper->mapper_cpu_read = mapper004_cpu_read;
    mapper->mapper_cpu_write = mapper004_cpu_write;
    mapper->mapper_ppu_read = mapper_004_ppu_read;
    mapper->mapper_ppu_write = mapper_004_ppu_write;
    mapper->mapper_irq_state = mapper004_irq_state;
    mapper->mapper_irq_clear = mapper004_irq_clear;
    mapper->reset = mapper004_reset;
    mapper->mirror = mapper004_mirror;
    mapper->scanline = mapper004_scanline;
}

