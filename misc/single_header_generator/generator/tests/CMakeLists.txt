project(gtest_generator CXX)

include(GoogleTest)

set(GENERATOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(SOURCES
    test_logger.cpp
    test_file_formatter.cpp
    test_file.cpp
    test_utils.cpp
    test_files_provider.cpp
#    test_generator.cpp
    )

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${GENERATOR_SRC_DIR}/core/utils.cpp
    ${GENERATOR_SRC_DIR}/core/file.cpp
    ${GENERATOR_SRC_DIR}/core/file_formatter.cpp
    ${GENERATOR_SRC_DIR}/core/files_provider.cpp
#    ${GENERATOR_SRC_DIR}/generator.cpp
    ${GENERATOR_SRC_DIR}/core/utils.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${GENERATOR_SRC_DIR}/core
        ${GENERATOR_SRC_DIR}/core/include
#        ${TEST_INCLUDE_DIR}
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
