#ifndef NESEMULATOR_TITLE_H
#define NESEMULATOR_TITLE_H
#include "pixelinator.h"
#include "emulator_state.h"
#include "title/animation.h"

#define BUTTON_SCALE 5

#define MAX(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define MIN(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

static PIX_Font* default_font = NULL;

void initialize_title();
void draw_title_background();
void draw_title_sprites();
void draw_title();

#endif //NESEMULATOR_TITLE_H
