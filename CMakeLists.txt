cmake_minimum_required(VERSION 2.8)
project( video2gif )

add_definitions("-std=c++1y")

find_package( OpenCV REQUIRED )

include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_search_module(MAGICK REQUIRED Magick++)

add_executable( video2gif video2gif.cpp )
target_link_libraries(video2gif ${MAGICK_LIBRARIES} ${OpenCV_LIBS})
include_directories(${OpenCV_INCLUDE_DIRS} ${MAGICK_INCLUDE_DIRS})
