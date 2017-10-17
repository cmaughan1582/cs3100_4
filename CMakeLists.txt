cmake_minimum_required(VERSION 2.8)
project(Assign4)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -O3 ${CMAKE_CXX_FLAGS}")
endif()

add_executable(assign4 main.cpp)