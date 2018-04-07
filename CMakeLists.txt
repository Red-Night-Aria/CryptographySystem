cmake_minimum_required(VERSION 3.5)
project(CryptographySystem)

set(CMAKE_CXX_STANDARD 11)

add_executable(CryptographySystem main.cpp csapp.c UserProxy.cpp SourceManager.cpp SourceManager.h)

target_link_libraries(CryptographySystem pthread)