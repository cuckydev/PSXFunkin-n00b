cmake_minimum_required(VERSION 3.20)

project(
	PSn00bSDK-PSXFunkin
	LANGUAGES    C CXX ASM
	VERSION      1.0.0
	DESCRIPTION  "PSXFunkin"
	HOMEPAGE_URL "https://cuckydev.staropa.net/"
)

psn00bsdk_add_executable(PSXFunkin DYNAMIC
	# Program
	"src/Main.cpp"

	# Backend
	"src/Backend/CD.cpp"
	"src/Backend/CD.h"
	"src/Backend/DLL.cpp"
	"src/Backend/DLL.h"
)
target_include_directories(PSXFunkin PRIVATE "src")

psn00bsdk_add_library(PSXFunkin_Week1 SHARED
	# Program
	"src/Week1/Week1.cpp"
)
target_include_directories(PSXFunkin_Week1 PRIVATE "src")

psn00bsdk_add_cd_image(
	iso      # Target name
	funkin   # Output file name
	funkin.xml  # Path to config file
	DEPENDS PSXFunkin PSXFunkin_Week1
)
