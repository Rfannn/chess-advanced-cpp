cmake_minimum_required(VERSION 3.16)
project(ChessCpp17 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

#console version
add_executable(chess
    src/main.cpp
    src/common.cpp
    src/Board.cpp
    src/Piece.cpp
    src/Renderer.cpp
    src/Game.cpp
)

if(MSVC)
    target_compile_options(chess PRIVATE /W4 /utf-8)
else()
    target_compile_options(chess PRIVATE -Wall -Wextra)
endif()

#gui version(win32 api)
if(WIN32)
    add_executable(chess_gui WIN32
        src/main_gui.cpp
        src/common.cpp
        src/Board.cpp
        src/Piece.cpp
        src/Renderer.cpp
        src/Game.cpp
        src/GUI.cpp
    )
    target_link_libraries(chess_gui PRIVATE gdi32 user32)
    if(MSVC)
        target_compile_options(chess_gui PRIVATE /W4 /utf-8)
    else()
        target_compile_options(chess_gui PRIVATE -Wall -Wextra)
    endif()
endif()
