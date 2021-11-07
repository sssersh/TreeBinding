#
# CMakeLists.txt
#
# Build file for the tests
#

cmake_minimum_required(VERSION 3.10.1)
set(TEST_PROJECT_NAME tests)
project(${TEST_PROJECT_NAME} CXX)

# Use s++17, because gcc can't find std::enable_if_t for c++11
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ") #-Wall -ansi -Wno-deprecated
endif()

if(MSVC)
	#vc 2012 fix for vararg templates
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_VARIADIC_MAX=10")
endif()

if(WIN32)
	set(_OPT_CMAKE_ARGS "-Dgtest_force_shared_crt=ON;-DCMAKE_SH=CMAKE_SH-NOTFOUND")
else()
	set(_OPT_CMAKE_ARGS "")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()






#-------------------
# Test
#-------------------
enable_testing()

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

message("${TEST_SRC_FILES}")

# from list of files we'll create tests test_name.cpp -> test_name
foreach(_test_file ${TEST_SRC_FILES})
	get_filename_component(_test_name ${_test_file} NAME_WE)
	add_executable(${_test_name} ${_test_file})
	target_link_libraries(${_test_name} ${CMAKE_THREAD_LIBS_INIT} ${LIBS} ${CONAN_LIBS})
	add_test(${_test_name} ${_test_name})
	set_tests_properties(${_test_name} PROPERTIES TIMEOUT 5)
endforeach()
