cmake_minimum_required(VERSION 2.8)
project( video2gif )
find_package( OpenCV REQUIRED )
find_package(ImageMagick 6.7 COMPONENTS Magick++ MagickCore)
include_directories( ${OpenCV_INCLUDE_DIRS} ${ImageMagick_INCLUDE_DIRS})
add_executable( video2gif src/video2gif.cpp )
if(ImageMagick_FOUND)
    # Find Imagemagick Library directory
    get_filename_component(MAGICK_LIB_DIR ${ImageMagick_MagickCore_LIBRARY} DIRECTORY)
    # Find where Magick++-config lives
    file(GLOB_RECURSE MAGICK_CONFIG FOLLOW_SYMLINKS ${MAGICK_LIB_DIR}/Magick++-config)
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

target_link_libraries( video2gif ${OpenCV_LIBS} ${ImageMagick_LIBRARIES} )

