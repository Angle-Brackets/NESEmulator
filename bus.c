#include "bus.h"

void clear_memory(Bus* bus){
    for(size_t i = 0; i < RAM_SIZE; i++){
        bus->ram[i] = 0;
    }
}

void b_write(Bus* bus, u_int16_t addr, u_int8_t data){
    if(addr >= MIN_ADDRESS && addr <= MAX_ADDRESS){
        bus->ram[addr] = data;
    }
}

u_int8_t b_read(Bus* bus, u_int16_t addr, bool readOnly){
    if(addr >= MIN_ADDRESS && addr <= MAX_ADDRESS){
        return bus->ram[addr];
    }
    return INVALID; //NULL would work here as well, just didn't want to use it.
}