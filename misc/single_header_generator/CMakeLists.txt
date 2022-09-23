#
# Build one header generator and run it
#

project(generator)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    file.cpp
#    generator.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

if (WITH_TESTS)
    add_subdirectory(tests)
endif()
