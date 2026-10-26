add_library(ann_dataset STATIC
  dataset.cpp
  dataset.h
)
add_library(ann::dataset ALIAS ann_dataset)

target_link_libraries(ann_dataset
  PUBLIC ann::common
  PRIVATE ann::io
)
