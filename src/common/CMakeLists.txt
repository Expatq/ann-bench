add_library(ann_common STATIC
  backend.h
  config.h
  metrics.h
  search_result.cpp
  search_result.h
  types.cpp
  types.h
)
add_library(ann::common ALIAS ann_common)

target_include_directories(ann_common PUBLIC "${PROJECT_SOURCE_DIR}")
target_compile_features(ann_common PUBLIC cxx_std_23)
