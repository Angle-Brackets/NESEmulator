#include "title/title.h"
#include "../NES/nes.h"

void draw() {
    switch(emulator_state.state){
        case TITLE_SCREEN:
            draw_title_background();
            draw_title_sprites();
            draw_title();
            break;
        case MAIN_MENU:
            initialize_nes(true);
            emulator_state.state = NES;
            break;
        case NES:
            run_nes_emulation();
            break;
        default:
            break;
    }

    emulator_state.frames_elapsed = (emulator_state.frames_elapsed + 1) % 60000;
}

i32 main(){
    i32 width, height;
    initialize(1920, 1080, 1920, 1080, 60, 15, 1, MULTITHREADING_ENABLED | BITMAP_ACTIVE | IGNORE_FOCUS, 0, draw);
    SDL_SetWindowFullscreen(global.render.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GetWindowSize(global.render.window, &width, &height);
    resize_bitmap(width, height);
    set_window_size(width, height);
    //Fonts
    default_font = load_pix_font("../assets/pixel-emulator-font/PixelEmulator-xq08.ttf", 20, (SDL_Color){255,255,255,255});
    set_default_font(default_font);

    initialize_title();

    start();

    return 0;
}


