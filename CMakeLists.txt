cmake_minimum_required(VERSION 3.20)
project(
    dmath
    VERSION 1.0.0
    LANGUAGES CXX
)

# Force modern C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect all implementation files from src/ and its subdirectories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Define the static library target
add_library(dmath STATIC ${SOURCES} "src/vector/vector3.cpp")

# Configure include directories
# PUBLIC: Headers inside include/ are usable by dmath AND any consuming target
# BUILD_INTERFACE: Ensures correct include paths during in-tree builds
target_include_directories(dmath
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Enable common compiler warnings
if(MSVC)
    target_compile_options(dmath PRIVATE /W4)
else()
    target_compile_options(dmath PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ==============================================================================
# Testing Configuration (optional)
# ==============================================================================
option(BUILD_TESTS "Build unit tests for dmath" ON)

if(BUILD_TESTS)
    enable_testing()
    
    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    
    if(TEST_SOURCES)
        add_executable(dmath_tests ${TEST_SOURCES})
        target_link_libraries(dmath_tests PRIVATE dmath)
        add_test(NAME run_dmath_tests COMMAND dmath_tests)
    endif()
endif()
