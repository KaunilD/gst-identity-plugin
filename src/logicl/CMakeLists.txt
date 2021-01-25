set(SRCS_OPENCL
    "logicl.cpp"
)
set(HEADERS_OPENCL
    "logicl.h"
)

add_library(opencl_wrapper STATIC ${SRCS_OPENCL} ${HEADERS_OPENCL})

target_link_libraries(opencl_wrapper ${OpenCL_LIBRARY})

target_include_directories(opencl_wrapper
  PUBLIC
  .
  ${OpenCL_INCLUDE_DIRS}
)
