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


    struct Header* header_buffer = (struct Header*) malloc(sizeof(struct Header));
    fread((char*)header_buffer, sizeof(char), sizeof(struct Header), fp);

    //Some training information for the rom that can be ignored.
    if (header_buffer->mapper1 & 0x04)
        fseek(fp, 512, SEEK_CUR);

    cart->mapperID = ((header_buffer->mapper2 >> 4) << 4) | (header_buffer->mapper1 >> 4);

    //Get the file format (We are skipping 0 and 2 for now)
    uint8_t file_type_num = 1;

    if (file_type_num == 0) {}
    else if (file_type_num == 1) {
        cart->prg_banks = header_buffer->prg_rom_chunks;
        cart->prg_memory = malloc(cart->prg_banks * PRG_BANK_SIZE);
        fread((char*)cart->prg_memory, sizeof(char), cart->prg_banks * PRG_BANK_SIZE, fp);

        cart->chr_banks = header_buffer->chr_rom_chunks;
        cart->chr_memory = malloc(cart->chr_banks * CHR_BANK_SIZE);
        fread((char*)cart->chr_memory, sizeof(char), cart->chr_banks * CHR_BANK_SIZE, fp);
    } else {}

    //Load up our mapper according to the ID
    cart->mapper = (MAPPER){0};
    switch(cart->mapperID){
        case 0:
            create_mapper_000(&cart->mapper, cart->prg_banks, cart->chr_banks);
            break;
    }

    fclose(fp);
}

bool cart_read_cpu(Cartridge* cart, uint8_t addr, uint8_t* data) {
    uint32_t mapped_addr = 0;
    if(cart->mapper.mapper_cpu_read(&cart->mapper, addr, &mapped_addr)){
        *data = cart->prg_memory[mapped_addr];
        return true;
    }

    return false;
}

bool cart_write_cpu(Cartridge* cart, uint8_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if(cart->mapper.mapper_cpu_write(&cart->mapper, addr, &mapped_addr)){
        cart->prg_memory[mapped_addr] = data;
        return true;
    }

    return false;
}

bool cart_read_ppu(Cartridge* cart, uint8_t addr, uint8_t* data) {
    uint32_t mapped_addr = 0;
    if(cart->mapper.mapper_ppu_read(&cart->mapper, addr, &mapped_addr)){
        *data = cart->chr_memory[mapped_addr];
        return true;
    }

    return false;
}

bool cart_write_ppu(Cartridge* cart, uint8_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if(cart->mapper.mapper_ppu_write(&cart->mapper, addr, &mapped_addr)){
        cart->chr_memory[mapped_addr] = data;
        return true;
    }

    return false;
}