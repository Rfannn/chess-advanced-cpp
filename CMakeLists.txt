cmake_minimum_required(VERSION 3.16)
project(ChessCpp17 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(chess src/main.cpp src/common.cpp src/Board.cpp)

if(MSVC)
    target_compile_options(chess PRIVATE /W4 /utf-8)
else()
    target_compile_options(chess PRIVATE -Wall -Wextra)
endif()
