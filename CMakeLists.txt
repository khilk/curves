cmake_minimum_required(VERSION 3.23)
project(cad)
set(CMAKE_CXX_STANDARD 20)

add_library(curves SHARED curves.h curves.cpp)
find_package(TBB)
target_link_libraries(curves TBB::tbb)

add_executable(cad main.cpp)
target_link_libraries(cad curves)
