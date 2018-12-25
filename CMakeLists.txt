cmake_minimum_required(VERSION 3.5)
project(Functional VERSION 0.1.0)

# config options
option(FN_ENABLE_TESTS "Enable unit tests" ON)

# compiler warnings
if (MSVC)
    add_compile_options("/W4")
else()
    add_compile_options("-Wpedantic")
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
endif()

# add functional library
include(CheckCXXCompilerFlag)

add_library(Functional INTERFACE)
add_library(Functional::Functional ALIAS Functional)

target_compile_features(Functional INTERFACE cxx_std_17)

target_include_directories(Functional INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# add tests
if (FN_ENABLE_TESTS)
    add_subdirectory(test)
endif()
