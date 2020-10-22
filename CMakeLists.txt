cmake_minimum_required(VERSION 3.10)
project(video2gif)

find_package( OpenCV REQUIRED )
find_package(ImageMagick 7.0 COMPONENTS Magick++ MagickCore)
include_directories( ${OpenCV_INCLUDE_DIRS} ${ImageMagick_INCLUDE_DIRS})

set(SOURCE_FILES src/video2gif.cpp src/Video.cpp)
add_executable(video2gif ${SOURCE_FILES})

if(ImageMagick_FOUND)
    # Ask about CXX and lib flags/locations
    set(MAGICK_CONFIG ${MAGICK_CONFIG} CACHE STRING "Path to Magick++-config utility")
    execute_process(COMMAND "${MAGICK_CONFIG}" "--cxxflags" OUTPUT_VARIABLE MAGICK_CXX_FLAGS)
    execute_process(COMMAND "${MAGICK_CONFIG}" "--libs" OUTPUT_VARIABLE MAGICK_LD_FLAGS)
    # Add these to cache
    set(MAGICK_CXX_FLAGS "${MAGICK_CXX_FLAGS}" CACHE STRING "ImageMagick configuration specific compilation flags." )
    set(MAGICK_LD_FLAGS  "${MAGICK_LD_FLAGS}" CACHE STRING "ImageMagick configuration specific linking flags.")
    # Split into list:
    string(REGEX MATCHALL "([^\ ]+)" MAGICK_CXX_FLAGS "${MAGICK_CXX_FLAGS}")
    string(REGEX MATCHALL "([^\ ]+)" MAGICK_LD_FLAGS "${MAGICK_LD_FLAGS}")
    # Remove trailing whitespace (CMAKE warns about this)
    string(STRIP "${MAGICK_CXX_FLAGS}" MAGICK_CXX_FLAGS)
    string(STRIP "${MAGICK_LD_FLAGS}" MAGICK_LD_FLAGS)

    #target_compile_options(video2gif ${MAGICK_CXX_FLAGS})
    target_link_libraries(video2gif ${MAGICK_LD_FLAGS})

endif(ImageMagick_FOUND)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${MAGICK_CXX_FLAGS} ${MAGICK_LD_FLAGS}")
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

target_link_libraries( video2gif ${OpenCV_LIBS} ${ImageMagick_LIBRARIES} )


