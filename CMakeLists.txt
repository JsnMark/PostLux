cmake_minimum_required(VERSION 4.0.1)
project(test_matrix)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/085af2cc08600bdb13827ca40261abcbe5048bb5.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

find_package(OpenCV REQUIRED core highgui imgcodecs)    
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(playground tests/playground.cpp)
target_link_libraries(playground ${OpenCV_LIBS})

add_executable(test_matrix tests/test_matrix.cpp)
target_link_libraries(test_matrix gtest_main)

include(GoogleTest)
gtest_discover_tests(test_matrix)
