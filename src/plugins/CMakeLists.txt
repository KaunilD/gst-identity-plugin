set(SRCS_IDENTITY
    "./gstoclrgb2gray.cpp"
    "./ocl.cpp"
)
set(HEADERS_IDENTITY
    "./gstoclrgb2gray.h"
    "ocl.h"
)

add_library(oclrgb2gray SHARED ${SRCS_IDENTITY} ${HEADERS_IDENTITY})

target_link_libraries(oclrgb2gray
  ${GSTLIBS_LIBRARIES} ${OpenCL_LIBRARY}
)

target_include_directories(oclrgb2gray
  PUBLIC
  .
  ${GSTLIBS_LIBRARY_DIRS}
  ${OpenCL_INCLUDE_DIRS}
)

add_custom_target(install_oclrgb2gray ALL
    COMMAND mv ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/liboclrgb2gray.so $ENV{GST_PLUGIN_PATH}
    DEPENDS oclrgb2gray
)


