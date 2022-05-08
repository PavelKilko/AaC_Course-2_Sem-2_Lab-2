project(tests)

add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(tests test.cpp)

target_link_libraries(tests gtest gtest_main)