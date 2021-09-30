#
# CMakeLists.txt
#
# Build file for the tests
#

cmake_minimum_required(VERSION 3.10.1)
set(PROJECT_NAME_STR tests)
project(${PROJECT_NAME_STR} CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
get_filename_component(DEPS_ROOT "${PROJECT_BINARY_DIR}/deps" ABSOLUTE)

# Use s++17, because gcc can't find std::enable_if_t for c++11
set(CMAKE_CXX_STANDARD 17)

include(ExtProjectUtils)

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

# Will download external CMakeable project from git repo, branch "master" and install it in $DEPS_ROOT
# This also will create "googletest.git" target, which we'll use as dependency for our test project
ExtProjectGit("https://github.com/google/googletest.git" "master" ${DEPS_ROOT} CMAKE_ARGS "${_OPT_CMAKE_ARGS}")

include_directories("${DEPS_ROOT}/include")
link_directories("${DEPS_ROOT}/lib")
link_directories("${DEPS_ROOT}/lib64")

#-------------------
# Test
#-------------------
enable_testing()

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# from list of files we'll create tests test_name.cpp -> test_name
foreach(_test_file ${TEST_SRC_FILES})
	get_filename_component(_test_name ${_test_file} NAME_WE)
	add_executable(${_test_name} ${_test_file})
	add_dependencies(${_test_name} "googletest.git")
	target_link_libraries(${_test_name} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})
	add_test(${_test_name} ${_test_name})
	set_tests_properties(${_test_name} PROPERTIES TIMEOUT 5)
endforeach()
