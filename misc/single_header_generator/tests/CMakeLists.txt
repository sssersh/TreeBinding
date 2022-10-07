project(gtest_generator CXX)

include(GoogleTest)

set(GENERATOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(SOURCES
    test_logger.cpp
    test_file.cpp
    test_utils.cpp
    test_generator_info.cpp
    test_generator.cpp
    )

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${GENERATOR_SRC_DIR}/file.cpp
    ${GENERATOR_SRC_DIR}/generator.cpp
    ${GENERATOR_SRC_DIR}/utils.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${GENERATOR_SRC_DIR}
        ${TEST_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIBS}
    ${CONAN_LIBS}
    -lstdc++fs)

#    ${CMAKE_THREAD_LIBS_INIT}

#    gtest_main
#    gtest
#    pthread