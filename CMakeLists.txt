cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(TrailLab VERSION 1.0.0 LANGUAGES CXX C)

# Geode SDK path - relative to workspace root
set(GEODE_SDK_DIR "${CMAKE_SOURCE_DIR}/../build-tools/geode-sdk")

# Include Geode's build system functions
include("${GEODE_SDK_DIR}/cmake/GeodeFile.cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/TrailLab.cpp
    src/TrailSettings.cpp
    src/TrailPresets.cpp
)

# Include directories
include_directories("${GEODE_SDK_DIR}/loader/include")
include_directories("${CMAKE_SOURCE_DIR}/include")

add_library(TrailLab SHARED ${SOURCES})

target_include_directories(TrailLab PRIVATE include)

# Link against Geode SDK
target_link_libraries(TrailLab geode-sdk)

setup_geode_mod(TrailLab)

# Android-specific settings
if(ANDROID)
    set_target_properties(TrailLab PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
endif()
