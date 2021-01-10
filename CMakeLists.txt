cmake_minimum_required(VERSION 3.10)
project(ObjectTracker)

set (CMAKE_CXX_STANDARD 17)

include_directories(include)

find_package(OpenCV REQUIRED)
add_executable(ObjectTracker src/main.cpp src/libtracker.cpp)
target_link_libraries(ObjectTracker ${OpenCV_LIBS})