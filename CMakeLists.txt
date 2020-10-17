cmake_minimum_required(VERSION 3.17)
project(Arbol_Fibonacci)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-lX11")
set(CMAKE_CXX_FLAGS "-lpthread")

add_executable(Arbol_Fibonacci main.cpp)

target_link_libraries(Arbol_Fibonacci -lX11 -lpthread)
