cmake_minimum_required(VERSION 3.22)
project(WebServer)

set(CMAKE_CXX_STANDARD 23)

add_executable(WebServer main.cpp util.cpp util.h epoll.cpp epoll.h requestData.cpp requestData.h)
