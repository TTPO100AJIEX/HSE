cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(onnx)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")

find_package(OpenCV REQUIRED)
find_package(PillowResize REQUIRED)

add_executable(main main.cpp ../get_input.cpp)
include_directories(main "../")
include_directories (main "${CMAKE_SOURCE_DIR}/onnxruntime/include")
target_link_libraries(main "${CMAKE_SOURCE_DIR}/onnxruntime/lib/libonnxruntime.so.1.16.3")
target_link_libraries(main "${CMAKE_SOURCE_DIR}/onnxruntime/lib/libonnxruntime_providers_cuda.so")
target_link_libraries(main PillowResize)
target_link_libraries(main ${OpenCV_LIBS})
set_property(TARGET main PROPERTY CXX_STANDARD 17)