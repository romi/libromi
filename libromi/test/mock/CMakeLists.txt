
project(romimocks)

#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include")
add_library(romimocks INTERFACE)
target_include_directories(romimocks INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
