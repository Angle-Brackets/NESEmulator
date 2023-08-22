#include "cpu6502.h"
#include <string.h>

static const struct INSTRUCTION INSTRUCTIONS[] = {
        { "BRK", BRK, IMM, 7 },{ "ORA", ORA, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 3 },{ "ORA", ORA, ZP0, 3 },{ "ASL", ASL, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PHP", PHP, IMP, 3 },{ "ORA", ORA, IMM, 2 },{ "ASL", ASL, IMP, 2 },{ "???", XXX, IMP, 2 },{ "???", NOP, IMP, 4 },{ "ORA", ORA, ABS, 4 },{ "ASL", ASL, ABS, 6 },{ "???", XXX, IMP, 6 },
        { "BPL", BPL, REL, 2 },{ "ORA", ORA, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "ORA", ORA, ZPX, 4 },{ "ASL", ASL, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "CLC", CLC, IMP, 2 },{ "ORA", ORA, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "ORA", ORA, ABX, 4 },{ "ASL", ASL, ABX, 7 },{ "???", XXX, IMP, 7 },
        { "JSR", JSR, ABS, 6 },{ "AND", AND, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "BIT", BIT, ZP0, 3 },{ "AND", AND, ZP0, 3 },{ "ROL", ROL, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PLP", PLP, IMP, 4 },{ "AND", AND, IMM, 2 },{ "ROL", ROL, IMP, 2 },{ "???", XXX, IMP, 2 },{ "BIT", BIT, ABS, 4 },{ "AND", AND, ABS, 4 },{ "ROL", ROL, ABS, 6 },{ "???", XXX, IMP, 6 },
        { "BMI", BMI, REL, 2 },{ "AND", AND, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "AND", AND, ZPX, 4 },{ "ROL", ROL, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "SEC", SEC, IMP, 2 },{ "AND", AND, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "AND", AND, ABX, 4 },{ "ROL", ROL, ABX, 7 },{ "???", XXX, IMP, 7 },
        { "RTI", RTI, IMP, 6 },{ "EOR", EOR, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 3 },{ "EOR", EOR, ZP0, 3 },{ "LSR", LSR, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PHA", PHA, IMP, 3 },{ "EOR", EOR, IMM, 2 },{ "LSR", LSR, IMP, 2 },{ "???", XXX, IMP, 2 },{ "JMP", JMP, ABS, 3 },{ "EOR", EOR, ABS, 4 },{ "LSR", LSR, ABS, 6 },{ "???", XXX, IMP, 6 },
        { "BVC", BVC, REL, 2 },{ "EOR", EOR, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "EOR", EOR, ZPX, 4 },{ "LSR", LSR, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "CLI", CLI, IMP, 2 },{ "EOR", EOR, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "EOR", EOR, ABX, 4 },{ "LSR", LSR, ABX, 7 },{ "???", XXX, IMP, 7 },
        { "RTS", RTS, IMP, 6 },{ "ADC", ADC, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 3 },{ "ADC", ADC, ZP0, 3 },{ "ROR", ROR, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PLA", PLA, IMP, 4 },{ "ADC", ADC, IMM, 2 },{ "ROR", ROR, IMP, 2 },{ "???", XXX, IMP, 2 },{ "JMP", JMP, IND, 5 },{ "ADC", ADC, ABS, 4 },{ "ROR", ROR, ABS, 6 },{ "???", XXX, IMP, 6 },
        { "BVS", BVS, REL, 2 },{ "ADC", ADC, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "ADC", ADC, ZPX, 4 },{ "ROR", ROR, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "SEI", SEI, IMP, 2 },{ "ADC", ADC, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "ADC", ADC, ABX, 4 },{ "ROR", ROR, ABX, 7 },{ "???", XXX, IMP, 7 },
        { "???", NOP, IMP, 2 },{ "STA", STA, IZX, 6 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 6 },{ "STY", STY, ZP0, 3 },{ "STA", STA, ZP0, 3 },{ "STX", STX, ZP0, 3 },{ "???", XXX, IMP, 3 },{ "DEY", DEY, IMP, 2 },{ "???", NOP, IMP, 2 },{ "TXA", TXA, IMP, 2 },{ "???", XXX, IMP, 2 },{ "STY", STY, ABS, 4 },{ "STA", STA, ABS, 4 },{ "STX", STX, ABS, 4 },{ "???", XXX, IMP, 4 },
        { "BCC", BCC, REL, 2 },{ "STA", STA, IZY, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 6 },{ "STY", STY, ZPX, 4 },{ "STA", STA, ZPX, 4 },{ "STX", STX, ZPY, 4 },{ "???", XXX, IMP, 4 },{ "TYA", TYA, IMP, 2 },{ "STA", STA, ABY, 5 },{ "TXS", TXS, IMP, 2 },{ "???", XXX, IMP, 5 },{ "???", NOP, IMP, 5 },{ "STA", STA, ABX, 5 },{ "???", XXX, IMP, 5 },{ "???", XXX, IMP, 5 },
        { "LDY", LDY, IMM, 2 },{ "LDA", LDA, IZX, 6 },{ "LDX", LDX, IMM, 2 },{ "???", XXX, IMP, 6 },{ "LDY", LDY, ZP0, 3 },{ "LDA", LDA, ZP0, 3 },{ "LDX", LDX, ZP0, 3 },{ "???", XXX, IMP, 3 },{ "TAY", TAY, IMP, 2 },{ "LDA", LDA, IMM, 2 },{ "TAX", TAX, IMP, 2 },{ "???", XXX, IMP, 2 },{ "LDY", LDY, ABS, 4 },{ "LDA", LDA, ABS, 4 },{ "LDX", LDX, ABS, 4 },{ "???", XXX, IMP, 4 },
        { "BCS", BCS, REL, 2 },{ "LDA", LDA, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 5 },{ "LDY", LDY, ZPX, 4 },{ "LDA", LDA, ZPX, 4 },{ "LDX", LDX, ZPY, 4 },{ "???", XXX, IMP, 4 },{ "CLV", CLV, IMP, 2 },{ "LDA", LDA, ABY, 4 },{ "TSX", TSX, IMP, 2 },{ "???", XXX, IMP, 4 },{ "LDY", LDY, ABX, 4 },{ "LDA", LDA, ABX, 4 },{ "LDX", LDX, ABY, 4 },{ "???", XXX, IMP, 4 },
        { "CPY", CPY, IMM, 2 },{ "CMP", CMP, IZX, 6 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 8 },{ "CPY", CPY, ZP0, 3 },{ "CMP", CMP, ZP0, 3 },{ "DEC", DEC, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "INY", INY, IMP, 2 },{ "CMP", CMP, IMM, 2 },{ "DEX", DEX, IMP, 2 },{ "???", XXX, IMP, 2 },{ "CPY", CPY, ABS, 4 },{ "CMP", CMP, ABS, 4 },{ "DEC", DEC, ABS, 6 },{ "???", XXX, IMP, 6 },
        { "BNE", BNE, REL, 2 },{ "CMP", CMP, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "CMP", CMP, ZPX, 4 },{ "DEC", DEC, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "CLD", CLD, IMP, 2 },{ "CMP", CMP, ABY, 4 },{ "NOP", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "CMP", CMP, ABX, 4 },{ "DEC", DEC, ABX, 7 },{ "???", XXX, IMP, 7 },
        { "CPX", CPX, IMM, 2 },{ "SBC", SBC, IZX, 6 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 8 },{ "CPX", CPX, ZP0, 3 },{ "SBC", SBC, ZP0, 3 },{ "INC", INC, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "INX", INX, IMP, 2 },{ "SBC", SBC, IMM, 2 },{ "NOP", NOP, IMP, 2 },{ "???", SBC, IMP, 2 },{ "CPX", CPX, ABS, 4 },{ "SBC", SBC, ABS, 4 },{ "INC", INC, ABS, 6 },{ "???", XXX, IMP, 6 },
        { "BEQ", BEQ, REL, 2 },{ "SBC", SBC, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "SBC", SBC, ZPX, 4 },{ "INC", INC, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "SED", SED, IMP, 2 },{ "SBC", SBC, ABY, 4 },{ "NOP", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "SBC", SBC, ABX, 4 },{ "INC", INC, ABX, 7 },{ "???", XXX, IMP, 7 }
};

CPU6502* initialize_cpu(Bus* bus){
    CPU6502* cpu = malloc(sizeof(CPU6502));

    //Initialize CPU
    *cpu = (CPU6502){
            .a = UNITIALIZED,
            .x = UNITIALIZED,
            .y = UNITIALIZED,
            .stkp = UNITIALIZED,
            .pc = UNITIALIZED,
            .status = UNITIALIZED,
            .bus = bus,

            .fetched = UNITIALIZED,
            .temp = UNITIALIZED,
            .addr_abs = UNITIALIZED,
            .addr_rel = UNITIALIZED,
            .opcode = UNITIALIZED,
            .cycles = UNITIALIZED,
            .clock_count = UNITIALIZED
    };

    memcpy(cpu->lookup, INSTRUCTIONS, sizeof(INSTRUCTION) * OPCODES);

    return cpu;
}

u_int8_t get_flag(CPU6502* cpu, enum FLAGS6502 f) {
    return ((cpu->status & f) > 0) ? 1 : 0;
}

void set_flag(CPU6502* cpu, enum FLAGS6502 f, bool v) {
    if(v){
        cpu->status |= f;
    }
    else{
        cpu->status &= ~f;
    }
}

void write(CPU6502* cpu, u_int16_t addr, u_int8_t data){
    bus_cpu_write(cpu->bus, addr, data);
}

u_int8_t read(CPU6502* cpu, u_int16_t addr){
    return bus_cpu_read(cpu->bus, addr, false);
}

void cpu_reset(CPU6502* cpu){
    // Get the new address for PC
    cpu->addr_abs = 0xFFFC;
    u_int16_t low = read(cpu, cpu->addr_abs + 0);
    u_int16_t high = read(cpu, cpu->addr_abs + 1);

    //Set it!
    cpu->pc = (high << 8) | low;

    //Reset registers
    cpu->a = UNITIALIZED;
    cpu->x = UNITIALIZED;
    cpu->y = UNITIALIZED;
    cpu->stkp = 0xFD;
    cpu->status = 0x00 | U;

    //Clear helper variables
    cpu->addr_rel = UNITIALIZED;
    cpu->addr_abs = UNITIALIZED;
    cpu->fetched = 0x00;

    cpu->cycles = 8;
}

void irq(CPU6502* cpu){
    if(get_flag(cpu, I) == 0){
        //We are now activating an interrupt, so we need to push PC to the stack
        //it takes 2 pushes as the PC is a 16 bit value.
        // Push the program counter to the stack. It's 16-bits dont
        // forget so that takes two pushes
        write(cpu, 0x0100 + cpu->stkp, (cpu->pc >> 8) & 0x00FF);
        cpu->stkp--;
        write(cpu, 0x0100 + cpu->stkp, cpu->pc & 0x00FF);
        cpu->stkp--;

        // Then Push the status register to the stack
        set_flag(cpu, B, 0);
        set_flag(cpu, U, 1);
        write(cpu, 0x0100 + cpu->stkp, cpu->status);
        cpu->stkp--;

        // Read new program counter location from fixed address
        set_flag(cpu, I, 1);
        cpu->addr_abs = 0xFFFE;
        uint16_t lo = read(cpu, cpu->addr_abs + 0);
        uint16_t hi = read(cpu, cpu->addr_abs + 1);
        cpu->pc = (hi << 8) | lo;

        // IRQs take time
        cpu->cycles = 7;
    }
}

void nmi(CPU6502 *cpu) {
    //Similar affair to above, but this interrupt can't be ignored.
    write(cpu, 0x0100 + cpu->stkp, (cpu->pc >> 8) & 0x00FF);
    cpu->stkp--;
    write(cpu, 0x0100 + cpu->stkp, cpu->pc & 0x00FF);
    cpu->stkp--;

    //Write status to stack
    set_flag(cpu, B, 0);
    set_flag(cpu, U, 1);
    write(cpu, 0x0100 + cpu->stkp, cpu->status);
    cpu->stkp--;

    set_flag(cpu, I, 1);
    //Update PC
    cpu->addr_abs = 0xFFFA;
    u_int16_t low = read(cpu, cpu->addr_abs + 0);
    u_int16_t high = read(cpu, cpu->addr_abs + 1);
    cpu->pc = (high << 8) | low;

    cpu->cycles = 8;
}

void clock(CPU6502* cpu){
    //This is not a microcode accurate emulation of the hardware. All we're doing is effectively delaying the code
    //By some amount of clock cycles to emulate the microcode at a high level.

    if(cpu->cycles == 0){
        //Read the next instruction byte
        cpu->opcode = read(cpu, cpu->pc);

        //Always set this to 1 (Not sure why)
        set_flag(cpu, U, 1);

        //Increment PC
        cpu->pc++;

        //Get the number of cycles for the next instruction
        cpu->cycles = cpu->lookup[cpu->opcode].cycles;

        //Grab the immediate data using the requisite addressing mode
        u_int8_t cycle1 = (cpu->lookup[cpu->opcode].addr_mode)(cpu);

        //Perform Operation
        u_int8_t cycle2 = (cpu->lookup[cpu->opcode].operation)(cpu);

        //Number of cycles might have been altered by the instruction (page swapping for example)
        cpu->cycles += (cycle1 & cycle2);
        set_flag(cpu, U, 1);
    }

    //Increment global clock count and decrement cycles.
    cpu->clock_count++;
    cpu->cycles--;
}

u_int8_t fetch(CPU6502* cpu){
    if(cpu->lookup[cpu->opcode].addr_mode != IMP){
        cpu->fetched = read(cpu, cpu->addr_abs);
    }
    return cpu->fetched;
}

/**
 * Addressing Modes for the 6502
 * These handle how the CPU is able to access memory, which is indicated by the particular instruction used.
 */

u_int8_t IMP(CPU6502* cpu){
    cpu->fetched = cpu->a;
    return 0;
}

u_int8_t IMM(CPU6502* cpu){
    cpu->addr_abs = cpu->pc++;
    return 0;
}

u_int8_t ZP0(CPU6502* cpu){
    cpu->addr_abs = read(cpu, cpu->pc);
    cpu->pc++;
    cpu->addr_abs &= LOW_BIT_MASK;
    return 0;
}

u_int8_t ZPX(CPU6502* cpu){
    cpu->addr_abs = read(cpu, cpu->pc) + cpu->x;
    cpu->pc++;
    cpu->addr_abs &= LOW_BIT_MASK;
    return 0;
}

u_int8_t ZPY(CPU6502* cpu){
    cpu->addr_abs = read(cpu, cpu->pc) + cpu->y;
    cpu->pc++;
    cpu->addr_abs &= LOW_BIT_MASK;
    return 0;
}

u_int8_t REL(CPU6502* cpu){
    cpu->addr_rel = read(cpu, cpu->pc);
    cpu->pc++;

    if(cpu->addr_rel & SIGN_MASK){
        cpu->addr_rel |= HIGH_BIT_MASK;
    }
    return 0;
}

u_int8_t ABS(CPU6502* cpu){
    u_int16_t low = read(cpu, cpu->pc);
    cpu->pc++;
    u_int16_t high = read(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = (high << 8) | low;
    return 0;
}

u_int8_t ABX(CPU6502* cpu){
    u_int16_t low = read(cpu, cpu->pc);
    cpu->pc++;
    u_int16_t high = read(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = (high << 8) | low;
    cpu->addr_abs += cpu->x;

    if((cpu->addr_abs & HIGH_BIT_MASK) != (high << 8)){
        return 1;
    }
    return 0;
}

u_int8_t ABY(CPU6502* cpu) {
    u_int16_t low = read(cpu, cpu->pc);
    cpu->pc++;
    u_int16_t high = read(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = (high << 8) | low;
    cpu->addr_abs += cpu->y;

    if((cpu->addr_abs & HIGH_BIT_MASK) != (high << 8)){
        return 1;
    }

    return 0;
}

u_int8_t IND(CPU6502* cpu) {
    u_int16_t low_address = read(cpu, cpu->pc);
    cpu->pc++;
    u_int16_t high_address = read(cpu, cpu->pc);
    cpu->pc++;

    u_int16_t address = (high_address << 8) | low_address;

    //THIS IS ACTUALLY A BUG IN THE 6502 regarding page boundaries not being swapped correctly.
    if(low_address == LOW_BIT_MASK){
        cpu->addr_abs = (read(cpu, address & HIGH_BIT_MASK) << 8) | read(cpu, address + 0);
    }
    else {
        cpu->addr_abs = (read(cpu, address + 1) << 8) | read(cpu, address + 0);
    }

    return 0;
}

u_int8_t IZX(CPU6502* cpu){
    u_int16_t temp =  read(cpu, cpu->pc);
    cpu->pc++;

    u_int16_t low = read(cpu, (u_int16_t)(temp + (u_int16_t)cpu->x) & LOW_BIT_MASK);
    u_int16_t high = read(cpu, (u_int16_t)(temp + (u_int16_t)cpu->x + 1) & LOW_BIT_MASK);

    cpu->addr_abs = (high << 8) | low;

    return 0;
}

u_int8_t IZY(CPU6502* cpu) {
    u_int16_t temp = read(cpu, cpu->pc);
    cpu->pc++;

    u_int16_t low = read(cpu, temp & LOW_BIT_MASK);
    u_int16_t high = read(cpu, (temp + 1) & LOW_BIT_MASK);

    cpu->addr_abs = (high << 8) | low;
    cpu->addr_abs += cpu->y;

    if((cpu->addr_abs & HIGH_BIT_MASK) != (high << 8)){
        return 1;
    }
    return 0;
}

/**
 * Opcodes available in the 6502 Processor, certain invalid opcodes are not allowed.
 */

u_int8_t ADC(CPU6502* cpu){
    //Grab the fetched value stored in our registers
    fetch(cpu);

    //The add is performed with 16 bits to account for overflow.
    cpu->temp = (u_int16_t)cpu->a + (u_int16_t)cpu->fetched + (u_int16_t)get_flag(cpu, C);

    //Carry bit is set if the sum we generate is over 256
    set_flag(cpu, C, cpu->temp > 255);

    //The zero flag is set if the sum is zero
    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == 0);

    //The overflow flag is set based on some possible conditions.
    //If the sum is Positive + Positive = Negative or Negative + Negative = Positive
    //We can be sure that an overflow occurred, and that is what this expression captures.
    set_flag(cpu, V, (~((uint16_t)cpu->a ^ (uint16_t)cpu->fetched) & ((uint16_t)cpu->a ^ (uint16_t)cpu->temp)) & 0x0080);

    //We look at the left-most bit to decide if it is negative (1 = negative)
    set_flag(cpu, N, cpu->temp & SIGN_MASK);

    //Put the result in the accumulator and mask it down to 8 bits for size reasons.
    cpu->a = cpu->temp & LOW_BIT_MASK;

    return 1;
}

u_int8_t SBC(CPU6502* cpu){
    fetch(cpu);

    u_int16_t value = ((u_int16_t)cpu->fetched) ^ LOW_BIT_MASK;

    //Now everything is the same as addition due to math!
    cpu->temp = (u_int16_t)cpu->a + (u_int16_t)value + (u_int16_t)get_flag(cpu, C);

    set_flag(cpu, C, cpu->temp & HIGH_BIT_MASK);
    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == 0);
    set_flag(cpu, V, (cpu->temp ^ (uint16_t)cpu->a) & (cpu->temp ^ value) & 0x0080);
    set_flag(cpu, N, cpu->temp & 0x0080);
    cpu->a = cpu->temp & LOW_BIT_MASK;

    return 1;
}

u_int8_t AND(CPU6502* cpu){
    fetch(cpu);
    cpu->a = cpu->a & cpu->fetched;
    set_flag(cpu, Z, cpu->a == ZERO);
    set_flag(cpu, N, cpu->a & SIGN_MASK);
    return 1;
}

u_int8_t ASL(CPU6502* cpu){
    fetch(cpu);
    cpu->temp = (u_int16_t)cpu->fetched << 1;
    set_flag(cpu, C, (cpu->temp & HIGH_BIT_MASK) > 0);
    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == ZERO);
    set_flag(cpu, N, cpu->temp & SIGN_MASK);

    if(cpu->lookup[cpu->opcode].addr_mode == IMP){
        cpu->a = cpu->temp & LOW_BIT_MASK;
    }
    else{
        write(cpu, cpu->addr_abs, cpu->temp & LOW_BIT_MASK);
    }

    return 0;
}

u_int8_t BCC(CPU6502* cpu){
    if(get_flag(cpu, C) == 0){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if((cpu->addr_abs & HIGH_BIT_MASK) != (cpu->pc & HIGH_BIT_MASK)){
            cpu->cycles++;
        }
        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

u_int8_t BCS(CPU6502* cpu){
    if(get_flag(cpu, C) == 1){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if((cpu->addr_abs & HIGH_BIT_MASK) != (cpu->pc & HIGH_BIT_MASK)){
            cpu->cycles++;
        }
        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

u_int8_t BEQ(CPU6502* cpu){
    if(get_flag(cpu, Z) == 1){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if((cpu->addr_abs & HIGH_BIT_MASK) != (cpu->pc & HIGH_BIT_MASK)){
            cpu->cycles++;
        }
        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

u_int8_t BIT(CPU6502* cpu){
    fetch(cpu);
    cpu->temp = cpu->a & cpu->fetched;

    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == ZERO);
    set_flag(cpu, N, cpu->fetched & (1 << 7));
    set_flag(cpu, V, cpu->fetched & (1 << 6));
    return 0;
}

u_int8_t BMI(CPU6502* cpu){
    if(get_flag(cpu, N) == 1){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if((cpu->addr_abs & HIGH_BIT_MASK) != (cpu->pc & HIGH_BIT_MASK)){
            cpu->cycles++;
        }
        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

u_int8_t BNE(CPU6502* cpu){
    if(get_flag(cpu, Z) == 0){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if((cpu->addr_abs & HIGH_BIT_MASK) != (cpu->pc & HIGH_BIT_MASK)){
            cpu->cycles++;
        }
        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

u_int8_t BPL(CPU6502* cpu){
    if(get_flag(cpu, N) == 0){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if((cpu->addr_abs & HIGH_BIT_MASK) != (cpu->pc & HIGH_BIT_MASK)){
            cpu->cycles++;
        }
        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

u_int8_t BRK(CPU6502* cpu){
    cpu->pc++;

    write(cpu, 0x0100 + cpu->stkp, (cpu->pc >> 8) & LOW_BIT_MASK);
    cpu->stkp--;
    write(cpu, 0x0100 + cpu->stkp, cpu->pc & LOW_BIT_MASK);
    cpu->stkp--;

    set_flag(cpu, B, 1);
    write(cpu, 0x0100 + cpu->stkp, cpu->status);
    cpu->stkp--;
    set_flag(cpu, B, 0);
    set_flag(cpu, I, 1);

    cpu->pc = (u_int16_t)read(cpu, 0xFFFE) | ((u_int16_t)read(cpu, 0xFFFF) << 8);
    return 0;
}

u_int8_t BVC(CPU6502* cpu){
    if(get_flag(cpu, V) == 0){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if((cpu->addr_abs & HIGH_BIT_MASK) != (cpu->pc & HIGH_BIT_MASK)){
            cpu->cycles++;
        }
        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

u_int8_t BVS(CPU6502* cpu){
    if(get_flag(cpu, V) == 1){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if((cpu->addr_abs & HIGH_BIT_MASK) != (cpu->pc & HIGH_BIT_MASK)){
            cpu->cycles++;
        }
        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

u_int8_t CLC(CPU6502* cpu){
    set_flag(cpu, C, false);
    return 0;
}

u_int8_t CLD(CPU6502* cpu){
    set_flag(cpu, D, false);
    return 0;
}

u_int8_t CLI(CPU6502* cpu){
    set_flag(cpu, I, false);
    return 0;
}

u_int8_t CLV(CPU6502* cpu){
    set_flag(cpu, V, false);
    return 0;
}

u_int8_t CMP(CPU6502* cpu){
    fetch(cpu);

    cpu->temp = (u_int16_t)cpu->a - (u_int16_t)cpu->fetched;

    set_flag(cpu, C, cpu->a >= cpu->fetched);
    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == 0x0000);
    set_flag(cpu, N, cpu->temp & 0x0080);
    return 1;
}

u_int8_t CPX(CPU6502* cpu) {
    fetch(cpu);

    cpu->temp = (u_int16_t)cpu->x - (u_int16_t)cpu->fetched;

    set_flag(cpu, C, cpu->x >= cpu->fetched);
    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == 0x0000);
    set_flag(cpu, N, cpu->temp & 0x0080);
    return 0;
}

u_int8_t CPY(CPU6502* cpu) {
    fetch(cpu);

    cpu->temp = (u_int16_t)cpu->y - (u_int16_t)cpu->fetched;

    set_flag(cpu, C, cpu->y >= cpu->fetched);
    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == 0x0000);
    set_flag(cpu, N, cpu->temp & 0x0080);
    return 0;
}

u_int8_t DEC(CPU6502* cpu) {
    fetch(cpu);

    cpu->temp = cpu->fetched - 1;
    write(cpu, cpu->addr_abs, cpu->temp & LOW_BIT_MASK);

    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == 0x0000);
    set_flag(cpu, N, cpu->temp & 0x0080);
    return 0;
}

u_int8_t DEX(CPU6502* cpu){
    cpu->x--;

    set_flag(cpu, Z, cpu->x == ZERO);
    set_flag(cpu, N, cpu->x & SIGN_MASK);
    return 0;
}

u_int8_t DEY(CPU6502* cpu){
    cpu->y--;

    set_flag(cpu, Z, cpu->y == ZERO);
    set_flag(cpu, N, cpu->y & SIGN_MASK);
    return 0;
}

u_int8_t EOR(CPU6502* cpu){
    fetch(cpu);

    cpu->a = cpu->a ^ cpu->fetched;
    set_flag(cpu, Z, cpu->a == ZERO);
    set_flag(cpu, N, cpu->a & SIGN_MASK);
    return 1;
}

u_int8_t INC(CPU6502* cpu) {
    fetch(cpu);
    cpu->temp = cpu->fetched + 1;

    write(cpu, cpu->addr_abs, cpu->temp & LOW_BIT_MASK);
    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == 0x0000);
    set_flag(cpu, N, cpu->temp & 0x0080);
    return 0;
}

u_int8_t INX(CPU6502* cpu){
    cpu->x++;

    set_flag(cpu, Z, cpu->x == ZERO);
    set_flag(cpu, N, cpu->x & SIGN_MASK);
    return 0;
}

u_int8_t INY(CPU6502* cpu){
    cpu->y++;

    set_flag(cpu, Z, cpu->y == ZERO);
    set_flag(cpu, N, cpu->y & SIGN_MASK);
    return 0;
}

u_int8_t JMP(CPU6502* cpu){
    cpu->pc = cpu->addr_abs;
    return 0;
}

u_int8_t JSR(CPU6502* cpu) {
    cpu->pc--;

    write(cpu, 0x0100 + cpu->stkp, (cpu->pc >> 8) & LOW_BIT_MASK);
    cpu->stkp--;
    write(cpu, 0x0100 + cpu->stkp, cpu->pc & LOW_BIT_MASK);
    cpu->stkp--;

    cpu->pc = cpu->addr_abs;
    return 0;
}

u_int8_t LDA(CPU6502* cpu){
    fetch(cpu);
    cpu->a = cpu->fetched;

    set_flag(cpu, Z, cpu->a == ZERO);
    set_flag(cpu, N, cpu->a & SIGN_MASK);
    return 1;
}

u_int8_t LDX(CPU6502* cpu) {
    fetch(cpu);
    cpu->x = cpu->fetched;

    set_flag(cpu, Z, cpu->x == ZERO);
    set_flag(cpu, N, cpu->x & SIGN_MASK);
    return 1;
}

u_int8_t LDY(CPU6502* cpu) {
    fetch(cpu);
    cpu->y = cpu->fetched;

    set_flag(cpu, Z, cpu->y == ZERO);
    set_flag(cpu, N, cpu->y & SIGN_MASK);
    return 1;
}

u_int8_t LSR(CPU6502* cpu){
    fetch(cpu);
    set_flag(cpu, C, cpu->fetched & 0x0001);

    cpu->temp = cpu->fetched >> 1;
    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == 0x0000);
    set_flag(cpu, N, cpu->temp & 0x0080);

    if(cpu->lookup[cpu->opcode].addr_mode == IMP){
        cpu->a = cpu->temp & LOW_BIT_MASK;
    }
    else{
        write(cpu, cpu->addr_abs, cpu->temp & LOW_BIT_MASK);
    }
    return 0;
}

u_int8_t NOP(CPU6502* cpu) {
    //There are a LOT of possible NOPs, and it changes per ROM, so more will probably be added.
    switch(cpu->opcode){
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
    }
    return 0;
}

u_int8_t ORA(CPU6502* cpu){
    fetch(cpu);
    cpu->a = cpu->a | cpu->fetched;

    set_flag(cpu, Z, cpu->a == ZERO);
    set_flag(cpu, N, cpu->a & SIGN_MASK);
    return 1;
}

u_int8_t PHA(CPU6502* cpu){
    write(cpu, 0x0100 + cpu->stkp, cpu->a);
    cpu->stkp--;
    return 0;
}

u_int8_t PHP(CPU6502* cpu) {
    write(cpu, 0x0100 + cpu->stkp, cpu->status | B | U);

    set_flag(cpu, B, 0);
    set_flag(cpu, U, 0);
    cpu->stkp--;
    return 0;
}

u_int8_t PLA(CPU6502* cpu) {
    cpu->stkp++;
    cpu->a = read(cpu, 0x0100 + cpu->stkp);

    set_flag(cpu, Z, cpu->a == ZERO);
    set_flag(cpu, N, cpu->a & SIGN_MASK);
    return 0;
}

u_int8_t PLP(CPU6502* cpu) {
    cpu->stkp++;
    cpu->status = read(cpu, 0x0100 + cpu->stkp);

    set_flag(cpu, U, 1);
    return 0;
}

u_int8_t ROL(CPU6502* cpu) {
    fetch(cpu);
    cpu->temp = (u_int16_t)(cpu->fetched << 1) | get_flag(cpu, C);

    set_flag(cpu, C, cpu->temp & HIGH_BIT_MASK);
    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == 0x0000);
    set_flag(cpu, N, cpu->temp & 0x0080);

    if(cpu->lookup[cpu->opcode].addr_mode == IMP){
        cpu->a = cpu->temp & LOW_BIT_MASK;
    }
    else{
        write(cpu, cpu->addr_abs, cpu->temp & LOW_BIT_MASK);
    }
    return 0;
}

u_int8_t ROR(CPU6502* cpu) {
    fetch(cpu);
    cpu->temp = (u_int16_t)(get_flag(cpu, C) << 7) | (cpu->fetched >> 1);

    set_flag(cpu, C, cpu->fetched & 0x01);
    set_flag(cpu, Z, (cpu->temp & LOW_BIT_MASK) == ZERO);
    set_flag(cpu, N, cpu->temp & 0x0080);

    if(cpu->lookup[cpu->opcode].addr_mode == IMP){
        cpu->a = cpu->temp & LOW_BIT_MASK;
    }
    else{
        write(cpu, cpu->addr_abs, cpu->temp & LOW_BIT_MASK);
    }
    return 0;
}

u_int8_t RTI(CPU6502* cpu) {
    cpu->stkp++;

    cpu->status = read(cpu, 0x0100 + cpu->stkp);
    cpu->status &= ~B;
    cpu->status &= ~U;

    cpu->stkp++;
    cpu->pc = (u_int16_t)read(cpu, 0x0100 + cpu->stkp);
    cpu->stkp++;
    cpu->pc |= (u_int16_t)read(cpu, 0x0100 + cpu->stkp) << 8;

    return 0;
}

u_int8_t RTS(CPU6502* cpu) {
    cpu->stkp++;
    cpu->pc = (u_int16_t)read(cpu, 0x0100 + cpu->stkp);
    cpu->stkp++;
    cpu->pc |= (u_int16_t)read(cpu, 0x0100 + cpu->stkp) << 8;

    cpu->pc++;
    return 0;
}

u_int8_t SEC(CPU6502* cpu){
    set_flag(cpu, C, true);
    return 0;
}

u_int8_t SED(CPU6502* cpu){
    set_flag(cpu, D, true);
    return 0;
}

u_int8_t SEI(CPU6502* cpu){
    set_flag(cpu, I, true);
    return 0;
}

u_int8_t STA(CPU6502* cpu) {
    write(cpu, cpu->addr_abs, cpu->a);
    return 0;
}

u_int8_t STX(CPU6502* cpu) {
    write(cpu, cpu->addr_abs, cpu->x);
    return 0;
}

u_int8_t STY(CPU6502* cpu) {
    write(cpu, cpu->addr_abs, cpu->y);
    return 0;
}

u_int8_t TAX(CPU6502* cpu) {
    cpu->x = cpu->a;

    set_flag(cpu, Z, cpu->x == ZERO);
    set_flag(cpu, N, cpu->x & SIGN_MASK);
    return 0;
}

u_int8_t TAY(CPU6502* cpu) {
    cpu->y = cpu->a;

    set_flag(cpu, Z, cpu->y == ZERO);
    set_flag(cpu, N, cpu->y & SIGN_MASK);
    return 0;
}

u_int8_t TSX(CPU6502* cpu) {
    cpu->x = cpu->stkp;

    set_flag(cpu, Z, cpu->x == ZERO);
    set_flag(cpu, N, cpu->x & SIGN_MASK);

    return 0;
}

u_int8_t TXA(CPU6502* cpu) {
    cpu->a = cpu->x;

    set_flag(cpu, Z, cpu->a == ZERO);
    set_flag(cpu, N, cpu->a & SIGN_MASK);
    return 0;
}

u_int8_t TXS(CPU6502* cpu) {
    cpu->stkp = cpu->x;
    return 0;
}

u_int8_t TYA(CPU6502* cpu) {
    cpu->a = cpu->y;

    set_flag(cpu, Z, cpu->a == ZERO);
    set_flag(cpu, N, cpu->a & SIGN_MASK);
    return 0;
}

u_int8_t XXX(CPU6502* cpu){
    return 0;
}

static char* hex(uint32_t n, uint8_t d){
    char* res = (char*)malloc(sizeof(char) * (d+1));
    for(int32_t i = d - 1; i >= 0; i--, n >>= 4){
        res[i] = "0123456789ABCDEF"[n & 0xF];
    }
    res[d] = '\0';
    return res;
}

void disassemble(CPU6502* cpu, uint16_t start, uint16_t stop){
    //Again not memory safe!
    uint32_t addr = start;
    uint8_t value = 0x00;
    uint8_t low = 0x00;
    uint8_t high = 0x00;
    uint16_t line_addr = 0;

    while(addr <= (uint32_t)stop){
        line_addr = addr;
        printf("$%s:", hex(addr, 4));
        uint8_t opcode = bus_cpu_read(cpu->bus, addr, true);
        addr++;
        printf("%s ", cpu->lookup[opcode].name);

        if(cpu->lookup[opcode].addr_mode == IMP){
            printf(" {IMP}");
        }
        else if(cpu->lookup[opcode].addr_mode == IMM){
            value = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            printf("#$%s {IMM}", hex(value, 2));
        }
        else if(cpu->lookup[opcode].addr_mode == ZP0){
            low = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            high = 0x00;
            printf("$%s {ZP0}", hex(low, 2));
        }
        else if(cpu->lookup[opcode].addr_mode == ZPX){
            low = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            high = 0x00;
            printf("$%s, X {ZPX}", hex(low, 2));
        }
        else if(cpu->lookup[opcode].addr_mode == ZPY){
            low = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            high = 0x00;
            printf("$%s, Y {ZPY}", hex(low, 2));
        }
        else if(cpu->lookup[opcode].addr_mode == IZX){
            low = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            high = 0x00;
            printf("$%s, X {IZX}", hex(low, 2));
        }
        else if(cpu->lookup[opcode].addr_mode == IZY){
            low = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            high = 0x00;
            printf("($%s), Y {IZY}", hex(low, 2));
        }
        else if(cpu->lookup[opcode].addr_mode == ABS){
            low = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            high = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            printf("$%s {ABS}", hex((uint16_t)(high << 8) | low, 4));
        }
        else if(cpu->lookup[opcode].addr_mode == ABX){
            low = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            high = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            printf("$%s, X {ABX}", hex((uint16_t)(high << 8) | low, 4));
        }
        else if(cpu->lookup[opcode].addr_mode == ABY){
            low = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            high = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            printf("$%s, Y {ABY}", hex((uint16_t)(high << 8) | low, 4));
        }
        else if(cpu->lookup[opcode].addr_mode == IND){
            low = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            high = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            printf("($%s) {IND}", hex((uint16_t)(high << 8) | low, 4));
        }
        else if(cpu->lookup[opcode].addr_mode == REL){
            value = bus_cpu_read(cpu->bus, addr, true);
            addr++;
            printf("$%s [$%s] {REL}", hex(value, 2), hex(addr + (int8_t)value, 4));
        }
        printf("\n");
    }
}