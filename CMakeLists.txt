cmake_minimum_required(VERSION 3.15)

project(ath_dsp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ATH_DSP_SOURCES
    "./*.cpp"
    "./*.h"
)

add_library(ath_dsp STATIC ${ATH_DSP_SOURCES})

target_include_directories(ath_dsp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)