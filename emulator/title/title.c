#include <time.h>
#include "title.h"

static sprite_t *play_btn = NULL, *settings_btn = NULL;
static sprite_sheet* title_sprite_sheet = NULL;
static PIX_Font* title_font = NULL;
static SDL_Color bad_colors[3] = {{116, 154, 212, 255}, {41, 88, 124, 255}, {255,0,215,255}};



void initialize_title(){
    title_font = load_pix_font("../assets/pixel-emulator-font/PixelEmulator-xq08.ttf", 50, (SDL_Color){255,255,255,255});
    srand(time(NULL));
    if(play_btn == NULL){
        play_btn = create_sprite_from_img(0, 0, "../assets/icons/ButtonPlay.png");
        play_btn->x = (global.render.width / 2) - (play_btn->width / 2) * BUTTON_SCALE;
        play_btn->y = global.render.height * 0.7;
    }

    if(settings_btn == NULL){
        settings_btn = create_sprite_from_img(0, 0, "../assets/icons/ButtonSettings.png");
        settings_btn->x = (global.render.width / 2) - (settings_btn->width / 2) * BUTTON_SCALE;
        settings_btn->y = global.render.height * 0.7 + play_btn->height * (BUTTON_SCALE+2);
    }
}

static void button_select(sprite_t* button, i32 state, u32 replaced_color){
    typedef struct Tuple {
        u32 x;
        u32 y;
    } tuple_t;

    static sprite_t* current_button = NULL;
    static SDL_Color outline = {204, 209, 224, 255};
    static bool outline_state = true; //True means its darkening, False its brightening.
    static u32 current_replaced_color = 0;
    static i32 current_state = -1;
    static tuple_t* indexes = NULL;
    static u32 index_len = 0;

    if(indexes == NULL || state != current_state){
        if(indexes != NULL && current_button != NULL) {
            //Restores the previously modified button to its default state.
            for(u32 i = 0; i < index_len; i++){
                current_button->sprite_data[indexes[i].y][indexes[i].x] = *(SDL_Color*)&current_replaced_color;
            }
            free(indexes);
        }
        index_len = 0;
        indexes = malloc(sizeof(tuple_t) * button->width * button->height);
        for(u32 i = 0; i < button->height; i++){
            for(u32 j = 0; j < button->width; j++){
                if(*(u32*)&button->sprite_data[i][j] == replaced_color){
                    indexes[index_len++] = (tuple_t){j, i};
                }
            }
        }
        outline_state = true;
        current_button = button;
        current_replaced_color = replaced_color;
        current_state = state;
    }

    for(u32 i = 0; i < index_len; i++){
        button->sprite_data[indexes[i].y][indexes[i].x] = outline;
    }

    if(outline_state){
        outline.a--;
        if(outline.a <= 170)
            outline_state = !outline_state;
    }
    else{
        outline.a++;
        if(outline.a >= 255)
            outline_state = !outline_state;
    }

}

void draw_title_background(){
#define GRID
    static u32** background = NULL;
    static SDL_Color pal[32] = {
            {0, 0, 117, 255},
            {89, 21, 207, 255},
            {87, 20, 204, 255},
            {85, 20, 201, 255},
            {83, 19, 198, 255},
            {81, 19, 195, 255},
            {79, 18, 192, 255},
            {77, 18, 189, 255},
            {75, 17, 186, 255},
            {73, 17, 183, 255},
            {71, 16, 180, 255},
            {69, 16, 177, 255},
            {67, 15, 174, 255},
            {65, 15, 171, 255},
            {63, 14, 168, 255},
            {61, 14, 165, 255},
            {59, 13, 162, 255},
            {57, 13, 159, 255},
            {55, 12, 156, 255},
            {53, 12, 153, 255},
            {51, 11, 150, 255},
            {49, 11, 147, 255},
            {47, 10, 144, 255},
            {45, 10, 141, 255},
            {43, 9, 138, 255},
            {41, 9, 135, 255},
            {39, 8, 132, 255},
            {37, 8, 129, 255},
            {35, 7, 126, 255},
            {33, 7, 123, 255},
            {25, 6, 120, 255},
            {0, 0, 117, 255}
    };

    // Define the value of N (the number of pixels between color changes)
    static u32 N = 16;

    if (background == NULL) {
        background = (u32**)malloc(sizeof(u32*) * global.bitmap.height);
        for (u32 i = 0; i < global.bitmap.height; i++) {
            background[i] = (u32 *)malloc(sizeof(u32) * global.bitmap.width);
        }

#ifdef GRID
        N = 36;
        for (u32 i = 0; i < global.bitmap.height; i++) {
            for (u32 j = 0; j < global.bitmap.width; j++) {
                // Calculate whether to alternate colors every N pixels
                if ((i / N + j / N) % 2 == 0) {
                    background[i][j] = 0; // Set to color 0 (or your first color)
                } else {
                    background[i][j] = 1; // Set to color 1 (or your second color)
                }
            }
        }
#else
        N = 16;
        for (u32 i = 0; i < global.bitmap.height; i++) {
            for (u32 j = 0; j < global.bitmap.width; j++) {
                // Calculate the scaled coordinates
                u32 scaledI = i / N;
                u32 scaledJ = j / N;

                u32 minDistanceVertical = scaledI < global.bitmap.height / N - scaledI - 1 ? scaledI : global.bitmap.height / N - scaledI - 1;
                u32 minDistanceHorizontal = scaledJ < global.bitmap.width / N - scaledJ - 1 ? scaledJ : global.bitmap.width / N - scaledJ - 1;
                u32 minDistance = minDistanceVertical < minDistanceHorizontal ? minDistanceVertical : minDistanceHorizontal;

                // Calculate the value with looping from 15 to 0
                background[i][j] = 31 - (minDistance % 32);
            }
        }
#endif
    }

    static float frequency = 0.05f;  // Adjust this value to control the speed of oscillation
    static float amplitude = 5.0f;  // Adjust this value to control the amplitude of oscillation
    static float time = 0.0f;       // Initialize time to 0
    static float time_offset = 0.1f;

    for (u32 i = 0; i < global.bitmap.height; i++) {
        for (u32 j = 0; j < global.bitmap.width; j++) {
            // Calculate the x-coordinate with oscillation
            float oscillation = amplitude * sinf(2.0f * 3.14195f * frequency * time);
            draw_pixel(&pal[background[i][j]], j + oscillation, i);
        }
        time += time_offset;  // Adjust this value to control the speed of oscillation
        if(time > time_offset * global.bitmap.height){
            time = 0.0f;
        }
    }
}

void draw_title_sprites(){
    //WIP, I will add the rest later.
    static title_sprite_t mario1 = {0};
    static title_sprite_t link = {0};
    static title_sprite_t kirby = {0};
    static title_sprite_t kirby_parasol = {0};
    static title_sprite_t mario3 = {0};
    static title_sprite_t mario3_racoon = {0};
    static title_sprite_t shell = {0};

    if(title_sprite_sheet == NULL){
        title_sprite_sheet = create_sprite_sheet("../assets/art/PixEmuSprites.png", bad_colors, 3);
        mario1 = create_title_sprite(title_sprite_sheet, 0, 200, 0, 0, 16, 16, 3);
        link = create_title_sprite(title_sprite_sheet, 0, 250, 0, 8, 16, 16, 3);
        kirby = create_title_sprite(title_sprite_sheet, 0, 300, 0, 16, 16, 16, 3);
        kirby_parasol = create_title_sprite(title_sprite_sheet, 0, 300, 0, 16, 30, 32, 1);
        mario3 = create_title_sprite(title_sprite_sheet, 0, 350, 0, 16, 16, 32, 1);
        mario3_racoon = create_title_sprite(title_sprite_sheet, 0, 350, 0, 16, 24, 32, 2);
        shell = create_title_sprite(title_sprite_sheet, 0, 400, 16*4, 40, 16, 16, 1);

        //Mario 1
        add_animation(&mario1, 3, 6, 16, 0, 16, 0); //Walking
        add_animation(&mario1, 1, 0, 16*4, 0, 0, 0); //Jumping

        //Link
        add_animation(&link, 2, 16, 0, 8, 16, 0); //Walking Down
        add_animation(&link, 2, 16, 16*2, 8, 16, 0); //Walking Left
        add_animation(&link, 2, 16, 16*4, 8, 16, 0); //Walking Up

        //Kirby
        add_animation(&kirby, 4, 8, 0, 16, 16, 0); //Walking
        add_animation(&kirby, 5, 4, 16*4, 16, 16, 0); //Jumping
        add_animation(&kirby, 2, 48, 16*10, 16, 16, 0); //Sleeping

        //Kirby Parasol
        add_animation(&kirby_parasol, 4, 12, 0, 24, 28, 0);

        //Mario 3
        add_animation(&mario3, 2, 16, 0, 48, 16, 0); //Walking Left
        add_animation(&mario3_racoon, 3, 6, 16*3, 48, 24, 0); //Racoon Mario Running
        add_animation(&mario3_racoon, 3, 6, 16*3 + 24*3, 48, 24, 0); //Racoon Mario Flying

        //Shell
        add_animation(&shell, 4, 12, 16*3+1, 40, 16, 0); //Shell Spinning
    }

    //Rolls for if the sprite should be rendered.
    if(!kirby.rendered && !kirby_parasol.rendered && (f32)rand() / RAND_MAX <= 0.05 && global.time.frame_count == 0){
        kirby.rendered = true;
        kirby.sprite->x = -(i32)kirby.sprite->width;
        kirby.sprite->y = MAX(rand() % global.render.height - kirby.sprite->height, kirby.sprite->height);
    }

    if(!kirby_parasol.rendered && !kirby.rendered && (f32)rand() / RAND_MAX <= 0.05 && global.time.frame_count == 0){
        kirby_parasol.rendered = true;
        kirby_parasol.sprite->x = -(i32)kirby_parasol.sprite->width;
        kirby_parasol.sprite->y = MAX(rand() % global.render.height - kirby_parasol.sprite->height, kirby_parasol.sprite->height);
    }

    if(!mario1.rendered && (f32)rand() / RAND_MAX <= 0.05 && global.time.frame_count == 0){
        mario1.rendered = true;
        mario1.sprite->x = -(i32)mario1.sprite->width;
        mario1.sprite->y = MAX(rand() % global.render.height - mario1.sprite->height, mario1.sprite->height);
    }

    if(!link.rendered && (f32)rand() / RAND_MAX <= 0.05 && global.time.frame_count == 0){
        link.rendered = true;
        link.sprite->x = MAX(rand() % global.render.width - link.sprite->width, link.sprite->width);
        link.sprite->y = -(i32)link.sprite->height;
    }



    if(kirby.rendered){
        kirby.sprite->x += 200 * global.time.delta;
        DRAW_BOUNDED(kirby, 4)
    }

    if(kirby_parasol.rendered){
        kirby_parasol.sprite->x += 180 * global.time.delta;
        DRAW_BOUNDED(kirby_parasol, 4)
    }

    if(mario1.rendered){
        mario1.sprite->x += 210 * global.time.delta;
        DRAW_BOUNDED(mario1, 4)
    }

    if(link.rendered){
        link.sprite->y += 160 * global.time.delta;
        DRAW_BOUNDED(link, 4)
    }

}

void draw_title(){
    static i32 selected = 0;
    if(get_key_state_str("Escape") & KS_PRESSED){
        exit_program();
    }

    if(get_key_state_str("UP") & KS_PRESSED){
        selected--;
    }

    if(get_key_state_str("DOWN") & KS_PRESSED){
        selected++;
    }

    selected = abs(selected) % 2;

    //Modify sprite edges based on if they are selected or not.
    if(selected == 0){
        button_select(play_btn, selected, 0xFF000000);
        if(get_key_state_scancode(SDL_SCANCODE_RETURN) & KS_PRESSED){
            emulator_state.state = MAIN_MENU;
        }
    }
    else if (selected == 1){
        button_select(settings_btn, selected, 0xFF000000);
    }

    draw_sprite_to_bitmap(play_btn, BUTTON_SCALE);
    draw_sprite_to_bitmap(settings_btn, BUTTON_SCALE);

    draw_bitmap();
    render_text(default_font, 0, 0, LEFT, "Updates %u", global.bitmap.bitmap_updates);
    render_text(default_font, 0, 20, LEFT, "FPS: %u", global.time.frame_rate);
    render_text(title_font, global.render.width/2, global.render.height * 0.3, CENTER, "PixEmu")
}
