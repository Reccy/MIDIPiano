cmake_minimum_required(VERSION 3.1.0)

project(MidiPiano LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Library Setup
set(LIBRARY_DIR "core")
set(LIBRARY_NAME "MidiPianoCore")

set(LIBRARY_HEADERS
	"${LIBRARY_DIR}/midi_piano.h"
)

set(LIBRARY_SOURCES
	"${LIBRARY_DIR}/midi_piano.cpp"
)

add_library("${LIBRARY_NAME}" STATIC "${LIBRARY_SOURCES}" "${LIBRARY_HEADERS}")

# Executable Setup
set(EXECUTABLE_DIR "main")
set(EXECUTABLE_NAME "${PROJECT_NAME}")

set(EXECUTABLE_SOURCES
	"${EXECUTABLE_DIR}/main.cpp"
)

add_executable("${EXECUTABLE_NAME}" "${EXECUTABLE_SOURCES}")
target_include_directories("${EXECUTABLE_NAME}" PRIVATE "${LIBRARY_DIR}")
target_link_libraries("${EXECUTABLE_NAME}" "${LIBRARY_NAME}")
