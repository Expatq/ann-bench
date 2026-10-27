add_library(ann_util INTERFACE)
add_library(ann::util ALIAS ann_util)

target_sources(ann_util INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/timer.h")
target_include_directories(ann_util INTERFACE "${PROJECT_SOURCE_DIR}/src")
target_compile_features(ann_util INTERFACE cxx_std_23)
