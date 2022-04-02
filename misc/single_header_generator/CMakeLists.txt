#
# Build one header generator and run it
#

cmake_minimum_required(VERSION 3.10.1)

project(generator)

set(SOURCES generator.cpp)
set(EXE generator)

add_executable(${EXE} ${SOURCES})

# stdc++fs - lib for std::experimental::filesystem
target_link_libraries(${EXE} stdc++fs)
