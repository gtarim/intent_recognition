cmake_minimum_required( VERSION 3.5 )

project( intent_recognition_test )

enable_testing()

find_package( GTest REQUIRED )
include( GoogleTest )

include_directories( ../ )

set( TEST_SOURCES test.cpp ../recognition.cpp )
add_executable( intent_recognition_test ${TEST_SOURCES} )

target_link_libraries( intent_recognition_test gtest_main )

add_test( NAME intent_recognition_test COMMAND intent_recognition_test )
gtest_discover_tests( intent_recognition_test )

set_target_properties( intent_recognition_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../test/" )