cmake_minimum_required(VERSION 2.8.5)
project(engine)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Lowercase project name for binaries and packaging
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

if(NOT AMIGA)
	message(SEND_ERROR "This project only compiles for Amiga")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAMIGA -Wall -Wextra -fomit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DACE_DEBUG")

file(GLOB_RECURSE SOURCES src/*[^atari].c)
file(GLOB_RECURSE HEADERS src/*[^atari].h)

list(REMOVE_ITEM SOURCES src/main_atari.c)
include_directories(
	${PROJECT_SOURCE_DIR}/src
)

if(GAME_DEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGAME_DEBUG")
endif()
if(ACE_DEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DACE_DEBUG") # For ACE headers with ifdefs
endif()

# ACE
add_subdirectory(deps/ace ace)
include_directories(deps/ace/include)

# Linux/other UNIX get a lower-case binary name
set(TARGET_NAME ${PROJECT_NAME_LOWER}.exe)

add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${TARGET_NAME} ace)

