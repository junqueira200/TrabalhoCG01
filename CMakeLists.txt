cmake_minimum_required(VERSION 3.13)
project(TrabalhoCG01)

set(CMAKE_CXX_STANDARD 14)

add_executable(TrabalhoCG01 main.cpp)

target_link_libraries(TrabalhoCG01  -lglut -lGL -lGLU)

