set(SRCS_OCL
    "./ocl.cpp"
)
set(HEADERS_OCL
    "./ocl.h"
)

add_library(oclwrapper SHARED ${SRCS_OCL} ${HEADERS_OCL})

target_link_libraries(oclwrapper
  ${OpenCL_LIBRARY}
)

target_include_directories(oclwrapper
  PUBLIC
  .
  ${OpenCL_INCLUDE_DIRS}
)

