#ifndef NESEMULATOR_CARTRIDGE_H
#define NESEMULATOR_CARTRIDGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Mappers
#include "../mappers/custom_mappers/mapper_000.h"
#include "../mappers/custom_mappers/mapper_001.h"
#include "../mappers/custom_mappers/mapper_002.h"
#include "../mappers/custom_mappers/mapper_003.h"

#define PRG_BANK_SIZE 16384 //~16K
#define CHR_BANK_SIZE 8192 //~8K

typedef struct Cartridge {
    char* file; //File that we loaded
    u_int8_t* prg_memory; //Program Memory (Game code and other things)
    u_int8_t* chr_memory; //Character Memory (Graphics)

    u_int8_t mapperID;
    u_int8_t prg_banks;
    u_int8_t chr_banks;

    MAPPER mapper; //Mapper we're using.
    enum MIRROR mirror;
} Cartridge;

void initialize_cartridge(Cartridge* cart, const char* file);
void reset_cartridge(Cartridge* cart); //Resets mappers

bool cart_read_cpu(Cartridge* cart, uint16_t addr, uint8_t* data);
bool cart_write_cpu(Cartridge* cart, uint16_t addr, uint8_t data);
bool cart_read_ppu(Cartridge* cart, uint16_t addr, uint8_t* data);
bool cart_write_ppu(Cartridge* cart, uint16_t addr, uint8_t data);

enum MIRROR cart_mirror(Cartridge* cart);

#endif //NESEMULATOR_CARTRIDGE_H
