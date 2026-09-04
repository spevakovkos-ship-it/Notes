cmake_minimum_required(VERSION 4.0.0)

project(notes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(app notes.cpp)

install(TARGETS app DESTINATION bin)