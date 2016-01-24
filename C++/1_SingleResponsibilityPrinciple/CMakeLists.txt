cmake_minimum_required(VERSION 2.6)
project(calculator)

add_definitions("-std=c++14")

include_directories(
    ./
    ./Parsers/
    ./Printers/
)

add_executable(calculator main.cpp)

install(TARGETS calculator RUNTIME DESTINATION bin)