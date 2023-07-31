#include "bus.h"

void clear_memory(Bus* bus){
    for(size_t i = 0; i < RAM_SIZE; i++){
        bus->cpu_ram[i] = 0;
    }
}

void bus_cpu_write(Bus* bus, u_int16_t addr, u_int8_t data){
    if(cart_write_cpu(bus->cart, addr, data)){
        //Do nothing..
    }
    else if(addr >= MIN_ADDRESS_RAM && addr <= MAX_ADDRESS_RAM){
        bus->cpu_ram[addr & MIRROR_MASK_RAM] = data;
    }
    else if(addr >= MIN_ADDRESS_PPU && addr <= MAX_ADDRESS_PPU){
        ppu_cpu_write(bus->ppu, addr & MIRROR_MASK_PPU, data);
    }
}

u_int8_t bus_cpu_read(Bus* bus, u_int16_t addr, bool readOnly){
    u_int8_t data = ZERO;
    if(cart_read_cpu(bus->cart, addr, &data)){
        //Do nothing..
    }
    else if(addr >= MIN_ADDRESS_RAM && addr <= MAX_ADDRESS_RAM){
        data = bus->cpu_ram[addr & MIRROR_MASK_RAM];
    }
    if(addr >= MIN_ADDRESS_PPU && addr <= MAX_ADDRESS_PPU){
        data = ppu_cpu_read(bus->ppu, addr & MIRROR_MASK_PPU, readOnly);
    }

    return data;
}

void insert_cartridge(Bus* bus, Cartridge* cartridge) {
    bus->cpu = initialize_cpu(bus);
    bus->ppu = initialize_ppu();
    bus->cart = cartridge;

    ppu_connect_cartridge(bus->ppu, cartridge);
}

void bus_reset(Bus* bus) {
    cpu_reset(bus->cpu);
    bus->system_clocks = 0;
}

void bus_clock(Bus* bus) {
    ppu_clock(bus->ppu);

    if(bus->system_clocks % 3 == 0){
        clock(bus->cpu);
    }

    if(bus->ppu->nmi){
        bus->ppu->nmi = false;
        nmi(bus->cpu);
    }

    bus->system_clocks++;
}
