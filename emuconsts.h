#ifndef NESEMULATOR_EMUCONSTS_H
#define NESEMULATOR_EMUCONSTS_H

#define UNITIALIZED 0x0000
#define INVALID 0x0000
#define ZERO 0x00

#define MIN_ADDRESS 0x0000
#define MAX_ADDRESS 0xFFFF
#define LOW_BIT_MASK 0x00FF
#define HIGH_BIT_MASK 0xFF00
#define SIGN_MASK 0x80
#define RAM_SIZE 64 * 1024 //~64K
#define OPCODES 256

enum FLAGS6502 {
    C = (1 << 0), //Carry bit
    Z = (1 << 1), //Zero flag
    I = (1 << 2), //Disable Interrupts
    D = (1 << 3), //Decimal Mode (unused)
    B = (1 << 4), //Break
    U = (1 << 5), //Unused
    V = (1 << 7), //Overflow
    N = (1 << 8), //Negative
};

#endif //NESEMULATOR_EMUCONSTS_H
