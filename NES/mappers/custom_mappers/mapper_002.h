#ifndef NESEMULATOR_MAPPER_002_H
#define NESEMULATOR_MAPPER_002_H

#include "../mapper.h"

static u_int8_t prg_bank_lo = 0x00;
static u_int8_t prg_bank_hi = 0x00;

/**
 * Will create a mapper struct with the properties of mapper_002
 * @param mapper Mapper struct to write to
 */
void create_mapper_002(MAPPER* mapper, u_int8_t num_prg_banks, u_int8_t num_chr_banks);


#endif //NESEMULATOR_MAPPER_002_H
