#
# CMakeLists.txt
#
# Build file for treebinding project
#
cmake_minimum_required(VERSION 3.10.1)

set(${PROJECT_NAME} treebinding)

project(${PROJECT_NAME} CXX)

# Use s++17, because gcc can't find std::enable_if_t for c++11
set (CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

set(INCLUDES
   ${INCLUDES}
        ${INCLUDE_DIR}/TreeBinding.h
)

# Include all directories to keep navigation in CLion in origin sources in folder TreeBindiing
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (one_header_generator)

set(TEST_DIR tests)

#if(BUILD_TESTING)
    add_subdirectory (${TEST_DIR})
#endif()

