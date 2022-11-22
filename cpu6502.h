#ifndef NESEMULATOR_CPU6502_H
#define NESEMULATOR_CPU6502_H

#include "bus.h"

//Forward Declaring this so we can avoid a circular logic error.
typedef struct Bus Bus;
/**
 * The design of the CPU
 */
typedef struct CPU6502 {
    struct INSTRUCTION {
        char name[10];
        u_int8_t (*operation)(struct CPU6502* cpu);
        u_int8_t (*addr_mode)(struct CPU6502* cpu);
    } INSTRUCTION;

    //General structure of an instruction, these are stored in lookup for easy access
    struct INSTRUCTION lookup[OPCODES];

    //General purpose registers
    u_int8_t a; //Accumulator
    u_int8_t x; //X Register
    u_int8_t y; //Y Register

    //Control Flow Registers
    u_int8_t stkp; //Stack Pointer
    u_int16_t pc; //Program Counter / Instruction Pointer
    u_int8_t status; //Condition Codes are set here

    Bus* bus;

    //Emulator ONLY variables, help make the emulation easier to do
    u_int8_t fetched; //Current value in ALU
    u_int16_t temp; //A nice variable for operations
    u_int16_t addr_abs; //All valid memory addresses pass through here
    u_int16_t addr_rel; //Holds absolute address after branch
    u_int8_t opcode; //Instruction Byte
    u_int8_t cycles; //Count number of cycles remaining in the instruction
    u_int32_t clock_count; //Global counter of the number of clocks.
} CPU6502;

/**
 * Resets the CPU into a known state
 * @param cpu 6502 CPU we're working with
 */
void reset(CPU6502* cpu);

/**
 * Interrupt Request to execute code a specific region
 * @param cpu 6502 CPU we're working with
 */
void irq(CPU6502* cpu);

/**
 * Non-Maskable Interrupt Request - Cannot be stopped.
 * @param cpu 6502 CPU we're working with
 */
void nmi(CPU6502* cpu);

/**
 * Perform a clock cycle worth of updates. This is NOT hardware accurate, as we just do everything in one shot
 * rather than emulating the hardware for this cycle directly.
 * @param cpu 6502 CPU we're working with
 */
void clock(CPU6502* cpu);

/**
 * Wrapper for bus.h write.
 * Write a byte to the given address
 * @param cpu 6502 CPU we're working with
 * @param addr Address to read from 0x000 - 0xFFFF (might change later)
 * @param data A byte to write to the address
 */
void write(CPU6502* cpu, u_int16_t addr, u_int8_t data);

/**
 * Wrapper for bus.h's read.
 * Read a byte from the given address
 * @param cpu 6502 CPU we're working with
 * @param addr Address to read from 0x0000 - 0xFFFF (might change later)
 * @return A byte at the given valid address.
 */
u_int8_t read(CPU6502* cpu, u_int16_t addr);

/**
 * This function figures out how to access the data depending on the given operation.
 * The information is passed as either an IMMEDIATE (IMM) or from an ADDRESS (ADDR).
 * @param cpu 6502 CPU we're working with
 * @return A byte of data
 */
u_int8_t fetch(CPU6502* cpu);

/**
 * Connects up the bus to the CPU
 * @param cpu 6502 CPU
 * @param bus Bus we're connecting to
 */
void connect_bus(CPU6502* cpu, Bus* bus);

/**
 * All of the addressing modes available on the 6502 Processor, there are quite a bit!
 * These functions also may return a number representing if more cycles are needed.
 */

u_int8_t IMP(CPU6502* cpu);
u_int8_t ZP0(CPU6502* cpu);
u_int8_t ZPY(CPU6502* cpu);
u_int8_t ABS(CPU6502* cpu);
u_int8_t ABY(CPU6502* cpu);
u_int8_t IZX(CPU6502* cpu);
u_int8_t IMM(CPU6502* cpu);
u_int8_t ZPX(CPU6502* cpu);
u_int8_t REL(CPU6502* cpu);
u_int8_t ABX(CPU6502* cpu);
u_int8_t IND(CPU6502* cpu);
u_int8_t IZY(CPU6502* cpu);

/**
 * The 56 legal opcodes available on the 6502 Processor, INVALID opcodes are not allowed, despite there
 * being known uses for some of them.
 *
 * These functions will return 0 if no extra cycles are needed, 1 if more are needed (may be refactored to use bool).
 */

u_int8_t BNE(CPU6502* cpu);
u_int8_t ADC(CPU6502* cpu);
u_int8_t CLV(CPU6502* cpu);
u_int8_t BCS(CPU6502* cpu);
u_int8_t DEC(CPU6502* cpu);
u_int8_t INC(CPU6502* cpu);
u_int8_t BVS(CPU6502* cpu);
u_int8_t JSR(CPU6502* cpu);
u_int8_t LSR(CPU6502* cpu);
u_int8_t PHP(CPU6502* cpu);
u_int8_t ROR(CPU6502* cpu);
u_int8_t SEC(CPU6502* cpu);
u_int8_t STX(CPU6502* cpu);
u_int8_t TSX(CPU6502* cpu);
u_int8_t BPL(CPU6502* cpu);
u_int8_t AND(CPU6502* cpu);
u_int8_t CMP(CPU6502* cpu);
u_int8_t BEQ(CPU6502* cpu);
u_int8_t DEX(CPU6502* cpu);
u_int8_t INX(CPU6502* cpu);
u_int8_t CLC(CPU6502* cpu);
u_int8_t LDA(CPU6502* cpu);
u_int8_t NOP(CPU6502* cpu);
u_int8_t PLA(CPU6502* cpu);
u_int8_t RTI(CPU6502* cpu);
u_int8_t SED(CPU6502* cpu);
u_int8_t STY(CPU6502* cpu);
u_int8_t TXA(CPU6502* cpu);
u_int8_t BRK(CPU6502* cpu);
u_int8_t ASL(CPU6502* cpu);
u_int8_t CPX(CPU6502* cpu);
u_int8_t BIT(CPU6502* cpu);
u_int8_t DEY(CPU6502* cpu);
u_int8_t INY(CPU6502* cpu);
u_int8_t CLD(CPU6502* cpu);
u_int8_t LDX(CPU6502* cpu);
u_int8_t ORA(CPU6502* cpu);
u_int8_t PLP(CPU6502* cpu);
u_int8_t RTS(CPU6502* cpu);
u_int8_t SEI(CPU6502* cpu);
u_int8_t TAX(CPU6502* cpu);
u_int8_t TXS(CPU6502* cpu);
u_int8_t BVC(CPU6502* cpu);
u_int8_t BCC(CPU6502* cpu);
u_int8_t CPY(CPU6502* cpu);
u_int8_t BMI(CPU6502* cpu);
u_int8_t EOR(CPU6502* cpu);
u_int8_t JMP(CPU6502* cpu);
u_int8_t CLI(CPU6502* cpu);
u_int8_t LDY(CPU6502* cpu);
u_int8_t PHA(CPU6502* cpu);
u_int8_t ROL(CPU6502* cpu);
u_int8_t SBC(CPU6502* cpu);
u_int8_t STA(CPU6502* cpu);
u_int8_t TAY(CPU6502* cpu);
u_int8_t TYA(CPU6502* cpu);

u_int8_t XXX(CPU6502* cpu); //Invalid opcode, basically a NOP


#endif //NESEMULATOR_CPU6502_H
