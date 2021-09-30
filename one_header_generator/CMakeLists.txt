
cmake_minimum_required(VERSION 3.10.1)

project(generator)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -std=c++17")

set(SOURCES generator.cpp)
set(EXE generator)

add_executable(${EXE} ${SOURCES})

# stdc++fs - lib for std::experimental::filesystem
target_link_libraries(${EXE} stdc++fs)

# Run compiled one header generator
execute_process(COMMAND ./${EXE} ${CMAKE_SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        RESULTS_VARIABLE GENERATOR_RETURN
        OUTPUT_VARIABLE GENERATOR_OUTPUT
        )

message("${GENERATOR_OUTPUT}")

if(GENERATOR_RETURN)
    message(FATAL_ERROR "")
    endif()
