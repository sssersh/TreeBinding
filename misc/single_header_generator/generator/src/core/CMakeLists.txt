
project(generator_core)

set(SOURCES
    utils.cpp
    file.cpp
    file_formatter.cpp
    files_provider.cpp
#    generator.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})

if (WITH_TESTS)
    add_subdirectory(tests)
endif()
