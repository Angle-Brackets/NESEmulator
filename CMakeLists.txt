cmake_minimum_required(VERSION 3.21)
project(NESEmulator VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include necessary directories
include_directories(include/)
include_directories(emulator/)

# Compiler flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building Release...")
    set(CMAKE_C_FLAGS_RELEASE "-Ofast")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building Debug...")
    set(CMAKE_C_FLAGS "-Wall -Wextra")
    set(CMAKE_C_FLAGS_DEBUG "-g")
endif()

# Define the executable and its source files
add_executable(NESEmulator
    emulator/main.c 
    NES/memory/bus.c NES/memory/bus.h 
    NES/cpu/cpu6502.c NES/cpu/cpu6502.h 
    NES/emuconsts.h 
    NES/ppu/ppu2C02.h NES/ppu/ppu2C02.c 
    NES/memory/cartridge.c NES/memory/cartridge.h 
    NES/mappers/mapper.c NES/mappers/mapper.h 
    NES/mappers/custom_mappers/mapper_000.c NES/mappers/custom_mappers/mapper_000.h 
    NES/mappers/custom_mappers/mapper_002.c NES/mappers/custom_mappers/mapper_002.h 
    NES/mappers/custom_mappers/mapper_003.c NES/mappers/custom_mappers/mapper_003.h 
    NES/mappers/custom_mappers/mapper_001.c NES/mappers/custom_mappers/mapper_001.h 
    NES/mappers/custom_mappers/mapper_004.c NES/mappers/custom_mappers/mapper_004.h 
    NES/nes.c 
    emulator/files/emu_file.c
    emulator/title/animation.c emulator/title/animation.h
    emulator/title/title.c emulator/title/title.h
    emulator/emulator_state.c emulator/emulator_state.h
)

# Setup external dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)

# Find and link Pixelinator static library
find_library(PIXELINATOR_LIB Pixelinator PATHS ${CMAKE_SOURCE_DIR}/libs)

if(NOT PIXELINATOR_LIB)
    message(FATAL_ERROR "libPixelinator.a not found in ${CMAKE_SOURCE_DIR}/libs")
endif()

# Include Pixelinator headers
target_include_directories(NESEmulator PUBLIC ${CMAKE_SOURCE_DIR}/libs/include)

# Ensure SDL headers are included (if needed)
if(DEFINED SDL2_SOURCE_DIR)
    target_include_directories(NESEmulator PUBLIC ${SDL2_SOURCE_DIR}/include)
endif()

# Threading support
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(NESEmulator PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(NESEmulator "${CMAKE_THREAD_LIBS_INIT}")
endif()

# Link necessary libraries
target_link_libraries(NESEmulator PUBLIC
    ${PIXELINATOR_LIB}
    SDL2::SDL2-static
    SDL2_image::SDL2_image-static
    SDL2_ttf::SDL2_ttf-static
    SDL2_mixer::SDL2_mixer-static
    m
)