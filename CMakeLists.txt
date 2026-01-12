cmake_minimum_required(VERSION 3.15)

project(ath_dsp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE ALL_SOURCES
    "./*.cpp"
    "./*.h"
)

set(LIB_SOURCES "")
foreach(src ${ALL_SOURCES})
    # Exclude tests and _deps using regex
    if(NOT src MATCHES "/tests/" AND NOT src MATCHES "/_deps/")
        list(APPEND LIB_SOURCES ${src})
    endif()
endforeach()

add_library(ath_dsp STATIC ${LIB_SOURCES})

target_include_directories(ath_dsp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)