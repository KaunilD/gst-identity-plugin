set(SRCS_RGB2GRAY
    "./gstoclrgb2gray.cpp"
)
set(HEADERS_RGB2GRAY
    "./gstoclrgb2gray.h"
)

add_library(oclrgb2gray SHARED ${SRCS_RGB2GRAY} ${HEADERS_RGB2GRAY})

target_link_libraries(oclrgb2gray
  ${GSTLIBS_LIBRARIES} ${GSTCPP_LIBRARIES} 
)

target_include_directories(oclrgb2gray
  PUBLIC
  .
  ${GSTLIBS_LIBRARY_DIRS}
  ${GSTCPP_INCLUDE_DIRS}
  /usr/include/gstreamer-1.0
  /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include 
  /usr/include/glib-2.0 
  /usr/lib/aarch64-linux-gnu/glib-2.0/include/
  /usr/include/libxml2 
  /usr/include/libxml++-2.6
)

add_custom_target(install_oclrgb2gray ALL
    COMMAND mv ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/liboclrgb2gray.so $ENV{GST_PLUGIN_PATH}
    DEPENDS oclrgb2gray
)


