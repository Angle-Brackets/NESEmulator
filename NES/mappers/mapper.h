#ifndef NESEMULATOR_MAPPER_H
#define NESEMULATOR_MAPPER_H

#include <stdio.h>
#include <stdbool.h>

enum MIRROR
{
    HORIZONTAL,
    VERTICAL,
    ONESCREEN_LO,
    ONESCREEN_HI,
} mirror;

typedef struct MAPPER {
    u_int8_t prg_banks;
    u_int8_t chr_banks;

    //The 4 functions associated with each mapper.
    bool (*mapper_cpu_read)(struct MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t* data);
    bool (*mapper_cpu_write)(struct MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr, u_int8_t data);
    bool (*mapper_ppu_read)(struct MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr);
    bool (*mapper_ppu_write)(struct MAPPER* mapper, u_int16_t addr, u_int32_t* mapped_addr);
    void (*reset)(struct MAPPER* mapper);

} MAPPER;

/**
 * Initializes the base attributes of the mapper, excluding the 5 functions associated with it!
 * This shouldn't be called in any real instance, use the mappers defined in the custom_mappers folder.
 * @param mapper_base Mapper Base Struct to write to
 * @param num_prg_banks Number of Program Banks
 * @param num_chr_banks Number of Character Banks
 */
void init_mapper(MAPPER* mapper, u_int8_t num_prg_banks, u_int8_t num_chr_banks);

#endif //NESEMULATOR_MAPPER_H
