cmake_minimum_required(VERSION 3.21)
project(SplayTree)

add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 17)

add_executable(SplayTree main.cpp src)
