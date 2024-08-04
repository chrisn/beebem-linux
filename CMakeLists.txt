#-------------------------------------------------------------------------------
#
# BeebEm - BBC Micro and Master 128 Emulator
# Copyright (C) 2024 Chris Needham
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program; if not, write to the Free
# Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA  02110-1301, USA.
#
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#
# CMake project setup
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8.7)
project(beebem)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: default is Release")
endif()

message(STATUS "CMAKE_VERSION=${CMAKE_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#-------------------------------------------------------------------------------
#
# Dependencies
#
#-------------------------------------------------------------------------------

if(BUILD_STATIC)
    message(STATUS "Static build")

    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
endif(BUILD_STATIC)

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    message(STATUS "ZLIB_INCLUDE_DIRS='${ZLIB_INCLUDE_DIRS}'")
    message(STATUS "ZLIB_LIBRARIES=${ZLIB_LIBRARIES}")

    include_directories(${ZLIB_INCLUDE_DIRS})
    add_definitions(-DHAVE_ZLIB_H)
    add_definitions(-DHAVE_LIBZ)
endif(ZLIB_FOUND)

find_package(SDL REQUIRED)
if(SDL_FOUND)
    message(STATUS "SDL_INCLUDE_DIRS='${SDL_INCLUDE_DIRS}'")
    message(STATUS "SDL_LIBRARIES=${SDL_LIBRARIES}")

    include_directories(${SDL_INCLUDE_DIRS})
endif(SDL_FOUND)

find_package(GTK2 2.6 REQUIRED gtk)
if(GTK2_FOUND)
    message(STATUS "GTK2_INCLUDE_DIRS='${GTK2_INCLUDE_DIRS}'")
    message(STATUS "GTK2_LIBRARIES=${GTK2_LIBRARIES}")

    include_directories(${GTK2_INCLUDE_DIRS})
endif()

#-------------------------------------------------------------------------------
#
# Compiler flags
#
#-------------------------------------------------------------------------------

if(CMAKE_VERSION VERSION_LESS "2.8.10")
    exec_program(
        ${CMAKE_CXX_COMPILER}
        ARGS --version
        OUTPUT_VARIABLE COMPILER_VERSION_STRING
    )
    string(REGEX REPLACE ".*([0-9]\\.[0-9]\\.[0-9]).*" "\\1" CMAKE_CXX_COMPILER_VERSION ${COMPILER_VERSION_STRING})
endif()

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.6.3")
    message(FATAL_ERROR "g++ 4.6.3 or later required")
endif()

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.7")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

set(COMMON_FLAGS "-Wall -Wextra -pedantic") # -Wconversion
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS ${COMMON_FLAGS})

message(STATUS "CMAKE_CXX_COMPILER_VERSION='${CMAKE_CXX_COMPILER_VERSION}'")
message(STATUS "CMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}'")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG='${CMAKE_CXX_FLAGS_DEBUG}'")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE='${CMAKE_CXX_FLAGS_RELEASE}'")
message(STATUS "CMAKE_CXX_COMPILE_OBJECT='${CMAKE_CXX_COMPILE_OBJECT}'")

#-------------------------------------------------------------------------------
#
# Source files
#
#-------------------------------------------------------------------------------

include_directories(Src)
include_directories(Src/gui)
include_directories(Src/posix)

add_definitions(-DDATA_DIR="/usr/local/share/beebem")
add_definitions(-DVERSION="4.19")
add_definitions(-DWITH_UNIX_EXTRAS) # Enable UNIX extras
add_definitions(-DPACKAGE="beebem")
# add_definitions(-DWITH_DEBUG_OUTPUT) # Additional debugging
# add_definitions(-DEG_DEBUG) # GUI debugging code
add_definitions(-DWITH_ECONET) # Econet support
# add_definitions(-DWITHOUT_LEVEL_INDICATOR) # Disable level indicator
add_definitions(-DWITHOUT_SHENANIGANS) # Disable all __LINE__ shenanigans
# add_definitions(-DWITHOUT_REALTIME_SLIDER) # Disable realtime slider on slide bar
add_definitions(-DWITHOUT_WELCOME_MESSAGE) # Disable annoying welcome message
add_definitions(-DWITH_FS_FPS) # Enable fullscreen FPS indicator
# add_definitions(-DWITH_SYSTEM_CP) # Compile with call to system 'cp' command when copying files
# add_definitions(-DWITH_FORCED_CM) # Compile with a forced colormap for 8 bit displays
add_definitions(-DMODET) # Enable ARM Thumb instructions
add_definitions(-DMODE32) # Enable 32-bit ARM instructions
add_definitions(-DBEEBEM)

set(SRCS
    Src/6502core.cpp
    Src/Arm.cpp
    Src/ArmDisassembler.cpp
    Src/AtoDConv.cpp
    Src/AviWriter.cpp
    Src/Bcd.cpp
    Src/BeebEmPages.cpp
    Src/BeebEmPagesDisks.cpp
    Src/BeebMem.cpp
    Src/BeebWin.cpp
    Src/BeebWinDx.cpp
    Src/BeebWinIo.cpp
    Src/BeebWinPrefs.cpp
    Src/BeebWinSpeech.cpp
    Src/Csw.cpp
    Src/Debug.cpp
    Src/DebugTrace.cpp
    Src/Dialog.cpp
    Src/Disc1770.cpp
    Src/Disc8271.cpp
    Src/DiscEdit.cpp
    Src/DiscInfo.cpp
    Src/Econet.cpp
    Src/Ext1770.cpp
    Src/FileDialog.cpp
    Src/FileType.cpp
    Src/FileUtils.cpp
    Src/Hardware.cpp
    Src/IC32Latch.cpp
    Src/Ide.cpp
    Src/IP232.cpp
    Src/KeyMap.cpp
    Src/Log.cpp
    Src/Main.cpp
    Src/Master512CoPro.cpp
    Src/Model.cpp
    Src/Music5000.cpp
    Src/PALRom.cpp
    Src/Preferences.cpp
    Src/RingBuffer.cpp
    Src/RomConfigFile.cpp
    Src/Rtc.cpp
    Src/Sasi.cpp
    Src/Scsi.cpp
    Src/Sdl.cpp
    Src/Serial.cpp
    Src/SerialDevices.cpp
    Src/SerialPort.cpp
    Src/Socket.cpp
    Src/Sound.cpp
    Src/SoundStreamer.cpp
    Src/Speech.cpp
    Src/SprowCoPro.cpp
    Src/StringUtils.cpp
    Src/SysVia.cpp
    Src/TapeControlDialog.cpp
    Src/TapeMap.cpp
    Src/Teletext.cpp
    Src/Thread.cpp
    Src/Tube.cpp
    Src/Uef.cpp
    Src/UefState.cpp
    Src/UserKeyboardDialog.cpp
    Src/UserPortBreakoutBox.cpp
    Src/UserPortRTC.cpp
    Src/UserVia.cpp
    Src/Via.cpp
    Src/Video.cpp
    Src/Z80Support.cpp
    Src/Z80.cpp
    Src/Z80dis.cpp
    Src/ARMulator/armdis.cpp
    Src/ARMulator/armemu.cpp
    Src/ARMulator/arminit.cpp
    Src/ARMulator/armsupp.cpp
    Src/ARMulator/thumbemu.cpp
    Src/gui/box.c
    Src/gui/button.c
    Src/gui/functions.c
    Src/gui/label.c
    Src/gui/log.c
    Src/gui/messagebox.c
    Src/gui/progressbar.c
    Src/gui/radiobutton.c
    Src/gui/radiogroup.c
    Src/gui/sdl.c
    Src/gui/slidebar.c
    Src/gui/tabgroup.c
    Src/gui/tabpage.c
    Src/gui/tickbox.c
    Src/gui/togglebutton.c
    Src/gui/widget_shared.c
    Src/gui/widget.c
    Src/gui/window.c
    Src/posix/Windows.cpp
)

add_executable(beebem ${SRCS})

#-------------------------------------------------------------------------------
#
# Linker
#
#-------------------------------------------------------------------------------

set(LIBS
    ${ZLIB_LIBRARIES}
    ${SDL_LIBRARIES}
    ${GTK2_LIBRARIES}
)

target_link_libraries(beebem ${LIBS})

#-------------------------------------------------------------------------------
#
# Installation
#

message(STATUS "CMAKE_INSTALL_PREFIX='${CMAKE_INSTALL_PREFIX}'")

# Install executable
install(TARGETS beebem DESTINATION bin)

# Install docs
set(DOCS
    Documents/CHANGES.txt
    Documents/Econet.zip
    Documents/Ibos.txt
    Documents/Integra-B.zip
    Documents/README.txt
    Documents/COPYRIGHT.txt
    Documents/README_Z80.txt
)

install(FILES ${DOCS} DESTINATION share/doc/beebem)

# Install data files
set(RESOURCES
    DriveMotor.snd
    HeadLoad.snd
    HeadSeek.snd
    HeadStep.snd
    HeadUnload.snd
    RelayOff.snd
    RelayOn.snd
    Teletext.fnt
)

install(DIRECTORY UserData DESTINATION share/beebem)
install(FILES ${RESOURCES} DESTINATION share/beebem)

#-------------------------------------------------------------------------------
