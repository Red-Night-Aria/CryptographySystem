cmake_minimum_required(VERSION 3.9)
project(CryptographySystem)

set(CMAKE_CXX_STANDARD 11)

add_executable(CryptographySystem main.cpp csapp.c Responser.cpp Responser.h)

target_link_libraries(CryptographySystem pthread)