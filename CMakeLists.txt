cmake_minimum_required(VERSION 3.7)

project(jsdp)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

include_directories(include "include")
file(GLOB SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC "src/Sdp.cpp" "include/Sdp.h")
# add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
