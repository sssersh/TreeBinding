project(gtest_generator CXX)

include(GoogleTest)

set(GENERATOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(SOURCES
        test_logger.cpp
#    file_unit.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
    ${GENERATOR_SRC_DIR}
    ${TEST_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${LIBS} ${CONAN_LIBS} -lstdc++fs)

#    ${CMAKE_THREAD_LIBS_INIT}

#    gtest_main
#    gtest
#    pthread