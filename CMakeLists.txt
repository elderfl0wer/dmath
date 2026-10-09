cmake_minimum_required(VERSION 3.20)

project(
    dmath
    VERSION 1.0.0
    DESCRIPTION "Modern C++ mathematics library"
    LANGUAGES CXX
)

# ==============================================================================
# Library
# ==============================================================================

# Collect all source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(dmath STATIC ${SOURCES})

# Require C++23
target_compile_features(dmath PUBLIC cxx_std_23)

# Public include directory
target_include_directories(dmath
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Compiler warnings
if(MSVC)
    target_compile_options(dmath PRIVATE /W4)
else()
    target_compile_options(dmath PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -02
        -flto
    )
endif()

# ==============================================================================
# Tests
# ==============================================================================

option(BUILD_TESTS "Build dmath tests" ON)

if(BUILD_TESTS)

    enable_testing()

    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    )

    if(TEST_SOURCES)

        add_executable(dmath_tests
            ${TEST_SOURCES}
        )

        target_link_libraries(dmath_tests
            PRIVATE
                dmath
        )

        add_test(
            NAME run_dmath_tests
            COMMAND dmath_tests
        )

    endif()

endif()
