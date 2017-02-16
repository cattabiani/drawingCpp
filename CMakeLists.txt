cmake_minimum_required(VERSION 2.6)
project(drawingcpp)

add_executable(drawingcpp drawingcpp.cpp)

install(TARGETS drawingcpp RUNTIME DESTINATION bin)
