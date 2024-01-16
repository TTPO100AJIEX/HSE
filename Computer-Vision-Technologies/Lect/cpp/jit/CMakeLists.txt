cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(jit)

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PillowResize REQUIRED)

add_executable(main main.cpp ../get_input.cpp)
include_directories(main "../")
target_link_libraries(main torch)
target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(main PillowResize)

set_property(TARGET main PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")
set(CMAKE_BUILD_TYPE Release)