#
# Build one header generator and run it
#

project(single_header_generator)

set(SOURCES
    logger.cpp
    file.cpp
    generator.cpp
)
set(EXE generator)

add_executable(${EXE} ${SOURCES})

# stdc++fs - lib for std::experimental::filesystem
target_link_libraries(${EXE} stdc++fs)
