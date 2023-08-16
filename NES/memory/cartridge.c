#include "cartridge.h"

void initialize_cartridge(Cartridge* cart, const char* file) {
    // iNES Format Header
    struct Header
    {
        char name[4];
        u_int8_t prg_rom_chunks;
        u_int8_t chr_rom_chunks;
        u_int8_t mapper1;
        u_int8_t mapper2;
        u_int8_t prg_ram_size;
        u_int8_t tv_system1;
        u_int8_t tv_system2;
        char unused[5];
    };

    FILE* fp = fopen(file, "rb");

    if(fp == NULL || ferror(fp)){
        fprintf(stderr, "Failed to open file.\n");
        exit(1);
    }

    struct Header header;
    fread((char*)&header, sizeof(struct Header), 1, fp);

    //Some training information for the rom that can be ignored.
    if(header.mapper1 & 0x04)
        fseek(fp, 512, SEEK_CUR);

    cart->mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

    //Get the file format (We are skipping 0 and 2 for now)
    cart->mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;
    uint8_t file_type_num = 1;
    if ((header.mapper2 & 0x0C) == 0x08) file_type_num = 2;

    if (file_type_num == 0) {}
    else if (file_type_num == 1) {
        cart->prg_banks = header.prg_rom_chunks;
        cart->prg_memory = (uint8_t*)malloc(cart->prg_banks * PRG_BANK_SIZE);
        fread((char*)cart->prg_memory, 16384, cart->prg_banks, fp);

        cart->chr_banks = header.chr_rom_chunks;
        if(cart->chr_banks == 0){
            //Create the RAM
            cart->chr_memory = (uint8_t*)malloc(CHR_BANK_SIZE);
        }
        else{
            cart->chr_memory = (uint8_t*)malloc(cart->chr_banks * CHR_BANK_SIZE);
        }

        fread((char*)cart->chr_memory, CHR_BANK_SIZE, cart->chr_banks, fp);
    } else {
        cart->prg_banks = ((header.prg_ram_size & 0x07) << 8) | header.prg_rom_chunks;
        cart->prg_memory = (uint8_t*)malloc(cart->prg_banks * PRG_BANK_SIZE);
        fread((char*)cart->prg_memory, PRG_BANK_SIZE, cart->prg_banks, fp);

        cart->chr_banks = ((header.prg_ram_size & 0x38) << 8) | header.chr_rom_chunks;
        cart->chr_memory = (uint8_t*)malloc(cart->chr_banks * CHR_BANK_SIZE);
        fread((char*)cart->chr_memory, CHR_BANK_SIZE, cart->chr_banks, fp);
    }

    //Load up our mapper according to the ID
    cart->mapper = (MAPPER){0};
    switch(cart->mapperID){
        case 0:
            create_mapper_000(&cart->mapper, cart->prg_banks, cart->chr_banks);
            break;
        case 1:
            create_mapper_001(&cart->mapper, cart->prg_banks, cart->chr_banks);
            break;
        case 2:
            create_mapper_002(&cart->mapper, cart->prg_banks, cart->chr_banks);
            break;
        case 3:
            create_mapper_003(&cart->mapper, cart->prg_banks, cart->chr_banks);
            break;
        default:
            fprintf(stderr, "Unsupported Mapper: %i\n", cart->mapperID);
            exit(1);
    }

    fclose(fp);
}

void reset_cartridge(Cartridge* cart){
    if(cart != NULL){
        cart->mapper.reset(&cart->mapper);
    }
}

bool cart_read_cpu(Cartridge* cart, uint16_t addr, uint8_t* data) {
    uint32_t mapped_addr = 0;
    if(cart->mapper.mapper_cpu_read(&cart->mapper, addr, &mapped_addr, data)){
        if(mapped_addr == 0xFFFFFFFF){
            return true; //Mapper has set a value itself.
        }
        else{
            *data = cart->prg_memory[mapped_addr];
        }
        return true;
    }

    return false;
}

bool cart_write_cpu(Cartridge* cart, uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if(cart->mapper.mapper_cpu_write(&cart->mapper, addr, &mapped_addr, data)){
        if (mapped_addr == 0xFFFFFFFF){
            // Mapper has actually set the data value, for example cartridge based RAM
            return true;
        }
        else{
            cart->prg_memory[mapped_addr] = data;
        }
        return true;
    }

    return false;
}

bool cart_read_ppu(Cartridge* cart, uint16_t addr, uint8_t* data) {
    uint32_t mapped_addr = 0;
    if(cart->mapper.mapper_ppu_read(&cart->mapper, addr, &mapped_addr)){
        *data = cart->chr_memory[mapped_addr];
        return true;
    }

    return false;
}

bool cart_write_ppu(Cartridge* cart, uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if(cart->mapper.mapper_ppu_write(&cart->mapper, addr, &mapped_addr)){
        cart->chr_memory[mapped_addr] = data;
        return true;
    }

    return false;
}

enum MIRROR cart_mirror(Cartridge* cart){
    static bool warning = false; //Warning for if ONESCREEN_LO or HI is used.
    enum MIRROR m = cart->mapper.mirror(&cart->mapper);

    if(!warning && ((m == ONESCREEN_LO) || (m == ONESCREEN_HI))){
        warning = true;
        fprintf(stderr, "Game loaded uses mirror modes that are not supported, it is unlikely that it will work!\n");
    }

    if(m == HARDWARE){
        return cart->mirror;
    }
    else{
        return m; //This mirror has been modified by the cartridge!
    }
}
