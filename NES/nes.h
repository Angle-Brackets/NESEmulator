#ifndef NESEMULATOR_NES_H
#define NESEMULATOR_NES_H
#include "pixelinator.h"
#include "memory/bus.h"
#include "../emulator/emulator_state.h"

void initialize_nes(bool debug, enum Emulator_State* emulator_state);
void run_nes_emulation();

#endif //NESEMULATOR_NES_H
