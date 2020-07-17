cmake_minimum_required(VERSION 3.1.0)

project(MidiPiano LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)


#
# Google Test Setup
#

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()


#
# Library Setup
#
set(LIBRARY_DIR "core")
set(LIBRARY_NAME "MidiPianoCore")

set(LIBRARY_HEADERS
	"${LIBRARY_DIR}/midi_piano.h"
)

set(LIBRARY_SOURCES
	"${LIBRARY_DIR}/midi_piano.cpp"
)

add_library("${LIBRARY_NAME}" STATIC "${LIBRARY_SOURCES}" "${LIBRARY_HEADERS}")


#
# Test Suite Setup
#
set(TEST_SUITE_DIR "test")
set(TEST_RUNNER_EXECUTABLE_NAME "MidiPianoTestRunner")
set(TEST_RUNNER_EXECUTABLE_SOURCES
	"${TEST_SUITE_DIR}/test_runner.cpp"
 "test/core/midi_piano_test.cpp")
add_executable("${TEST_RUNNER_EXECUTABLE_NAME}" "${TEST_RUNNER_EXECUTABLE_SOURCES}")
target_include_directories("${TEST_RUNNER_EXECUTABLE_NAME}" PRIVATE "${LIBRARY_DIR}")
target_link_libraries("${TEST_RUNNER_EXECUTABLE_NAME}" gtest_main "${LIBRARY_NAME}")


#
# Executable Setup
#
set(EXECUTABLE_DIR "main")
set(EXECUTABLE_NAME "${PROJECT_NAME}")

set(EXECUTABLE_SOURCES
	"${EXECUTABLE_DIR}/main.cpp"
)

add_executable("${EXECUTABLE_NAME}" "${EXECUTABLE_SOURCES}")
target_include_directories("${EXECUTABLE_NAME}" PRIVATE "${LIBRARY_DIR}")
target_link_libraries("${EXECUTABLE_NAME}" "${LIBRARY_NAME}")
