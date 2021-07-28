#
# CMakeLists.txt
#
# Build file for the tests
#

cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Build the Google Test library
add_subdirectory(vendor)

add_executable(
	tests
	src/tests.cpp
)
add_test(NAME check COMMAND tests)

target_include_directories (
    tests
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Details
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Details/Parsers
)

# Link against Google Test library
target_link_libraries(tests gmock_main)