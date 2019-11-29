# Install script for directory: /work/deps/ace

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/work/ace/libace.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace" TYPE FILE FILES
    "/work/deps/ace/include/ace/macros.h"
    "/work/deps/ace/include/ace/types.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/generic" TYPE FILE FILES
    "/work/deps/ace/include/ace/generic/main.h"
    "/work/deps/ace/include/ace/generic/screen.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/utils" TYPE FILE FILES
    "/work/deps/ace/include/ace/utils/bitmap.h"
    "/work/deps/ace/include/ace/utils/bmframe.h"
    "/work/deps/ace/include/ace/utils/chunky.h"
    "/work/deps/ace/include/ace/utils/custom.h"
    "/work/deps/ace/include/ace/utils/dir.h"
    "/work/deps/ace/include/ace/utils/endian.h"
    "/work/deps/ace/include/ace/utils/extview.h"
    "/work/deps/ace/include/ace/utils/file.h"
    "/work/deps/ace/include/ace/utils/font.h"
    "/work/deps/ace/include/ace/utils/palette.h"
    "/work/deps/ace/include/ace/utils/tag.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/managers" TYPE FILE FILES
    "/work/deps/ace/include/ace/managers/audio.h"
    "/work/deps/ace/include/ace/managers/blit.h"
    "/work/deps/ace/include/ace/managers/copper.h"
    "/work/deps/ace/include/ace/managers/game.h"
    "/work/deps/ace/include/ace/managers/joy.h"
    "/work/deps/ace/include/ace/managers/key.h"
    "/work/deps/ace/include/ace/managers/log.h"
    "/work/deps/ace/include/ace/managers/memory.h"
    "/work/deps/ace/include/ace/managers/mouse.h"
    "/work/deps/ace/include/ace/managers/rand.h"
    "/work/deps/ace/include/ace/managers/system.h"
    "/work/deps/ace/include/ace/managers/timer.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ace/managers/viewport" TYPE FILE FILES
    "/work/deps/ace/include/ace/managers/viewport/camera.h"
    "/work/deps/ace/include/ace/managers/viewport/scrollbuffer.h"
    "/work/deps/ace/include/ace/managers/viewport/simplebuffer.h"
    "/work/deps/ace/include/ace/managers/viewport/tilebuffer.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/fixmath" TYPE FILE FILES
    "/work/deps/ace/include/fixmath/fix16.h"
    "/work/deps/ace/include/fixmath/fix16_trig_sin_lut.h"
    "/work/deps/ace/include/fixmath/fixmath.h"
    "/work/deps/ace/include/fixmath/fract32.h"
    "/work/deps/ace/include/fixmath/int64.h"
    "/work/deps/ace/include/fixmath/uint32.h"
    )
endif()

