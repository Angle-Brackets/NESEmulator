cmake_minimum_required(VERSION 3.21)
project(NESEmulator VERSION 1.0)

set(CMAKE_C_STANDARD 99)

include_directories(include/)
include_directories("${CMAKE_SOURCE_DIR}/libs/libPixelinator.a")
find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(NESEmulator PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(NESEmulator "${CMAKE_THREAD_LIBS_INIT}")
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-OFast")

add_executable(NESEmulator main.c NES/memory/bus.c NES/memory/bus.h NES/cpu/cpu6502.c NES/cpu/cpu6502.h NES/emuconsts.h NES/ppu/ppu2C02.h NES/ppu/ppu2C02.c NES/memory/cartridge.c NES/memory/cartridge.h NES/mappers/mapper.c NES/mappers/mapper.h NES/mappers/custom_mappers/mapper_000.c NES/mappers/custom_mappers/mapper_000.h)
target_link_libraries(NESEmulator PRIVATE ${CMAKE_SOURCE_DIR}/libs/libPixelinator.a SDL2::SDL2 SDL2_mixer::SDL2_mixer SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)