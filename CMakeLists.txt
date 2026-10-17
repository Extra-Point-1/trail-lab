cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(TrailLab VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/TrailLab.cpp
    src/TrailSettings.cpp
    src/TrailPresets.cpp
)

# Include directories
include_directories(include)

add_library(TrailLab SHARED ${SOURCES})

target_include_directories(TrailLab PRIVATE include)

# Geode mod setup - this is provided by the Geode build system
setup_geode_mod(TrailLab)

# Android-specific settings
if(ANDROID)
    set_target_properties(TrailLab PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
endif()
