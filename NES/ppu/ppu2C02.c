#include "ppu2C02.h"
#include "../emuconsts.h"

PPU2C02* initialize_ppu() {
    PPU2C02* ppu = (PPU2C02*)calloc(1, sizeof(PPU2C02));
    //Sprites that are used to draw to the screen
    ppu->sprite_screen = create_sprite(0,0,NES_WIDTH,NES_HEIGHT,NULL);
    ppu->sprite_nametable[0] = create_sprite(0,0,NES_WIDTH,NES_HEIGHT,NULL);
    ppu->sprite_nametable[1] = create_sprite(0,0,NES_WIDTH,NES_HEIGHT,NULL);
    ppu->sprite_patterntable[0] = create_sprite(WIDTH-(2*128)-20,HEIGHT-135,128,128,NULL);
    ppu->sprite_patterntable[1] = create_sprite(WIDTH-(1*128)-10,HEIGHT-135,128,128,NULL);

    ppu->frame_complete = false;
    ppu->cycle = 0;
    ppu->scanline = 0;
    ppu->fine_x = 0;
    ppu->bg_next_tile_id = 0;
    ppu->bg_next_tile_attrib = 0;
    ppu->bg_next_tile_lsb = 0;
    ppu->bg_next_tile_msb = 0;
    ppu->address_latch = 0x00;
    ppu->ppu_data_buffer = 0x00;
    ppu->ppu_address = 0x0000;
    ppu->vram_addr.reg = 0x0000;
    ppu->tram_addr.reg = 0x0000;
    ppu->bg_shifter_pattern_lo = 0x0000;
    ppu->bg_shifter_pattern_hi = 0x0000;
    ppu->bg_shifter_attrib_lo = 0x0000;
    ppu->bg_shifter_attrib_hi = 0x0000;
    ppu->pOAM = (uint8_t*)ppu->OAM;
    ppu->oam_addr = 0x00;

    ppu->odd_frame = false;
    ppu->nmi = false;
    ppu->sprite_zero_hit_possible = false;
    ppu->sprite_zero_being_rendered = false;

    for(uint32_t i = 0; i < NES_HEIGHT; i++){
        for(uint32_t j = 0; j < NES_WIDTH; j++) {
            ppu->sprite_screen->sprite_data[i][j] = (SDL_Color){0};
            ppu->sprite_nametable[0]->sprite_data[i][j] = (SDL_Color){0};
            ppu->sprite_nametable[1]->sprite_data[i][j] = (SDL_Color){0};
        }
    }

    for(uint32_t i = 0; i < 128; i++){
        for(uint32_t j = 0; j < 128; j++) {
            ppu->sprite_patterntable[0]->sprite_data[i][j] = (SDL_Color){0};
            ppu->sprite_patterntable[1]->sprite_data[i][j] = (SDL_Color){0};
        }
    }

    //Colors that the NES was capable of displaying
    ppu->palette[0x00] = (SDL_Color){84, 84, 84, 255};
    ppu->palette[0x01] = (SDL_Color){0, 30, 116, 255};
    ppu->palette[0x02] = (SDL_Color){8, 16, 144, 255};
    ppu->palette[0x03] = (SDL_Color){48, 0, 136, 255};
    ppu->palette[0x04] = (SDL_Color){68, 0, 100, 255};
    ppu->palette[0x05] = (SDL_Color){92, 0, 48, 255};
    ppu->palette[0x06] = (SDL_Color){84, 4, 0, 255};
    ppu->palette[0x07] = (SDL_Color){60, 24, 0, 255};
    ppu->palette[0x08] = (SDL_Color){32, 42, 0, 255};
    ppu->palette[0x09] = (SDL_Color){8, 58, 0, 255};
    ppu->palette[0x0A] = (SDL_Color){0, 64, 0, 255};
    ppu->palette[0x0B] = (SDL_Color){0, 60, 0, 255};
    ppu->palette[0x0C] = (SDL_Color){0, 50, 60, 255};
    ppu->palette[0x0D] = (SDL_Color){0, 0, 0, 255};
    ppu->palette[0x0E] = (SDL_Color){0, 0, 0, 255};
    ppu->palette[0x0F] = (SDL_Color){0, 0, 0, 255};

    ppu->palette[0x10] = (SDL_Color){152, 150, 152, 255};
    ppu->palette[0x11] = (SDL_Color){8, 76, 196, 255};
    ppu->palette[0x12] = (SDL_Color){48, 50, 236, 255};
    ppu->palette[0x13] = (SDL_Color){92, 30, 228, 255};
    ppu->palette[0x14] = (SDL_Color){136, 20, 176, 255};
    ppu->palette[0x15] = (SDL_Color){160, 20, 100, 255};
    ppu->palette[0x16] = (SDL_Color){152, 34, 32, 255};
    ppu->palette[0x17] = (SDL_Color){120, 60, 0, 255};
    ppu->palette[0x18] = (SDL_Color){84, 90, 0, 255};
    ppu->palette[0x19] = (SDL_Color){40, 114, 0, 255};
    ppu->palette[0x1A] = (SDL_Color){8, 124, 0, 255};
    ppu->palette[0x1B] = (SDL_Color){0, 118, 40, 255};
    ppu->palette[0x1C] = (SDL_Color){0, 102, 120, 255};
    ppu->palette[0x1D] = (SDL_Color){0, 0, 0, 255};
    ppu->palette[0x1E] = (SDL_Color){0, 0, 0, 255};
    ppu->palette[0x1F] = (SDL_Color){0, 0, 0, 255};

    ppu->palette[0x20] = (SDL_Color){236, 238, 236, 255};
    ppu->palette[0x21] = (SDL_Color){76, 154, 236, 255};
    ppu->palette[0x22] = (SDL_Color){120, 124, 236, 255};
    ppu->palette[0x23] = (SDL_Color){176, 98, 236, 255};
    ppu->palette[0x24] = (SDL_Color){228, 84, 236, 255};
    ppu->palette[0x25] = (SDL_Color){236, 88, 180, 255};
    ppu->palette[0x26] = (SDL_Color){236, 106, 100, 255};
    ppu->palette[0x27] = (SDL_Color){212, 136, 32, 255};
    ppu->palette[0x28] = (SDL_Color){160, 170, 0, 255};
    ppu->palette[0x29] = (SDL_Color){116, 196, 0, 255};
    ppu->palette[0x2A] = (SDL_Color){76, 208, 32, 255};
    ppu->palette[0x2B] = (SDL_Color){56, 204, 108, 255};
    ppu->palette[0x2C] = (SDL_Color){56, 180, 204, 255};
    ppu->palette[0x2D] = (SDL_Color){60, 60, 60, 255};
    ppu->palette[0x2E] = (SDL_Color){0, 0, 0, 255};
    ppu->palette[0x2F] = (SDL_Color){0, 0, 0, 255};

    ppu->palette[0x30] = (SDL_Color){236, 238, 236, 255};
    ppu->palette[0x31] = (SDL_Color){168, 204, 236, 255};
    ppu->palette[0x32] = (SDL_Color){188, 188, 236, 255};
    ppu->palette[0x33] = (SDL_Color){212, 178, 236, 255};
    ppu->palette[0x34] = (SDL_Color){236, 174, 236, 255};
    ppu->palette[0x35] = (SDL_Color){236, 174, 212, 255};
    ppu->palette[0x36] = (SDL_Color){236, 180, 176, 255};
    ppu->palette[0x37] = (SDL_Color){228, 196, 144, 255};
    ppu->palette[0x38] = (SDL_Color){204, 210, 120, 255};
    ppu->palette[0x39] = (SDL_Color){180, 222, 120, 255};
    ppu->palette[0x3A] = (SDL_Color){168, 226, 144, 255};
    ppu->palette[0x3B] = (SDL_Color){152, 226, 180, 255};
    ppu->palette[0x3C] = (SDL_Color){160, 214, 228, 255};
    ppu->palette[0x3D] = (SDL_Color){160, 162, 160, 255};
    ppu->palette[0x3E] = (SDL_Color){0, 0, 0, 255};
    ppu->palette[0x3F] = (SDL_Color){0, 0, 0, 255};
    
    
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
            data = (ppu->status.reg & 0xE0) | (ppu->ppu_data_buffer & 0x1F);
            ppu->status.vertical_blank = 0;
            ppu->address_latch = 0;
            break;
        case OAM_ADDRESS:
            break;
        case OAM_DATA:
            data = ppu->pOAM[ppu->oam_addr];
            break;
        case SCROLL:
            break;
        case PPU_ADDRESS:
            break;
        case PPU_DATA:
            data = ppu->ppu_data_buffer;
            ppu->ppu_data_buffer = ppu_read(ppu, ppu->vram_addr.reg, true);
            //Case when the palette memory is being read, as that has no delay.
            if(ppu->vram_addr.reg >= MIN_PALETTE_MEMORY)
                data = ppu->ppu_data_buffer;
            ppu->vram_addr.reg += (ppu->control.increment_mode ? 32 : 1);
            break;
        default:
            UNITIALIZED;
    }

    return data;
}

void ppu_cpu_write(PPU2C02* ppu, u_int16_t addr, u_int8_t data) {
    switch(addr){
        case CONTROL:
            ppu->control.reg = data;
            ppu->tram_addr.nametable_x = ppu->control.nametable_x;
            ppu->tram_addr.nametable_y = ppu->control.nametable_y;
            break;
        case MASK:
            ppu->mask.reg = data;
            break;
        case STATUS:
            break;
        case OAM_ADDRESS:
            ppu->oam_addr = data;
            break;
        case OAM_DATA:
            ppu->pOAM[ppu->oam_addr] = data;
            //ppu->oam_addr++; //temp
            break;
        case SCROLL:
            if(ppu->address_latch == 0){
                ppu->fine_x = data & 0x07;
                ppu->tram_addr.coarse_x = data >> 3;
                ppu->address_latch = 1;
            }
            else{
                ppu->tram_addr.fine_y = data & 0x07;
                ppu->tram_addr.coarse_y = data >> 3;
                ppu->address_latch = 0;
            }
            break;
        case PPU_ADDRESS:
            if(ppu->address_latch == 0){
                ppu->tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (ppu->tram_addr.reg & LOW_BIT_MASK);
                ppu->address_latch = 1;
            }
            else{
                ppu->tram_addr.reg = (ppu->tram_addr.reg & HIGH_BIT_MASK) | data;
                ppu->vram_addr = ppu->tram_addr;
                ppu->address_latch = 0;
            }
            break;
        case PPU_DATA:
            ppu_write(ppu, ppu->vram_addr.reg, data);
            ppu->vram_addr.reg += (ppu->control.increment_mode ? 32 : 1);
            break;
        default:
            UNITIALIZED;
    }
}

u_int8_t ppu_read(PPU2C02* ppu, u_int16_t addr, bool read_only) {
    u_int8_t data = ZERO;
    addr &= MAX_ADDRESS_PPU;

    if(cart_read_ppu(ppu->cart, addr, &data)){
        //Do nothing..
    }
    else if(addr >= 0x0000 && addr <= MAX_PATTERN_MEMORY){
        data = ppu->tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
    }
    else if(addr >= MIN_NAMETABLE_MEMORY && addr <= MAX_NAMETABLE_MEMORY){
        //Nametable mirroring (scrolling)
        addr &= 0x0FFF;
        if(cart_mirror(ppu->cart) == VERTICAL){
            if(addr >= 0x0000 && addr <= 0x03FF)
                data = ppu->tblName[0][addr & 0x03FF];
            if(addr >= 0x0400 && addr <= 0x07FF)
                data = ppu->tblName[1][addr & 0x03FF];
            if(addr >= 0x0800 && addr <= 0x0BFF)
                data = ppu->tblName[0][addr & 0x03FF];
            if(addr >= 0x0C00 && addr <= 0x0FFF)
                data = ppu->tblName[1][addr & 0x03FF];
        }
        else if(cart_mirror(ppu->cart) == HORIZONTAL){
            if(addr >= 0x0000 && addr <= 0x03FF)
                data = ppu->tblName[0][addr & 0x03FF];
            if(addr >= 0x0400 && addr <= 0x07FF)
                data = ppu->tblName[0][addr & 0x03FF];
            if(addr >= 0x0800 && addr <= 0x0BFF)
                data = ppu->tblName[1][addr & 0x03FF];
            if(addr >= 0x0C00 && addr <= 0x0FFF)
                data = ppu->tblName[1][addr & 0x03FF];
        }
    }
    else if(addr >= MIN_PALETTE_MEMORY && addr <= MAX_PALETTE_MEMORY){
        addr &= 0x001F;
        //Mirroring
        if(addr == 0x0010) addr = 0x0000;
        if(addr == 0x0014) addr = 0x0004;
        if(addr == 0x0018) addr = 0x0008;
        if(addr == 0x001C) addr = 0x000C;
        data = ppu->tblPalette[addr] & (ppu->mask.grayscale ? 0x30 : 0x3F);
    }

    return data;
}

void ppu_write(PPU2C02* ppu, u_int16_t addr, u_int8_t data) {
    addr &= MAX_ADDRESS_PPU;
    if(cart_write_ppu(ppu->cart, addr, data)){
        //Do nothing..
    }
    else if(addr >= 0x0000 && addr <= MAX_PATTERN_MEMORY){
        ppu->tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    }
    else if(addr >= MIN_NAMETABLE_MEMORY && addr <= MAX_NAMETABLE_MEMORY){
        //Nametable mirroring (scrolling)
        addr &= 0x0FFF;
        if(cart_mirror(ppu->cart) == VERTICAL){
            if(addr >= 0x0000 && addr <= 0x03FF)
                ppu->tblName[0][addr & 0x03FF] = data;
            if(addr >= 0x0400 && addr <= 0x07FF)
                ppu->tblName[1][addr & 0x03FF] = data;
            if(addr >= 0x0800 && addr <= 0x0BFF)
                ppu->tblName[0][addr & 0x03FF] = data;
            if(addr >= 0x0C00 && addr <= 0x0FFF)
                ppu->tblName[1][addr & 0x03FF] = data;
        }
        else if(cart_mirror(ppu->cart) == HORIZONTAL){
            if(addr >= 0x0000 && addr <= 0x03FF)
                ppu->tblName[0][addr & 0x03FF] = data;
            if(addr >= 0x0400 && addr <= 0x07FF)
                ppu->tblName[0][addr & 0x03FF] = data;
            if(addr >= 0x0800 && addr <= 0x0BFF)
                ppu->tblName[1][addr & 0x03FF] = data;
            if(addr >= 0x0C00 && addr <= 0x0FFF)
                ppu->tblName[1][addr & 0x03FF] = data;
        }
    }
    else if(addr >= MIN_PALETTE_MEMORY && addr <= MAX_PALETTE_MEMORY){
        addr &= 0x001F;
        //Mirroring
        if(addr == 0x0010) addr = 0x0000;
        if(addr == 0x0014) addr = 0x0004;
        if(addr == 0x0018) addr = 0x0008;
        if(addr == 0x001C) addr = 0x000C;
        ppu->tblPalette[addr] = data;
    }
}

void ppu_connect_cartridge(PPU2C02* ppu, Cartridge* cartridge) {
    ppu->cart = cartridge;
}

SDL_Color color_from_pal_ram(PPU2C02* ppu, uint8_t palette, uint8_t pixel){
    return ppu->palette[ppu_read(ppu, 0x3F00 + (palette << 2) + pixel, true)];
}

void ppu_clock(PPU2C02* ppu){
    if(ppu->scanline >= -1 && ppu->scanline < 240) {
        if(ppu->scanline == 0 && ppu->cycle == 0 && ppu->odd_frame && (ppu->mask.render_background || ppu->mask.render_sprites)){
            ppu->cycle = 1;
        }
        if(ppu->scanline == -1 && ppu->cycle == 1) {
            ppu->status.vertical_blank = 0;
            ppu->status.sprite_zero_hit = 0;
            ppu->status.sprite_overflow = 0;

            for(int32_t i = 0; i < 8; i++){
                ppu->sprite_shifter_pattern_lo[i] = 0;
                ppu->sprite_shifter_pattern_hi[i] = 0;
            }
        }
        if((ppu->cycle >= 2 && ppu->cycle < 258) || (ppu->cycle >= 321 && ppu->cycle < 338)){
            update_shifters(ppu);
            switch((ppu->cycle - 1) % 8){
                case 0:
                    load_background_shifters(ppu);
                    ppu->bg_next_tile_id = ppu_read(ppu, 0x2000 | (ppu->vram_addr.reg & 0x0FFF), true);
                    break;
                case 2:
                    ppu->bg_next_tile_attrib = ppu_read(ppu, 0x23C0 |
                            (ppu->vram_addr.nametable_y << 11)      |
                            (ppu->vram_addr.nametable_x << 10)      |
                            ((ppu->vram_addr.coarse_y >> 2) << 3)   |
                            (ppu->vram_addr.coarse_x >> 2), true
                    );
                    if(ppu->vram_addr.coarse_y & 0x02) ppu->bg_next_tile_attrib >>= 4;
                    if(ppu->vram_addr.coarse_x & 0x02) ppu->bg_next_tile_attrib >>= 2;
                    ppu->bg_next_tile_attrib &= 0x03;
                    break;
                case 4:
                    ppu->bg_next_tile_lsb = ppu_read(ppu,
                                                     (ppu->control.pattern_background << 12) +
                                                     ((uint16_t)ppu->bg_next_tile_id << 4) +
                                                     (ppu->vram_addr.fine_y + 0), true
                    );
                    break;
                case 6:
                    ppu->bg_next_tile_msb = ppu_read(ppu,
                                                     (ppu->control.pattern_background << 12) +
                                                     ((uint16_t)ppu->bg_next_tile_id << 4) +
                                                     (ppu->vram_addr.fine_y + 8), true
                    );
                    break;
                case 7:
                    increment_scroll_x(ppu);
                    break;
            }
        }
        if(ppu->cycle == 256){
            increment_scroll_y(ppu);
        }
        if(ppu->cycle == 257){
            load_background_shifters(ppu);
            transfer_address_x(ppu);
        }
        if (ppu->cycle == 338 || ppu->cycle == 340){
            ppu->bg_next_tile_id = ppu_read(ppu, 0x2000 | (ppu->vram_addr.reg & 0x0FFF), true);
        }
        if(ppu->scanline == -1 && ppu->cycle >= 280 && ppu->cycle < 305){
            transfer_address_y(ppu);
        }

        //Foreground rendering (This is not done in parallel and not necessarily cycle accurate).
        //Sprite evaluation phase.
        if(ppu->cycle == 257 && ppu->scanline >= 0){
            memset(ppu->sprite_scanline, 0xFF, 8 * sizeof(struct sObjectAttributeEntry));
            ppu->sprite_count = 0;
            for (uint8_t i = 0; i < 8; i++){
                ppu->sprite_shifter_pattern_lo[i] = 0;
                ppu->sprite_shifter_pattern_hi[i] = 0;
            }

            uint8_t oam_entries = 0;
            ppu->sprite_zero_hit_possible = false;

            while(oam_entries < 64 && ppu->sprite_count < 9){
                int16_t diff = (((int16_t)ppu->scanline) - (int16_t)(ppu->OAM[oam_entries].y));
                if(diff >= 0 && diff < (ppu->control.sprite_size ? 16 : 8) && ppu->sprite_count < 8){
                    if(ppu->sprite_count < 8){
                        //Check if it's sprite zero
                        if(oam_entries == 0){
                            ppu->sprite_zero_hit_possible = true;
                        }

                        memcpy(&ppu->sprite_scanline[ppu->sprite_count], &ppu->OAM[oam_entries], sizeof(struct sObjectAttributeEntry));
                    }
                    ppu->sprite_count++;
                }
                oam_entries++;
            }
            ppu->status.sprite_overflow = (ppu->sprite_count >= 8);
        }

        if(ppu->cycle == 340){
            for(uint8_t i = 0; i < ppu->sprite_count; i++){
                uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
                uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;

                //8x8 sprites
                if(!ppu->control.sprite_size){
                    //No vertical flip
                    if(!(ppu->sprite_scanline[i].attribute & 0x80)){
                        sprite_pattern_addr_lo =
                                (ppu->control.pattern_sprite << 12) |
                                (ppu->sprite_scanline[i].id << 4)   |
                                (ppu->scanline - ppu->sprite_scanline[i].y);
                    }
                    else{
                        sprite_pattern_addr_lo =
                                (ppu->control.pattern_sprite << 12) |
                                (ppu->sprite_scanline[i].id << 4)   |
                                (7 - (ppu->scanline - ppu->sprite_scanline[i].y));
                    }
                }
                //8x16 sprites
                else{
                    //No vertical flip
                    if(!(ppu->sprite_scanline[i].attribute & 0x80)){
                        if(ppu->scanline - ppu->sprite_scanline[i].y < 8){
                            //Top half of tile
                            sprite_pattern_addr_lo =
                                    ((ppu->sprite_scanline[i].id & 0x01) << 12) |
                                    ((ppu->sprite_scanline[i].id & 0xFE) << 4)  |
                                    ((ppu->scanline - ppu->sprite_scanline[i].y) & 0x07);
                        }
                        else{
                            //Bottom half of tile
                            sprite_pattern_addr_lo =
                                    ((ppu->sprite_scanline[i].id & 0x01) << 12) |
                                    (((ppu->sprite_scanline[i].id & 0xFE)+1) << 4)  |
                                    ((ppu->scanline - ppu->sprite_scanline[i].y) & 0x07);
                        }
                    }
                    else{
                        //Vertical flip
                        if(ppu->scanline - ppu->sprite_scanline[i].y < 8){
                            sprite_pattern_addr_lo =
                                    ((ppu->sprite_scanline[i].id & 0x01) << 12) |
                                    (((ppu->sprite_scanline[i].id & 0xFE) + 1) << 4) |
                                    (7 - (ppu->scanline - ppu->sprite_scanline[i].y) & 0x07);
                        }
                        else{
                            sprite_pattern_addr_lo =
                                    ((ppu->sprite_scanline[i].id & 0x01) << 12) |
                                    ((ppu->sprite_scanline[i].id & 0xFE) << 4) |
                                    (7 - (ppu->scanline - ppu->sprite_scanline[i].y) & 0x07);
                        }
                    }
                }

                sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;
                sprite_pattern_bits_lo = ppu_read(ppu, sprite_pattern_addr_lo, true);
                sprite_pattern_bits_hi = ppu_read(ppu, sprite_pattern_addr_hi, true);

                //Horizontal Sprite Flip
                if(ppu->sprite_scanline[i].attribute & 0x40){
                    sprite_pattern_bits_lo = byte_flip(sprite_pattern_bits_lo);
                    sprite_pattern_bits_hi = byte_flip(sprite_pattern_bits_hi);
                }

                ppu->sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
                ppu->sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
            }
        }

    }

    if(ppu->scanline == 240){
        //Nothing.
    }

    if(ppu->scanline >= 241 && ppu->scanline < 261){
        if(ppu->scanline == 241 && ppu->cycle == 1){
            ppu->status.vertical_blank = 1;
            if(ppu->control.enable_nmi)
                ppu->nmi = true;
        }
    }

    //Finally we can draw!
    uint8_t bg_pixel = 0x00;
    uint8_t bg_palette = 0x00;

    //Render background.
    if(ppu->mask.render_background){
        if(ppu->mask.render_background_left || (ppu->cycle >= 9)) {
            uint16_t bit_mux = 0x8000 >> ppu->fine_x;
            uint8_t p0_pixel = (ppu->bg_shifter_pattern_lo & bit_mux) > 0;
            uint8_t p1_pixel = (ppu->bg_shifter_pattern_hi & bit_mux) > 0;
            //Pixel index
            bg_pixel = (p1_pixel << 1) | p0_pixel;

            //Get palette
            uint8_t bg_pal0 = (ppu->bg_shifter_attrib_lo & bit_mux) > 0;
            uint8_t bg_pal1 = (ppu->bg_shifter_attrib_hi & bit_mux) > 0;
            bg_palette = (bg_pal1 << 1) | bg_pal0;
        }
    }

    //Render Sprites
    uint8_t fg_pixel = 0x00;
    uint8_t fg_palette = 0x00;
    uint8_t fg_priority = 0x00;

    if(ppu->mask.render_sprites){
        if(ppu->mask.render_sprites_left || (ppu->cycle >= 9)) {
            ppu->sprite_zero_being_rendered = false;

            for (uint8_t i = 0; i < ppu->sprite_count; i++) {
                if (ppu->sprite_scanline[i].x == 0) {
                    uint8_t fg_pixel_lo = (ppu->sprite_shifter_pattern_lo[i] & 0x80) > 0;
                    uint8_t fg_pixel_hi = (ppu->sprite_shifter_pattern_hi[i] & 0x80) > 0;
                    fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

                    fg_palette = (ppu->sprite_scanline[i].attribute & 0x03) + 0x04;
                    fg_priority = (ppu->sprite_scanline[i].attribute & 0x20) == 0;

                    //It's not transparent!
                    if (fg_pixel != 0) {
                        if (i == 0) {
                            ppu->sprite_zero_being_rendered = true;
                        }
                        break;
                    }
                }
            }
        }
    }

    //Finally draw the sprite...
    uint8_t pixel = 0x00;
    uint8_t palette = 0x00;

    if(bg_pixel == 0 && fg_pixel == 0){
        //If the bg and fg pixels are transparent, then draw the background.
        pixel = 0x00;
        palette = 0x00;
    }
    else if(bg_pixel == 0 && fg_pixel > 0){
        //The background is transparent the foreground is not, draw the foreground pixel.
        pixel = fg_pixel;
        palette = fg_palette;
    }
    else if(bg_pixel > 0 && fg_pixel == 0){
        //The background is not transparent and the foreground is, draw the background.
        pixel = bg_pixel;
        palette = bg_palette;
    }
    else if(bg_pixel > 0 && fg_pixel > 0){
        //If both are not transparent, we rely on the priority flag we set to figure out which to draw.
        if(fg_priority){
            pixel = fg_pixel;
            palette = fg_palette;
        }
        else{
            pixel = bg_pixel;
            palette = bg_palette;
        }

        if(ppu->sprite_zero_being_rendered && ppu->sprite_zero_hit_possible){
            if(ppu->mask.render_background & ppu->mask.render_sprites){
                if(!(ppu->mask.render_background_left | ppu->mask.render_sprites_left)){
                    if(ppu->cycle >= 9 && ppu->cycle < 258){
                        ppu->status.sprite_zero_hit = 1;
                    }
                }
                else{
                    if(ppu->cycle >= 1 && ppu->cycle < 258){
                        ppu->status.sprite_zero_hit = 1;
                    }
                }
            }
        }
    }

    //WE CAN DRAW IT THANK GOD
    if(ppu->scanline >= 0 && ppu->scanline < NES_HEIGHT && ppu->cycle >= 0 && ppu->cycle < NES_WIDTH){
        ppu->sprite_screen->sprite_data[ppu->scanline][ppu->cycle] = color_from_pal_ram(ppu, palette, pixel);
    }

    ppu->cycle++;

    if(ppu->mask.render_background || ppu->mask.render_sprites){
        if(ppu->cycle == 260 && ppu->scanline < 240){
            cart_scanline(ppu->cart);
        }
    }

    if(ppu->cycle >= 341){
        ppu->cycle = 0;
        ppu->scanline++;
        if(ppu->scanline >= 261){
            ppu->scanline = -1;
            ppu->frame_complete = true;
            ppu->odd_frame = !ppu->odd_frame;
        }
    }

}

sprite_t* get_pattern_table(PPU2C02* ppu, uint8_t i, uint8_t palette){
    for(uint16_t tileY = 0; tileY < 16; tileY++){
        for(uint16_t tileX = 0; tileX < 16; tileX++){
            uint16_t offset = tileY * 256 + tileX * 16;

            for(uint16_t row = 0; row < 8; row++){
                uint8_t tile_lsb = ppu_read(ppu, i * 0x1000 + offset + row + 0, true);
                uint8_t tile_msb = ppu_read(ppu, i * 0x1000 + offset + row + 8, true);

                for(uint16_t col = 0; col < 8; col++){
                    uint8_t pixel = ((tile_lsb & 0x01) << 1) | (tile_msb & 0x01);
                    tile_lsb >>= 1;
                    tile_msb >>= 1;
                    ppu->sprite_patterntable[i]->sprite_data[tileY * 8 + row][tileX * 8 + (7 - col)] = color_from_pal_ram(ppu, palette, pixel);
                }
            }
        }
    }

    return ppu->sprite_patterntable[i];
}

uint8_t byte_flip(uint8_t b){
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

void increment_scroll_x(PPU2C02* ppu){
    if(ppu->mask.render_background || ppu->mask.render_sprites){
        if(ppu->vram_addr.coarse_x == 31){
            ppu->vram_addr.coarse_x = 0;
            ppu->vram_addr.nametable_x = ~ppu->vram_addr.nametable_x;
        }
        else{
            ppu->vram_addr.coarse_x++;
        }
    }
}
void increment_scroll_y(PPU2C02* ppu){
    if(ppu->mask.render_background || ppu->mask.render_sprites){
        if(ppu->vram_addr.fine_y < 7){
            ppu->vram_addr.fine_y++;
        }
        else{
            ppu->vram_addr.fine_y = 0;
            if(ppu->vram_addr.coarse_y == 29){
                ppu->vram_addr.coarse_y = 0;
                ppu->vram_addr.nametable_y = ~ppu->vram_addr.nametable_y;
            }
            else if(ppu->vram_addr.coarse_y == 31){
                ppu->vram_addr.coarse_y = 0;
            }
            else{
                ppu->vram_addr.coarse_y++;
            }
        }
    }
}

void transfer_address_x(PPU2C02* ppu){
    if(ppu->mask.render_background || ppu->mask.render_sprites){
        ppu->vram_addr.nametable_x = ppu->tram_addr.nametable_x;
        ppu->vram_addr.coarse_x = ppu->tram_addr.coarse_x;
    }
}

void transfer_address_y(PPU2C02* ppu){
    if(ppu->mask.render_background || ppu->mask.render_sprites){
        ppu->vram_addr.fine_y = ppu->tram_addr.fine_y;
        ppu->vram_addr.nametable_y = ppu->tram_addr.nametable_y;
        ppu->vram_addr.coarse_y = ppu->tram_addr.coarse_y;
    }
}

void load_background_shifters(PPU2C02* ppu){
    ppu->bg_shifter_pattern_lo = (ppu->bg_shifter_pattern_lo & 0xFF00) | ppu->bg_next_tile_lsb;
    ppu->bg_shifter_pattern_hi = (ppu->bg_shifter_pattern_hi & 0xFF00) | ppu->bg_next_tile_msb;
    ppu->bg_shifter_attrib_lo  = (ppu->bg_shifter_attrib_lo & 0xFF00) | ((ppu->bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
    ppu->bg_shifter_attrib_hi  = (ppu->bg_shifter_attrib_hi & 0xFF00) | ((ppu->bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
}

void update_shifters(PPU2C02* ppu){
    if(ppu->mask.render_background){
        ppu->bg_shifter_pattern_lo <<= 1;
        ppu->bg_shifter_pattern_hi <<= 1;
        ppu->bg_shifter_attrib_lo <<= 1;
        ppu->bg_shifter_attrib_hi <<= 1;
    }
    if(ppu->mask.render_sprites && ppu->cycle >= 1 && ppu->cycle < 258){
        for(int32_t i = 0; i < ppu->sprite_count; i++){
            if(ppu->sprite_scanline[i].x > 0){
                ppu->sprite_scanline[i].x--;
            }
            else{
                ppu->sprite_shifter_pattern_lo[i] <<= 1;
                ppu->sprite_shifter_pattern_hi[i] <<= 1;
            }
        }
    }
}
