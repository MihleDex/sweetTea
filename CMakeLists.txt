cmake_minimum_required(VERSION 3.28)
project(sweetTea)

set(CMAKE_CXX_STANDARD 20)

add_executable(sweetTea main.cpp)


find_package(raylib REQUIRED)

target_link_libraries(sweetTea raylib)