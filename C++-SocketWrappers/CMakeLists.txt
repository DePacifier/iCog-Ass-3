cmake_minimum_required(VERSION 3.2)

include_directories(${CMAKE_SOURCE_DIR})

add_library(testClient "testClient.cpp")

add_library(serverDemo "serverDemo.cpp")

add_executable(client "${CMAKE_BINARY_DIR}/testClient.cpp")

add_executable(demo "${CMAKE_BINARY_DIR}/serverDemo.cpp")
