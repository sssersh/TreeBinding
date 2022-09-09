
function(check_and_install_dependencies)
    find_package(GTest)
    find_package(Boost) # TODO: only COMPONENTS XXX wich is necessary

    if (GoogleTest_FOUND OR Boost_FOUND)
        message(STATUS "Some packages not found, install it by conan")
        find_program(conan_FOUND conan)
        if(conan_FOUND)
            execute_process(
                    COMMAND conan install ${CMAKE_SOURCE_DIR} -s build_type=${CMAKE_BUILD_TYPE} --build=missing
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    RESULTS_VARIABLE GENERATOR_RETURN
                    OUTPUT_VARIABLE GENERATOR_OUTPUT
                    )
            include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
            conan_basic_setup()
        else(conan_FOUND)
            # TODO1: pass path to libraries
            message(FATAL_ERROR "Error: conan command not found!")
        endif()
    endif()
endfunction()
