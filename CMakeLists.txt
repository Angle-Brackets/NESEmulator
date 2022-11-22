cmake_minimum_required(VERSION 3.21)
project(NESEmulator C)

set(CMAKE_C_STANDARD 99)

add_executable(NESEmulator main.c bus.c bus.h cpu6502.c cpu6502.h emuconsts.h)
