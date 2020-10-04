cmake_minimum_required(VERSION 3.15.0)
project(win32_opengl)

add_library(win32_opengl "win32_opengl.h" "win32_opengl.cpp"  "win32_opengl_version.h" "win32_open_gl_init_exception.h")
target_include_directories(win32_opengl PUBLIC ".")
