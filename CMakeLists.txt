cmake_minimum_required (VERSION 3.21.1)

project(treebinding)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(
    ${PROJECT_NAME}
    src/Details/TreeBinding.cpp
)
target_include_directories (
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Details
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Details/Parsers    
)

include(CTest)
enable_testing()

if(BUILD_TESTING)
    add_subdirectory (tests)
endif()