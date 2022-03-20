cmake_minimum_required(VERSION 3.20)

set(TITLE_ID "FUNK-00001" CACHE STRING "Title ID (XXXX-nnnnn format)")

string(REGEX MATCH "^([A-Z][A-Z][A-Z][A-Z])[^0-9]*([0-9][0-9][0-9])([0-9][0-9])$" _dummy ${TITLE_ID})
set(EXE_NAME  ${CMAKE_MATCH_1}_${CMAKE_MATCH_2}.${CMAKE_MATCH_3})
set(SAVE_NAME ${CMAKE_MATCH_1}-${CMAKE_MATCH_2}${CMAKE_MATCH_3})

project(
	PSn00bSDK-PSXFunkin
	LANGUAGES    C CXX ASM
	VERSION      0.0.1
	DESCRIPTION  "PSXFunkin"
	HOMEPAGE_URL "https://ckdev.org"
)

# Common definition interface
add_library(common_defs INTERFACE)

target_compile_definitions(
    common_defs INTERFACE
    TITLE_ID="${TITLE_ID}"
    SAVE_NAME="${SAVE_NAME}"
    FUNK_VERSION="${PROJECT_VERSION}"
    FUNK_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    FUNK_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    FUNK_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)
target_include_directories(common_defs INTERFACE "src")

# Main executable
psn00bsdk_add_executable(PSXFunkin DYNAMIC
	# Program
	"src/Main.cpp"
	"src/Main.h"
	"src/Public.h"
	"src/Types.h"
	"src/Hash.h"
	"src/HashMap.h"

	# Stage
	"src/Stage/Stage.h"

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
target_link_libraries(PSXFunkin common_defs)

# Libraries
# Menu library
psn00bsdk_add_library(PSXFunkin_Menu SHARED
	"src/Menu/Menu.cpp"
	"src/Menu/Menu.h"
)
target_link_libraries(PSXFunkin_Menu common_defs)

# Week 1 library
psn00bsdk_add_library(PSXFunkin_Week1 SHARED
	"src/Week1/Week1.cpp"
	"src/Week1/Week1.h"
)
target_link_libraries(PSXFunkin_Week1 common_defs)

# Compile tools
include(ExternalProject)

set(
    tools_args
    -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}
    # add whatever args you need to pass to configure the tools
)
ExternalProject_Add(
    tools
    SOURCE_DIR       ${PROJECT_SOURCE_DIR}/tools
    BINARY_DIR       tools-build
    INSTALL_DIR      .
    CMAKE_CACHE_ARGS ${tools_args}
)

# Generate symbol map
add_custom_command(
    OUTPUT  publicmap.bin
    COMMAND ${PROJECT_BINARY_DIR}/generatemap "PSXFunkin.map" "${CMAKE_SOURCE_DIR}/src/Public.h" publicmap.bin
    DEPENDS PSXFunkin "${CMAKE_SOURCE_DIR}/src/Public.h" tools
    COMMENT "Generating public symbol map"
)

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
	DEPENDS PSXFunkin PSXFunkin_Menu PSXFunkin_Week1 publicmap.bin
)
