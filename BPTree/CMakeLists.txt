cmake_minimum_required(VERSION 3.21)
project(BPTree)

add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 14)

add_executable(BPTree main.cpp src)
