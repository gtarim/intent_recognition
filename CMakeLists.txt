cmake_minimum_required(VERSION 3.5.0)

project(intent_recognition)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

option(TEST "Build the tests" OFF)
if( TEST )
    message("-- test enabled !")
    enable_testing()
    add_subdirectory(third_party/googletest)
    add_subdirectory(test)
endif()

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

set(SOURCES main.cpp)

add_executable(intent_recognition ${SOURCES})

set_target_properties(intent_recognition PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")

