cmake_minimum_required(VERSION 3.26)
project(LogPrint)

set(CMAKE_CXX_STANDARD 20)
INCLUDE_DIRECTORIES("/home/gyli/lib-gyli/fmt/include/fmt/")
LINK_DIRECTORIES("/home/gyli/lib-gyli/fmt/lib/")

add_executable(LogPrint main.cpp
        minilog.h)
target_link_libraries(LogPrint libfmt.a)


