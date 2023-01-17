#include "ppu2C02.h"
#include "../emuconsts.h"

//Just placeholders for now.

u_int8_t ppu_cpu_read(PPU2C02* ppu, u_int16_t addr, bool read_only) {
    u_int8_t data = ZERO;

    switch(addr){
        case CONTROL:
            break;
        case MASK:
            break;
        case STATUS:
            break;
        case OAM_ADDRESS:
            break;
        case OAM_DATA:
            break;
        case SCROLL:
            break;
        case PPU_ADDRESS:
            break;
        case PPU_DATA:
            break;
        default:
            INVALID;
    }

    return data;
}

void ppu_cpu_write(PPU2C02* ppu, u_int16_t addr, u_int8_t data) {
    switch(addr){
        case CONTROL:
            break;
        case MASK:
            break;
        case STATUS:
            break;
        case OAM_ADDRESS:
            break;
        case OAM_DATA:
            break;
        case SCROLL:
            break;
        case PPU_ADDRESS:
            break;
        case PPU_DATA:
            break;
        default:
            INVALID;
    }
}

u_int8_t ppu_read(PPU2C02* ppu, u_int16_t addr, bool read_only) {
    u_int8_t data = ZERO;
    addr &= MAX_ADDRESS_PPU;

    if(cart_read_ppu(ppu->cart, addr, &data)){
        //Do nothing..
    }

    return data;
}

void ppu_write(PPU2C02* ppu, u_int16_t addr, u_int8_t data) {
    addr &= MAX_ADDRESS_PPU;

    if(cart_write_ppu(ppu->cart, addr, data)){
        //Do nothing..
    }
}

void ppu_connect_cartridge(PPU2C02* ppu, const Cartridge *cartridge) {
    ppu->cart = cartridge;
}

