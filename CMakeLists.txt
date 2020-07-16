cmake_minimum_required(VERSION 3.13)

project(MidiPiano VERSION 0.0.1)

include_directories("inc/core")

file(GLOB LIBRARY_SOURCES "src/core/*.cpp")

add_library(MidiPianoCore ${LIBRARY_SOURCES})

add_executable(MidiPiano "src/main.cpp")
target_link_libraries(MidiPiano MidiPianoCore)
