cmake_minimum_required(VERSION 3.5)
project(CryptographySystem)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp/include
        /usr/local/include/
)

link_directories(/usr/local/include/lib)

add_executable(CryptographySystem main.cpp csapp.c UserProxy.cpp SourceManager.cpp SourceManager.h)

target_link_libraries(CryptographySystem
        SQLiteCpp
        sqlite3
        pthread
        dl
        boost_system
        boost_filesystem
        crypto
        )