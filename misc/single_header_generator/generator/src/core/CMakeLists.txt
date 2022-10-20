
project(generator_core)

set(SOURCES
    utils.cpp
    file.cpp
    file_formatter.cpp
    files_provider.cpp
#    generator.cpp
#    i_files_provider.h
#    i_generator.h
)

add_library(${PROJECT_NAME} ${SOURCES})

if (WITH_TESTS)
    add_subdirectory(tests)
endif()
