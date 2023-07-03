#include "ppu2C02.h"
#include "../emuconsts.h"

//Just placeholders for now.

PPU2C02* initialize_ppu() {
    PPU2C02* ppu = (PPU2C02*)malloc(sizeof(PPU2C02));
    ppu->sprite_screen = create_sprite(0,0,256*2,240*2,NULL);
    ppu->frame_complete = false;
    ppu->cycle = 0;
    ppu->scanline = 0;
    for(uint32_t i = 0; i < 240; i++){
        for(uint32_t j = 0; j < 256; j++) {
            ppu->sprite_screen->sprite_data[i][j] = (SDL_Color){0};
        }
    }

    //Colors that the NES was capable of displaying
    ppu->palette[0x00] = (SDL_Color){84, 84, 84};
    ppu->palette[0x01] = (SDL_Color){0, 30, 116};
    ppu->palette[0x02] = (SDL_Color){8, 16, 144};
    ppu->palette[0x03] = (SDL_Color){48, 0, 136};
    ppu->palette[0x04] = (SDL_Color){68, 0, 100};
    ppu->palette[0x05] = (SDL_Color){92, 0, 48};
    ppu->palette[0x06] = (SDL_Color){84, 4, 0};
    ppu->palette[0x07] = (SDL_Color){60, 24, 0};
    ppu->palette[0x08] = (SDL_Color){32, 42, 0};
    ppu->palette[0x09] = (SDL_Color){8, 58, 0};
    ppu->palette[0x0A] = (SDL_Color){0, 64, 0};
    ppu->palette[0x0B] = (SDL_Color){0, 60, 0};
    ppu->palette[0x0C] = (SDL_Color){0, 50, 60};
    ppu->palette[0x0D] = (SDL_Color){0, 0, 0};
    ppu->palette[0x0E] = (SDL_Color){0, 0, 0};
    ppu->palette[0x0F] = (SDL_Color){0, 0, 0};

    ppu->palette[0x10] = (SDL_Color){152, 150, 152};
    ppu->palette[0x11] = (SDL_Color){8, 76, 196};
    ppu->palette[0x12] = (SDL_Color){48, 50, 236};
    ppu->palette[0x13] = (SDL_Color){92, 30, 228};
    ppu->palette[0x14] = (SDL_Color){136, 20, 176};
    ppu->palette[0x15] = (SDL_Color){160, 20, 100};
    ppu->palette[0x16] = (SDL_Color){152, 34, 32};
    ppu->palette[0x17] = (SDL_Color){120, 60, 0};
    ppu->palette[0x18] = (SDL_Color){84, 90, 0};
    ppu->palette[0x19] = (SDL_Color){40, 114, 0};
    ppu->palette[0x1A] = (SDL_Color){8, 124, 0};
    ppu->palette[0x1B] = (SDL_Color){0, 118, 40};
    ppu->palette[0x1C] = (SDL_Color){0, 102, 120};
    ppu->palette[0x1D] = (SDL_Color){0, 0, 0};
    ppu->palette[0x1E] = (SDL_Color){0, 0, 0};
    ppu->palette[0x1F] = (SDL_Color){0, 0, 0};

    ppu->palette[0x20] = (SDL_Color){236, 238, 236};
    ppu->palette[0x21] = (SDL_Color){76, 154, 236};
    ppu->palette[0x22] = (SDL_Color){120, 124, 236};
    ppu->palette[0x23] = (SDL_Color){176, 98, 236};
    ppu->palette[0x24] = (SDL_Color){228, 84, 236};
    ppu->palette[0x25] = (SDL_Color){236, 88, 180};
    ppu->palette[0x26] = (SDL_Color){236, 106, 100};
    ppu->palette[0x27] = (SDL_Color){212, 136, 32};
    ppu->palette[0x28] = (SDL_Color){160, 170, 0};
    ppu->palette[0x29] = (SDL_Color){116, 196, 0};
    ppu->palette[0x2A] = (SDL_Color){76, 208, 32};
    ppu->palette[0x2B] = (SDL_Color){56, 204, 108};
    ppu->palette[0x2C] = (SDL_Color){56, 180, 204};
    ppu->palette[0x2D] = (SDL_Color){60, 60, 60};
    ppu->palette[0x2E] = (SDL_Color){0, 0, 0};
    ppu->palette[0x2F] = (SDL_Color){0, 0, 0};

    ppu->palette[0x30] = (SDL_Color){236, 238, 236};
    ppu->palette[0x31] = (SDL_Color){168, 204, 236};
    ppu->palette[0x32] = (SDL_Color){188, 188, 236};
    ppu->palette[0x33] = (SDL_Color){212, 178, 236};
    ppu->palette[0x34] = (SDL_Color){236, 174, 236};
    ppu->palette[0x35] = (SDL_Color){236, 174, 212};
    ppu->palette[0x36] = (SDL_Color){236, 180, 176};
    ppu->palette[0x37] = (SDL_Color){228, 196, 144};
    ppu->palette[0x38] = (SDL_Color){204, 210, 120};
    ppu->palette[0x39] = (SDL_Color){180, 222, 120};
    ppu->palette[0x3A] = (SDL_Color){168, 226, 144};
    ppu->palette[0x3B] = (SDL_Color){152, 226, 180};
    ppu->palette[0x3C] = (SDL_Color){160, 214, 228};
    ppu->palette[0x3D] = (SDL_Color){160, 162, 160};
    ppu->palette[0x3E] = (SDL_Color){0, 0, 0};
    ppu->palette[0x3F] = (SDL_Color){0, 0, 0};
    
    
    return ppu;
}

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

void ppu_connect_cartridge(PPU2C02* ppu, Cartridge* cartridge) {
    ppu->cart = cartridge;
}

void ppu_clock(PPU2C02* ppu){
    ppu->sprite_screen->sprite_data[ppu->scanline][ppu->cycle-1] = ppu->palette[(rand() % 2) ? 0x3F : 0x30];

    ppu->cycle++;
    if(ppu->cycle >= 341){
        ppu->cycle = 0;
        ppu->scanline++;
        if(ppu->scanline >= 300){
            ppu->scanline = 0;
            ppu->frame_complete = true;
        }
    }
}
