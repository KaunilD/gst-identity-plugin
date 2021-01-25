cmake_minimum_required(VERSION 3.8)
project(gst-identity-example)

# setup project output paths
set(PROJECT_OUTPUT_DIR  ${PROJECT_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR})
set(PROJECT_INCLUDE_DIR ${PROJECT_OUTPUT_DIR}/include)

file(MAKE_DIRECTORY ${PROJECT_INCLUDE_DIR})
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)

message("-- system arch:  ${CMAKE_SYSTEM_PROCESSOR}")
message("-- output path:  ${PROJECT_OUTPUT_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)

find_package(PkgConfig REQUIRED)

# for overlaying fps counter 
pkg_check_modules (CAIRO REQUIRED cairo)

# 
pkg_check_modules (GSTLIBS REQUIRED 
    gobject-2.0
    gstreamer-1.0>=1.14 
    gstreamer-base-1.0 
    gstreamer-app-1.0 
    gstreamer-video-1.0 
    gstreamer-audio-1.0
)
find_package(OpenCL REQUIRED)
message("-- DEBUG: Found OpenCL includes at   : ${OpenCL_INCLUDE_DIRS}")
message("-- DEBUG: Found OpenCL libraries at  : ${OpenCL_LIBRARY}")

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${pkg-config --cflags --libs gstreamer-1.0} -Wall -Werror")

# build C/C++ interface
include_directories(
    ${PROJECT_INCLUDE_DIR} 
    /usr/include/gstreamer-1.0 
    /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include 
    
    /usr/include/glib-2.0 
    /usr/lib/aarch64-linux-gnu/glib-2.0/include/
    
    /usr/include/libxml2 
    /usr/include/libxml++-2.6 
)

add_subdirectory("src/")
add_subdirectory("src/plugins/")
#add_subdirectory("src/oclwrapper/")