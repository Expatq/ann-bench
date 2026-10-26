add_library(ann_io STATIC
  file.cpp
  file.h
)
add_library(ann::io ALIAS ann_io)

target_include_directories(ann_io PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_compile_features(ann_io PUBLIC cxx_std_23)
