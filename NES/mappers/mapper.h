#ifndef NESEMULATOR_MAPPER_H
#define NESEMULATOR_MAPPER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

enum MIRROR
{
    HARDWARE,
    HORIZONTAL,
    VERTICAL,
    ONESCREEN_LO,
    ONESCREEN_HI,
};

typedef struct MAPPER {
    uint8_t prg_banks;
    uint8_t chr_banks;

    //The 4 memory functions associated with each mapper.
    bool (*mapper_cpu_read)(struct MAPPER* mapper, uint16_t addr, uint32_t* mapped_addr, uint8_t* data);
    bool (*mapper_cpu_write)(struct MAPPER* mapper, uint16_t addr, uint32_t* mapped_addr, uint8_t data);
    bool (*mapper_ppu_read)(struct MAPPER* mapper, uint16_t addr, uint32_t* mapped_addr);
    bool (*mapper_ppu_write)(struct MAPPER* mapper, uint16_t addr, uint32_t* mapped_addr);

    //IRQ functions
    bool (*mapper_irq_state)(struct MAPPER* mapper);
    void (*mapper_irq_clear)(struct MAPPER* mapper);

    //Utility Functions
    void (*reset)(struct MAPPER* mapper);
    void (*scanline)(struct MAPPER* mapper);
    enum MIRROR (*mirror)(struct MAPPER* mapper);

} MAPPER;

/**
 * Initializes the base attributes of the mapper, excluding the 5 functions associated with it!
 * This shouldn't be called in any real instance, use the mappers defined in the custom_mappers folder.
 * @param mapper_base Mapper Base Struct to write to
 * @param num_prg_banks Number of Program Banks
 * @param num_chr_banks Number of Character Banks
 */
void init_mapper(MAPPER* mapper, uint8_t num_prg_banks, uint8_t num_chr_banks);

#endif //NESEMULATOR_MAPPER_H
