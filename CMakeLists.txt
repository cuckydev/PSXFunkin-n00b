cmake_minimum_required(VERSION 3.20)

set(TITLE_ID "000.01" CACHE STRING "Title ID (XXXX-nnnnn format)")

string(REGEX MATCH "^([A-Z][A-Z][A-Z][A-Z])[^0-9]*([0-9][0-9][0-9])([0-9][0-9])$" _dummy ${TITLE_ID})
set(EXE_NAME  ${CMAKE_MATCH_1}_${CMAKE_MATCH_2}.${CMAKE_MATCH_3})
set(SAVE_NAME ${CMAKE_MATCH_1}-${CMAKE_MATCH_2}${CMAKE_MATCH_3})

project(
	PSn00bSDK-PSXFunkin
	LANGUAGES    C CXX ASM
	VERSION      1.0.0
	DESCRIPTION  "PSXFunkin"
	HOMEPAGE_URL "https://ckdev.org"
)

# Main executable
psn00bsdk_add_executable(PSXFunkin DYNAMIC
	# Program
	"src/MainLoop.cpp"
	"src/MainLoop.h"
	"src/Types.h"

	# Backend
	"src/Backend/GPU.cpp"
	"src/Backend/GPU.h"
	"src/Backend/Timer.cpp"
	"src/Backend/Timer.h"
	"src/Backend/CD.cpp"
	"src/Backend/CD.h"
	"src/Backend/DLL.cpp"
	"src/Backend/DLL.h"
	"src/Backend/Data.cpp"
	"src/Backend/Data.h"
)
target_include_directories(PSXFunkin PRIVATE "src")

# Libraries
psn00bsdk_add_library(PSXFunkin_Menu SHARED
	# Program
	"src/Menu/Menu.cpp"
	"src/Menu/Menu.h"
)
target_include_directories(PSXFunkin_Menu PRIVATE "src")

psn00bsdk_add_library(PSXFunkin_Week1 SHARED
	# Program
	"src/Week1/Week1.cpp"
	"src/Week1/Week1.h"
)
target_include_directories(PSXFunkin_Week1 PRIVATE "src")

# Compiler definitions
target_compile_definitions(PSXFunkin PRIVATE "TITLE_ID=\"${TITLE_ID}\"")
target_compile_definitions(PSXFunkin PRIVATE "EXE_NAME=\"${EXE_NAME}\"")
target_compile_definitions(PSXFunkin PRIVATE "SAVE_NAME=\"${SAVE_NAME}\"")

# System config file
file(
    CONFIGURE
    OUTPUT  SYSTEM.CNF
    CONTENT [[
BOOT=cdrom:\\${TITLE_ID};1
TCB=4
EVENT=10
STACK=801FFFF0
]]
    NEWLINE_STYLE LF
)

# CD image
psn00bsdk_add_cd_image(
	iso # Target name
	funkin # Output file name
	funkin.xml  # Path to config file
	DEPENDS PSXFunkin PSXFunkin_Menu PSXFunkin_Week1
)
