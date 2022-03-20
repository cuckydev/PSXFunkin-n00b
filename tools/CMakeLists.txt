cmake_minimum_required(VERSION 3.8)

# generatemap
project(generatemap LANGUAGES CXX)

add_executable(generatemap "generatemap/generatemap.cpp")
set_target_properties(generatemap PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

install(TARGETS generatemap DESTINATION .)
