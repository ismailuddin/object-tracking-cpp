cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 17)

project(Main)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

find_package(OpenCV REQUIRED)
add_executable(TrackObject ${SOURCES})
target_link_libraries(TrackObject ${OpenCV_LIBS})