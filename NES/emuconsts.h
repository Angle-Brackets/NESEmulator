#ifndef NESEMULATOR_EMUCONSTS_H
#define NESEMULATOR_EMUCONSTS_H

#define UNITIALIZED 0x0000
#define ZERO 0x00

#define WIDTH 750
#define HEIGHT 750
#define NES_WIDTH 256
#define NES_HEIGHT 240

#define MIN_ADDRESS_RAM 0x0000
#define MAX_ADDRESS_RAM 0x1FFF
#define MIRROR_MASK_RAM 0x07FF

#define MIN_ADDRESS_PPU 0x2000
#define MAX_ADDRESS_PPU 0x3FFF
#define MIRROR_MASK_PPU 0x0007
#define MAX_PATTERN_MEMORY 0x1FFF
#define MIN_NAMETABLE_MEMORY 0x2000
#define MAX_NAMETABLE_MEMORY 0x3EFF
#define MIN_PALETTE_MEMORY 0x3F00
#define MAX_PALETTE_MEMORY 0x3FFF
#define DMA_ADDRESS 0x4014
#define MIN_ADDRESS_CONTROLLER 0x4016
#define MAX_ADDRESS_CONTROLLER 0x4017

#define LOW_BIT_MASK 0x00FF
#define HIGH_BIT_MASK 0xFF00
#define SIGN_MASK 0x80
#define RAM_SIZE 2048 //2K
#define OPCODES 256

enum FLAGS6502 {
    C = (1 << 0), //Carry bit
    Z = (1 << 1), //Zero flag
    I = (1 << 2), //Disable Interrupts
    D = (1 << 3), //Decimal Mode (unused)
    B = (1 << 4), //Break
    U = (1 << 5), //Unused
    V = (1 << 6), //Overflow
    N = (1 << 7), //Negative
};

#endif //NESEMULATOR_EMUCONSTS_H
