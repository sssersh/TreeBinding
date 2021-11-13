#
# CMakeLists.txt
#
# Build file for the tests
#

cmake_minimum_required(VERSION 3.10.1)
set(TEST_PROJECT_NAME tests)
project(${TEST_PROJECT_NAME} CXX)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

include(GoogleTest)

# from list of files create tests test_name.cpp -> test_name
foreach(_test_file ${TEST_SRC_FILES})
	get_filename_component(_test_name ${_test_file} NAME_WE)
	add_executable(${_test_name} ${_test_file})
	target_link_libraries(${_test_name} ${CMAKE_THREAD_LIBS_INIT} ${LIBS} ${CONAN_LIBS})
	gtest_discover_tests(${_test_name})
	#	add_test(${_test_name} ${_test_name})
    #	set_tests_properties(${_test_name} PROPERTIES TIMEOUT 5)
endforeach()

# Target for run all tests
add_executable(runAllTests ${TEST_SRC_FILES} src/test_json.cpp)
target_link_libraries(runAllTests ${CMAKE_THREAD_LIBS_INIT} ${LIBS} ${CONAN_LIBS})
