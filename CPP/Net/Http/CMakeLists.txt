cmake_minimum_required(VERSION 2.8)

project(Http)

add_definitions(-D__LINUX__)

include_directories(../Socket/  ../../libs/  ../../libs/Utils ../../libs/jsoncpp ./ ../)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")


set(SOURCE_FILE main.cpp ../../libs/jsoncpp/jsoncpp.cpp ../../libs/Utils/ThreadBase.cpp)

add_executable(Http ${SOURCE_FILE})

target_link_libraries(Http pthread)