#include "nes.h"

static Cartridge cart;
static Bus nes;

static bool debug_enabled = false;
static bool emulation_active = false;
static f32 residual_time = 0.0f;
static u32 selected_palette = 0;

SDL_Color Black = {0,0,0,255};
SDL_Color White = {255,0,255,255};
SDL_Color BG = {186,148,108};
SDL_Color RED = {255,0,0,255};
SDL_Color GREEN = {0,255,0,255};

void update_controller(){
    nes.controller[0] = 0x00;
    nes.controller[0] |= get_key_state_str("X") & KS_HELD ? 0x80 : 0x00; //A
    nes.controller[0] |= get_key_state_str("Z") & KS_HELD ? 0x40 : 0x00; //B
    nes.controller[0] |= get_key_state_str("A") & KS_HELD ? 0x20 : 0x00; //Select
    nes.controller[0] |= get_key_state_str("S") & KS_HELD ? 0x10 : 0x00; //Start
    nes.controller[0] |= get_key_state_scancode(SDL_SCANCODE_UP) & KS_HELD ? 0x08 : 0x00;
    nes.controller[0] |= get_key_state_scancode(SDL_SCANCODE_DOWN) & KS_HELD ? 0x04 : 0x00;
    nes.controller[0] |= get_key_state_scancode(SDL_SCANCODE_LEFT) & KS_HELD ? 0x02 : 0x00;
    nes.controller[0] |= get_key_state_scancode(SDL_SCANCODE_RIGHT) & KS_HELD ? 0x01 : 0x00;
    if(SDL_NumJoysticks() > 0) {
        //Xbox Layout, change if you want.
        nes.controller[0] |= get_button_state(&global.controller.controllers[0], SDL_CONTROLLER_BUTTON_A) & CON_HELD ? 0x80 : 0x00;
        nes.controller[0] |= get_button_state(&global.controller.controllers[0], SDL_CONTROLLER_BUTTON_X) & CON_HELD ? 0x40 : 0x00;
        nes.controller[0] |= get_button_state(&global.controller.controllers[0], SDL_CONTROLLER_BUTTON_GUIDE) & CON_HELD ? 0x20 : 0x00;
        nes.controller[0] |= get_button_state(&global.controller.controllers[0], SDL_CONTROLLER_BUTTON_START) & CON_HELD ? 0x10 : 0x00;
        nes.controller[0] |= get_button_state(&global.controller.controllers[0], SDL_CONTROLLER_BUTTON_DPAD_UP) & CON_HELD ? 0x08 : 0x00;
        nes.controller[0] |= get_button_state(&global.controller.controllers[0], SDL_CONTROLLER_BUTTON_DPAD_DOWN) & CON_HELD ? 0x04 : 0x00;
        nes.controller[0] |= get_button_state(&global.controller.controllers[0], SDL_CONTROLLER_BUTTON_DPAD_LEFT) & CON_HELD ? 0x02 : 0x00;
        nes.controller[0] |= get_button_state(&global.controller.controllers[0], SDL_CONTROLLER_BUTTON_DPAD_RIGHT) & CON_HELD ? 0x01 : 0x00;
    }
}



void draw_cpu(i32 x, i32 y){
    FC_SetDefaultColor(get_default_font(), White);
    render_text(NULL, x , y , RIGHT, "STATUS:");
    FC_SetDefaultColor(get_default_font(), nes.cpu->status & N ? GREEN : RED);
    render_text(NULL, x  + 64, y, RIGHT, "N");
    FC_SetDefaultColor(get_default_font(), nes.cpu->status & V ? GREEN : RED);
    render_text(NULL, x  + 80, y , RIGHT, "V");
    FC_SetDefaultColor(get_default_font(), nes.cpu->status & U ? GREEN : RED);
    render_text(NULL, x  + 96, y , RIGHT, "-");
    FC_SetDefaultColor(get_default_font(), nes.cpu->status & B ? GREEN : RED);
    render_text(NULL, x  + 112, y , RIGHT, "B");
    FC_SetDefaultColor(get_default_font(), nes.cpu->status & D ? GREEN : RED);
    render_text(NULL, x  + 128, y , RIGHT, "D");
    FC_SetDefaultColor(get_default_font(), nes.cpu->status & I ? GREEN : RED);
    render_text(NULL, x  + 144, y , RIGHT, "I");
    FC_SetDefaultColor(get_default_font(), nes.cpu->status & Z ? GREEN : RED);
    render_text(NULL, x  + 160, y , RIGHT, "Z");
    FC_SetDefaultColor(get_default_font(), nes.cpu->status & C ? GREEN : RED);
    render_text(NULL, x  + 178, y , RIGHT, "C");
}

void initialize_nes(bool debug, enum Emulator_State* emulator_state){
    debug_enabled = debug;
    if(debug_enabled){
        //Debug output
        //initialize(WIDTH, HEIGHT, WIDTH, HEIGHT, 60, 15, 1, MULTITHREADING_ENABLED | BITMAP_ACTIVE | IGNORE_FOCUS, 0, draw);
    }
    else {
        //Standard Output
        //initialize(NES_WIDTH * 4, NES_HEIGHT * 4, NES_WIDTH, NES_HEIGHT, 60, 15, 1, MULTITHREADING_ENABLED | BITMAP_ACTIVE | IGNORE_FOCUS, 0, draw);
    }

    initialize_cartridge(&cart, "../roms/kirby.nes");
    insert_cartridge(&nes, &cart);
    bus_reset(&nes);
    *emulator_state = NES;
}

void run_nes_emulation(){
    if(get_key_state_str("R") & KS_PRESSED){
        bus_reset(&nes);
    }

    if(get_key_state_scancode(SDL_SCANCODE_SPACE) & KS_PRESSED){
        emulation_active = !emulation_active;
    }

    if(get_key_state_str("Escape") & KS_PRESSED){
        exit_program();
    }

    //Crops the bitmap
    if(get_key_state_str("K") & KS_PRESSED && global.bitmap.width > 0){
        nes.ppu->sprite_screen->x--;
        resize_bitmap(global.bitmap.width - 1, global.bitmap.height);
    }

    if(get_key_state_str("L") & KS_PRESSED && global.bitmap.width < NES_WIDTH){
        nes.ppu->sprite_screen->x++;
        resize_bitmap(global.bitmap.width + 1, global.bitmap.height);
    }


    fill_background(&BG);
    update_controller();
    if(emulation_active){
        if(residual_time > 0.0f){
            residual_time -= global.time.delta;
        }
        else {
            residual_time += (1.0f/60.0f) - global.time.delta;
            do { bus_clock(&nes); } while(!nes.ppu->frame_complete);
            nes.ppu->frame_complete = false;
        }
    }
    else if(debug_enabled) {
        if (get_key_state_str("C") & KS_PRESSED) {
            do { bus_clock(&nes); } while (nes.cpu->cycles != 0);
            do { bus_clock(&nes); } while (nes.cpu->cycles == 0);
        }
    }

    if(get_key_state_str("P") & KS_PRESSED){
        selected_palette++;
        selected_palette &= 0x07;
        printf("%i\n", selected_palette);
    }

    draw_sprite_to_bitmap(nes.ppu->sprite_screen, 1);

    if(debug_enabled) {
        draw_sprite_to_bitmap(get_pattern_table(nes.ppu, 0, selected_palette), 1);
        draw_sprite_to_bitmap(get_pattern_table(nes.ppu, 1, selected_palette), 1);
    }

    draw_bitmap();

    if(debug_enabled) {
        draw_cpu(global.render.width - 190, 0);
        FC_SetDefaultColor(get_default_font(), White);
        render_text(NULL, global.render.width, 125, RIGHT, "Updates: %u\nFPS: %u\n", global.bitmap.bitmap_updates, global.time.frame_rate)
        render_text(NULL, global.render.width, 50, RIGHT, "PC: 0x%04x", nes.cpu->pc);
        render_text(NULL, global.render.width, 75, RIGHT, "STKP: 0x%04x", nes.cpu->stkp);
        render_text(NULL, global.render.width, 100, RIGHT, "A: %u, X: %u, Y: %u", nes.cpu->a, nes.cpu->x, nes.cpu->y);
    }

    static char buffer[20];
    snprintf(buffer, 20, "NES - %u", global.time.frame_rate);
    SDL_SetWindowTitle(global.render.window, buffer);
}
