cmake_minimum_required(VERSION 2.8)e
project( video2gif )
find_package( OpenCV REQUIRED )
find_package(ImageMagick COMPONENTS Magick++)
include_directories( ${OpenCV_INCLUDE_DIRS} ${ImageMagick_INCLUDE_DIRS})
add_executable( video2gif video2gif.cpp )
target_link_libraries( video2gif ${OpenCV_LIBS} ${ImageMagick_LIBRARIES} )

########################################
#  Magick++
########################################
find_file(MAGICK_CONFIG_EXE "Magick++-config" PATHS
    "/usr/bin"
    "/usr/local/bin"
)
if (MAGICK_CONFIG_EXE)
  message(STATUS "Found Image Magick++ libaries -- Enabling MagickPainter.")
  execute_process(COMMAND Magick++-config --cppflags OUTPUT_VARIABLE Magick_CPP_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND Magick++-config --cxxflags OUTPUT_VARIABLE Magick_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND Magick++-config --ldflags  OUTPUT_VARIABLE Magick_LD_FLAGS  OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND Magick++-config --libs     OUTPUT_VARIABLE Magick_LIBS      OUTPUT_STRIP_TRAILING_WHITESPACE)
  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Magick_CPP_FLAGS} ${Magick_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${Magick_LIBS} ${Magick_LD_FLAGS}")
  
  
# remove_definitions(-DUSE_MAGICK_PAINTER)
endif (MAGICK_CONFIG_EXE)
