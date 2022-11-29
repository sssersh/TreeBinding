
project(generator)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/core/utils.cpp
    src/core/fs_adapter.cpp
    src/core/file.cpp
#    src/core/file_formatter.cpp
#    src/core/files_provider.cpp
#    src/main.cpp
    #    generator.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

if (WITH_TESTS)
    add_subdirectory(tests)
endif()
