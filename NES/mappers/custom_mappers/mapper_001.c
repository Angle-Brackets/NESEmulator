#include "mapper_001.h"

static u_int8_t chr_bank_select4lo = 0x00;
static u_int8_t chr_bank_select4hi = 0x00;
static u_int8_t chr_bank_select8 = 0x00;

static u_int8_t prg_bank_select16lo = 0x00;
static u_int8_t prg_bank_select16hi = 0x00;
static u_int8_t prg_bank_select32 = 0x00;

static u_int8_t loaded_reg = 0x00;
static u_int8_t load_reg_count = 0x00;
static u_int8_t control_reg = 0x00;

enum MIRROR mirror_mode = HORIZONTAL;

static u_int8_t* vRAMStatic;

bool mapper001_cpu_read(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t* data){
    if(addr >= 0x6000 && addr <= 0x7FFF){
        *mapped_addr = 0xFFFFFFFF; //Reading from ram on the cart.
        *data = vRAMStatic[addr & 0x1FFF];
        return true;
    }
    if(addr >= 0x8000){
        if(control_reg & 0b01000){
            //16K Mode
            if(addr >= 0x8000 && addr <= 0xBFFF){
                *mapped_addr = prg_bank_select16lo * 0x4000 + (addr & 0x3FFF);
                return true;
            }
            if(addr >= 0xC000 && addr <= 0xFFFF){
                *mapped_addr = prg_bank_select16hi * 0x4000 + (addr & 0x3FFF);
                return true;
            }
        }
        else{
            //32K Mode
            *mapped_addr = prg_bank_select32 * 0x8000 + (addr & 0x7FFF);
            return true;
        }
    }

    return false;
}

bool mapper001_cpu_write(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t data){
    if(addr >= 0x6000 && addr <= 0x7FFF){
        *mapped_addr = 0xFFFFFFFF;
        vRAMStatic[addr & 0x1FFF] = data;
        return true;
    }

    if(addr >= 0x8000){
        if(data & 0x80){
            //Reset serial loading of data
            loaded_reg = 0x00;
            load_reg_count = 0;
            control_reg = control_reg | 0x0C;
        }
        else{
            //The shifts are the serial data flow.
            loaded_reg >>= 1;
            loaded_reg |= (data & 0x01) << 4;
            load_reg_count++;

            if(load_reg_count == 5){
                uint8_t target_reg = (addr >> 13) & 0x03;
                if(target_reg == 0){
                    control_reg = loaded_reg & 0x1F;
                    switch(control_reg & 0x03){
                        case 0: mirror_mode = ONESCREEN_LO; break;
                        case 1: mirror_mode = ONESCREEN_HI; break;
                        case 2: mirror_mode = VERTICAL; break;
                        case 3: mirror_mode = HORIZONTAL; break;
                    }
                }
                else if(target_reg == 1){
                    if(control_reg & 0b10000){
                        chr_bank_select4lo = loaded_reg & 0x1F;
                    }
                    else{
                        chr_bank_select8 = loaded_reg & 0x1E;
                    }
                }
                else if(target_reg == 2){
                    if(control_reg & 0b10000){
                        chr_bank_select4hi = loaded_reg & 0x1F;
                    }
                }
                else if(target_reg == 3){
                    uint8_t prg_mode = (control_reg >> 2) & 0x03;

                    if(prg_mode == 0 || prg_mode == 1){
                        prg_bank_select32 = (loaded_reg & 0x0E) >> 1;
                    }
                    else if(prg_mode == 2){
                        prg_bank_select16lo = 0;
                        prg_bank_select16hi = loaded_reg & 0x0F;
                    }
                    else if(prg_mode == 3){
                        prg_bank_select16lo = loaded_reg & 0x0F;
                        prg_bank_select16hi = mapper->prg_banks - 1;
                    }
                }

                //All 5 bits were written, so now we reset.
                loaded_reg = 0x00;
                load_reg_count = 0;
            }
        }
    }

    return false;
}

bool mapper001_ppu_read(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr){
    if(addr < 0x2000){
        if(mapper->chr_banks == 0){
            *mapped_addr = addr;
            return true;
        }
        else{
            if(control_reg & 0b10000){
                if(addr >= 0x0000 && addr <= 0x0FFF){
                    *mapped_addr = chr_bank_select4lo * 0x1000 + (addr & 0x0FFF);
                    return true;
                }
                if(addr >= 0x1000 && addr <= 0x1FFF){
                    *mapped_addr = chr_bank_select4hi * 0x1000 + (addr & 0x0FFF);
                    return true;
                }
            }
            else{
                *mapped_addr = chr_bank_select8 * 0x1000 + (addr & 0x1FFF);
                return true;
            }
        }
    }

    return false;
}

bool mapper001_ppu_write(MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr){
    if(addr < 0x2000){
        if(mapper->chr_banks == 0){
            *mapped_addr = addr;
            return true;
        }
        return true;
    }

    return false;
}

void mapper001_reset(MAPPER* mapper){
    control_reg = 0x1C;
    loaded_reg = 0x00;
    load_reg_count = 0x00;

    chr_bank_select4lo = 0;
    chr_bank_select4hi = 0;
    chr_bank_select8 = 0;

    prg_bank_select16lo = 0;
    prg_bank_select16hi = mapper->prg_banks - 1;
    prg_bank_select32 = 0;
}

enum MIRROR mapper001_mirror(MAPPER* mapper){
    //ONESCREEN_LO and ONESCREEN_HI are not supported, that's why tetris and dr.mario have issues.
    return mirror_mode;
}

bool mapper001_irq_state(MAPPER* mapper){
    //Does nothing.
    return false;
}

void mapper001_irq_clear(MAPPER* mapper){
    //Does nothing.
}

void mapper001_scanline(MAPPER* mapper){
    //Does nothing.
}

void create_mapper_001(MAPPER* mapper, u_int8_t num_prg_banks, u_int8_t num_chr_banks){
    init_mapper(mapper, num_prg_banks, num_chr_banks);
    vRAMStatic = calloc(32 * 1024, sizeof(uint8_t));

    mapper->mapper_cpu_read = mapper001_cpu_read;
    mapper->mapper_cpu_write = mapper001_cpu_write;
    mapper->mapper_ppu_read = mapper001_ppu_read;
    mapper->mapper_ppu_write = mapper001_ppu_write;
    mapper->mapper_irq_state = mapper001_irq_state;
    mapper->mapper_irq_clear = mapper001_irq_clear;
    mapper->reset = mapper001_reset;
    mapper->mirror = mapper001_mirror;
    mapper->scanline = mapper001_scanline;
}


