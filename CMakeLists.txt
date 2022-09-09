#
# CMakeLists.txt
#
# Build file for creolization project
#
cmake_minimum_required(VERSION 3.10.1)

set(${PROJECT_NAME} creolization)
project(${PROJECT_NAME} CXX)

option(WITH_TESTS "Turn on/off build tests" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set (CMAKE_CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wpedantic -Werror")
endif()

include(${CMAKE_SOURCE_DIR}/cmake/check_and_install_dependencies.cmake)

# Now not used
#if (ENABLE_COVERAGE)
#    message("Enable coverage")
#    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
#    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
#endif()

if (WITH_TESTS)
    check_and_install_dependencies()
endif()

# Check, that origin sources are compilable
#include_directories(include)
#set(DUMMY include/creolization/details/Core/dummy.cpp)
#add_library(dummy_lib ${DUMMY})

# Compile one header generator
#add_subdirectory (misc/single_header_generator)

# Generate one header file from all sources
#add_custom_target(run_single_header_generator ALL
#        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/generator ${CMAKE_CURRENT_SOURCE_DIR}
#        DEPENDS generator
#        )

#set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/single_include)
#include_directories(${INCLUDE_DIR})
#set(INCLUDES
#    ${INCLUDES}
#    ${INCLUDE_DIR}/creolization.h
#)
#
#set(TEST_DIR tests)
#
#if(BUILD_TESTS)
#    enable_testing()
#    add_subdirectory (${TEST_DIR})
#endif()

# REPORT
#message( STATUS "")
#message( STATUS "General configuration for ${PROJECT_NAME}")
#message( STATUS "======================================")
#message( STATUS "")
#message( STATUS "   Configuration: ${CMAKE_BUILD_TYPE}")
#message( STATUS "")

# Include all directories to keep navigation in CLion in origin sources in folder include
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})
