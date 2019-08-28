cmake_minimum_required(VERSION 3.10)
project(wahaha)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(wahaha main.cpp)

#set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking

target_link_libraries(wahaha ${OpenCV_LIBS})
