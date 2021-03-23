cmake_minimum_required(VERSION 3.10)
project(togif)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} )

find_package( OpenCV REQUIRED )
find_package(ImageMagick 6.0 COMPONENTS Magick++ MagickCore)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src algs)




