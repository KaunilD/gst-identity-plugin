set(SRCS_IDENTITY
    "./gstidentity.cpp"
)
set(HEADERS_IDENTITY
    "./gstidentity.h"
)

add_library(gst-identity-plugin SHARED ${SRCS_IDENTITY} ${HEADERS_IDENTITY})

target_link_libraries(gst-identity-plugin
  ${GSTLIBS_LIBRARIES}
)

target_include_directories(gst-identity-plugin
  PUBLIC
  .
  ${GSTLIBS_LIBRARY_DIRS}
)

add_custom_target(install_gst_identity_plugin ALL
    COMMAND cp ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libgst-identity-plugin.so $ENV{GST_PLUGIN_PATH}
    DEPENDS gst-identity-plugin
)

