cmake_minimum_required(VERSION 2.8)
project("ikbd.tos")
# disable strange gcc assumptions
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# select Atari ST cross compiler
enable_language("ASM")

set(CMAKE_C_COMPILER m68k-atari-mint-gcc)
set(CMAKE_ASM_COMPILER m68k-atari-mint-as)
set(CMAKE_AR m68k-atari-mint-ar)
set(CMAKE_RANLIB m68k-atari-mint-ranlib)
set(CMAKE_C_FLAGS "-m68000 -O3 -fomit-frame-pointer -Wall -mshort -nostdlib -std=c99")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_C_FLAGS} ${PROJECT_SOURCE_DIR}/startup.S" )
set(CMAKE_ASM_FLAGS "-m68000 -I${PROJECT_SOURCE_DIR}")

link_directories(${CMAKE_SOURCE_DIR})

add_library(ikbd
 keys.c
 mouse.c
 ikbd.S
 ikbd.h
)

add_executable(ikbd.tos
 main.c
)

add_executable(ikbd_joy.tos
    main_joy.c
)

target_link_libraries(ikbd.tos ikbd)
target_link_libraries(ikbd_joy.tos ikbd)
