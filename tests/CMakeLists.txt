cmake_minimum_required(VERSION 3.16)
project(ath_dsp_tests LANGUAGES CXX)

add_compile_options(-mavx2 -mfma -O3)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master)
FetchContent_GetProperties(matplotplusplus)
if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_subdirectory(.. ath_dsp_build)

add_executable(ath_dsp_tests
    main.cpp
)

add_custom_command(
    TARGET ath_dsp_tests
    POST_BUILD
    COMMAND ath_dsp_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running ath_dsp_tests after build"
)

target_link_libraries(ath_dsp_tests PUBLIC matplot)

target_link_libraries(ath_dsp_tests
    PRIVATE
        ath_dsp
)

target_include_directories(ath_dsp_tests
    PRIVATE
        ${matplotplusplus_SOURCE_DIR}/source
)

target_compile_features(ath_dsp_tests PRIVATE cxx_std_23)