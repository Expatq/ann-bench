add_library(ann_full_scan STATIC
  backend.cpp
  backend.h
  backend_factory.cpp
  backend_factory.h
)
add_library(ann::full_scan ALIAS ann_full_scan)

target_link_libraries(ann_full_scan PUBLIC ann::common)
