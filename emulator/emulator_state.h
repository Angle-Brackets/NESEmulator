#ifndef NESEMULATOR_EMULATOR_STATE_H
#define NESEMULATOR_EMULATOR_STATE_H
#include "pixelinator.h"

struct Emulator_State {
    enum {
        TITLE_SCREEN,
        MAIN_MENU,
        NES
    } state;

    u32 frames_elapsed;
};

//The state of the emulator!
extern struct Emulator_State emulator_state;

#endif //NESEMULATOR_EMULATOR_STATE_H
