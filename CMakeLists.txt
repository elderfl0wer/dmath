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

file(GLOB_RECURSE DMATH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(dmath STATIC ${DMATH_SOURCES})

target_compile_features(dmath PUBLIC cxx_std_23)

target_include_directories(dmath
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_options(dmath PRIVATE /W4)
else()
    target_compile_options(dmath PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -O2
        -flto
    )
endif()


# ==============================================================================
# Tests
# ==============================================================================

option(BUILD_TESTS "Build dmath tests" ON)

if(BUILD_TESTS)

    enable_testing()

    file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    )

    foreach(TEST_FILE ${TEST_FILES})

        # Get filename without extension
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

        add_executable(${TEST_NAME}
            ${TEST_FILE}
        )

        target_link_libraries(${TEST_NAME}
            PRIVATE dmath
        )

        add_test(
            NAME ${TEST_NAME}
            COMMAND ${TEST_NAME}
        )

        # Put tests into build/tests/
        set_target_properties(${TEST_NAME}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY
            "${CMAKE_BINARY_DIR}/tests"
        )

    endforeach()

endif()


# ==============================================================================
# Output directories
# ==============================================================================

set_target_properties(dmath
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin"
)
