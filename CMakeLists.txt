#
# CMakeLists.txt
#
# Build file for treebinding project
#
cmake_minimum_required(VERSION 3.10.1)

set(${PROJECT_NAME} treebinding)

project(${PROJECT_NAME} CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Use s++17, because gcc can't find std::enable_if_t for c++11
set (CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wpedantic -Werror -std=c++17")
endif()

# Add conan depencies
execute_process(COMMAND conan install ${CMAKE_BINARY_DIR}/.. -s build_type=${CMAKE_BUILD_TYPE} --build=missing
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        RESULTS_VARIABLE GENERATOR_RETURN
        OUTPUT_VARIABLE GENERATOR_OUTPUT
        )
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Check, that origin sources builds
add_subdirectory (TreeBinding)

# Compile one header generator
add_subdirectory (one_header_generator)

# Generate one header file from all sources
add_custom_target(run_one_header_generator ALL
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/generator ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS generator
        )

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

set(INCLUDES
    ${INCLUDES}
    ${INCLUDE_DIR}/TreeBinding.h
)

# Include all directories to keep navigation in CLion in origin sources in folder TreeBindiing
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_DIR tests)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory (${TEST_DIR})
endif()

# REPORT
message( STATUS "")
message( STATUS "General configuration for ${PROJECT_NAME}")
message( STATUS "======================================")
message( STATUS "")
message( STATUS "   Configuration: ${CMAKE_BUILD_TYPE}")
message( STATUS "")
