
project(generator)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src/core)

#target_include_directories(src/include)

set(SOURCES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE generator_core)

#if (WITH_TESTS)
#    add_subdirectory(tests)
#endif()
